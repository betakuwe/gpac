/*
 *			GPAC - Multimedia Framework C SDK
 *
 *			Authors: Jean Le Feuvre
 *			Copyright (c) Telecom ParisTech 2012
 *					All rights reserved
 *
 *  This file is part of GPAC - sample DASH library usage
 *
 */

#include <gpac/dash.h>
#include <gpac/maths.h>
#include <gpac/thread.h>
#include <gpac/media_tools.h>
#include <gpac/constants.h>
#include <gpac/base_coding.h>
#include <gpac/download.h>

#ifdef GPAC_DISABLE_ISOM
#error "Cannot compile MP42TS if GPAC is not built with ISO File Format support"
#endif


static GFINLINE void PrintUsage(const char * progname) 
{
	fprintf(stderr, "USAGE: %s [OPTIONS] URL\n"
					"\n"
					"\t-ber=V             sets bit error rate as double. Default is 0\n"
					"\t-mtu=S             sets transport unit size in bytes (after removal of IP/TCP/others). Default is 1460\n"
					"\t-trace=F           sets trace file recording per segment stats\n"
					"\t-store-rand=R      records random distribution to file R\n"
					"\t-play-rand=R       reuses random distribution from file R\n"
					"\t-short             prints short report with tab separation\n"
					"\t-header            prints header for short report\n"
					"\t-cache=DIR         sets cache directory to DIR. use \"none\" to disable disk IO (default)\n"
					"\t-config=cfg        uses/generate CFG config file (default is in current dir)\n"
					"\t-retry-timeout     sets retry timeout after a 404 - default is 500ms\n"
					"\n"
#ifdef GPAC_MEMORY_TRACKING
					"\t-mem-track         enables memory tracking\n"
#endif
					"\t-logs=LOGS         set log tools and levels, formatted as a ':'-separated list of toolX[:toolZ]@levelX\n"
					"\t-h or -help        print this screen\n"
					"\n", progname);
}

typedef struct
{
	/*cfg and download manager*/
	GF_Config *cfg;
    GF_DownloadManager *downloader;

	/*DASH IO interface and DASH client*/
	GF_DASHFileIO dash_io;
	GF_DashClient *dash;

	Double ber;
	u32 mtu;
	u32 corrupte_byte_window;
	u64 bytes_processed;
	u64 next_corrupted_byte;
	u32 nb_windows;

	Double duration;

	u64 nb_samples, nb_samples_lost_full, nb_samples_lost_no_index, nb_samples_corr_no_index, nb_samples_lost_index, nb_samples_corr_index;
	u32 nb_seg_lost;

	Bool disable_cache;

	FILE *trace;
	FILE *rand_dist_file;
	Bool use_recorded_dist;

	Bool do_run;
	Bool connection_ack;

} GF_DashSim;

static Bool use_gaussian_rand = GF_FALSE;

static Double get_random(GF_DashSim *dsim)
{
	static Double V1, V2, S;
	static Bool phase = GF_FALSE;
	Double X;

	if (dsim->use_recorded_dist) {
		if (feof(dsim->rand_dist_file)) {
			GF_LOG(GF_LOG_ERROR, GF_LOG_APP, ("Not enough random numbers in input file, generating new ones ...\n"));
		} else {
			char szLine[100];
			fgets(szLine, 100, dsim->rand_dist_file);
			X = atof(szLine);
			return X;
		}
	}


	if (!use_gaussian_rand) {
		X = (Double) gf_rand() / RAND_MAX;
	} else {
		if (phase == GF_FALSE) {
			do {
				Double U1 = (Double) gf_rand() / RAND_MAX;
				Double U2 = (Double) gf_rand() / RAND_MAX;

				V1 = 2 * U1 - 1;
				V2 = 2 * U2 - 1;
				S = V1 * V1 + V2 * V2;

			} while(S >= 1 || S == 0);

			X = V1 * sqrt(-2 * log(S) / S);
			phase = GF_TRUE;
		} else {
			X = V2 * sqrt(-2 * log(S) / S);
			phase = GF_FALSE;
		}
	}

	if (dsim->rand_dist_file && !dsim->use_recorded_dist) {
		fprintf(dsim->rand_dist_file, "%g\n", X);
	}
	return X;
}


void dashsim_segment_netio(void *cbk, GF_NETIO_Parameter *param)
{
	if (param->msg_type == GF_NETIO_PARSE_HEADER) {
		GF_DashSim *dsim = (GF_DashSim *)cbk;
		if (!strcmp(param->name, "Dash-Newest-Segment")) {
			gf_dash_resync_to_segment(dsim->dash, param->value, gf_dm_sess_get_header(param->sess, "Dash-Oldest-Segment") );
		}
	}
	else if (param->msg_type == GF_NETIO_DATA_EXCHANGE) {
		//this is an end of HTTP chunkk - data could be send to the parser earlier than at the end of the segment
		if (param->reply) {
		}
	}
}


Bool dashsim_io_codec_supported(GF_DASHFileIO *dashio, const char *codec, u32 width, u32 height, Bool is_interlaced, u32 fps_num, u32 fps_denum, u32 sample_rate)
{
	//check if this codec and its properties are usefull for your device. If not , return 0
	return 1;
}

void dashsim_io_delete_cache_file(GF_DASHFileIO *dashio, GF_DASHFileIOSession session, const char *cache_url)
{
	gf_dm_delete_cached_file_entry_session((GF_DownloadSession *)session, cache_url);
}

GF_DASHFileIOSession dashsim_io_create(GF_DASHFileIO *dashio, Bool persistent, const char *url, s32 group_idx)
{
	GF_Err e=GF_OK;
	u32 flags = GF_NETIO_SESSION_NOT_THREADED;
	GF_DashSim *dsim = (GF_DashSim *)dashio->udta;
	GF_DownloadSession *sess;

	if (persistent)
		flags |= GF_NETIO_SESSION_PERSISTENT;

	if (dsim->disable_cache) 
		flags |= GF_NETIO_SESSION_MEMORY_CACHE;

	//this is the MPD downloader
	if (group_idx<0) {
		sess = gf_dm_sess_new(dsim->downloader, url, flags, NULL, NULL, &e);
	} 
	//this is a segment downloader
	else {
		sess = gf_dm_sess_new(dsim->downloader, url, flags, dashsim_segment_netio, dsim, &e);
	}
	return (GF_DASHFileIOSession ) sess;
}
void dashsim_io_del(GF_DASHFileIO *dashio, GF_DASHFileIOSession session)
{
	gf_dm_sess_del((GF_DownloadSession *)session);
}
void dashsim_io_abort(GF_DASHFileIO *dashio, GF_DASHFileIOSession session)
{
    gf_dm_sess_abort((GF_DownloadSession *)session);
}
GF_Err dashsim_io_setup_from_url(GF_DASHFileIO *dashio, GF_DASHFileIOSession session, const char *url, s32 group_idx)
{
	return gf_dm_sess_setup_from_url((GF_DownloadSession *)session, url);
}
GF_Err dashsim_io_set_range(GF_DASHFileIO *dashio, GF_DASHFileIOSession session, u64 start_range, u64 end_range, Bool discontinue_cache)
{
	return gf_dm_sess_set_range((GF_DownloadSession *)session, start_range, end_range, discontinue_cache);
}
GF_Err dashsim_io_init(GF_DASHFileIO *dashio, GF_DASHFileIOSession session)
{
	return gf_dm_sess_process_headers((GF_DownloadSession *)session);
}
GF_Err dashsim_io_run(GF_DASHFileIO *dashio, GF_DASHFileIOSession session)
{
	return gf_dm_sess_process((GF_DownloadSession *)session);
}
const char *dashsim_io_get_url(GF_DASHFileIO *dashio, GF_DASHFileIOSession session)
{
	return gf_dm_sess_get_resource_name((GF_DownloadSession *)session);
}
const char *dashsim_io_get_cache_name(GF_DASHFileIO *dashio, GF_DASHFileIOSession session)
{
	return gf_dm_sess_get_cache_name((GF_DownloadSession *)session);
}
const char *dashsim_io_get_mime(GF_DASHFileIO *dashio, GF_DASHFileIOSession session)
{
	return gf_dm_sess_mime_type((GF_DownloadSession *)session);
}
const char *dashsim_io_get_header_value(GF_DASHFileIO *dashio, GF_DASHFileIOSession session, const char *header_name)
{
	return gf_dm_sess_get_header((GF_DownloadSession *)session, header_name);
}
u64 dashsim_io_get_utc_start_time(GF_DASHFileIO *dashio, GF_DASHFileIOSession session)
{
	return gf_dm_sess_get_utc_start((GF_DownloadSession *)session);
}
u32 dashsim_io_get_bytes_per_sec(GF_DASHFileIO *dashio, GF_DASHFileIOSession session)
{
	u32 bps=0;
    gf_dm_sess_get_stats((GF_DownloadSession *)session, NULL, NULL, NULL, NULL, &bps, NULL);
	return bps;
}
u32 dashsim_io_get_total_size(GF_DASHFileIO *dashio, GF_DASHFileIOSession session)
{
	u32 size=0;
    gf_dm_sess_get_stats((GF_DownloadSession *)session, NULL, NULL, &size, NULL, NULL, NULL);
	return size;
}
u32 dashsim_io_get_bytes_done(GF_DASHFileIO *dashio, GF_DASHFileIOSession session)
{
	u32 size=0;
    gf_dm_sess_get_stats((GF_DownloadSession *)session, NULL, NULL, NULL, &size, NULL, NULL);
	return size;
}


void dashsim_process_file(GF_DashSim *dsim, u32 group_idx)
{
	GF_ISOFile *the_file;
	GF_Err e;
	u32 file_size;
	const char *url;
	u64 start_range, end_range, init_start_range, init_end_range, bytes_missing;
	u32 i, nb_samples, nb_samples_no_index, nb_corrupted_samples_no_index, nb_samples_index, nb_corrupted_samples_index;
	const char *init_url=NULL;
	const char *original_url;
	Bool has_next_segment;
	u32 corrupted = 0;
	u64 corr_start_range, corr_end_range;
	Double duration=0;

	start_range = end_range = init_start_range = init_end_range = 0;
	nb_samples = nb_samples_no_index = nb_samples_index = nb_corrupted_samples_no_index = nb_corrupted_samples_index = 0;
	file_size=0;

	the_file = gf_dash_get_group_udta(dsim->dash, group_idx);
	if (!the_file) {
		init_url = gf_dash_group_get_segment_init_url(dsim->dash, group_idx, &init_start_range, &init_end_range);
		e = gf_isom_open_progressive(init_url, init_start_range, init_end_range, &the_file, &bytes_missing);
		if (e != GF_OK) {
			fprintf(stderr, "Cannot open init segment %s: %s\n", init_url, gf_error_to_string( gf_isom_last_error(NULL) ) );
			goto exit;
		}
		gf_dash_set_group_udta(dsim->dash, group_idx, the_file);
		gf_dash_group_discard_segment(dsim->dash, group_idx);
	}

	gf_dash_group_get_next_segment_location(dsim->dash, group_idx, 0, &url, &start_range, &end_range, NULL, &init_url, &init_start_range, &init_end_range, &original_url, &has_next_segment, NULL, NULL);

	gf_dash_group_get_next_segment_size(dsim->dash, group_idx, &file_size);

	if (init_url) {
		gf_isom_delete(the_file);
		gf_dash_set_group_udta(dsim->dash, group_idx, NULL);
		e = gf_isom_open_progressive(init_url, init_start_range, init_end_range, &the_file, &bytes_missing);
		if (e != GF_OK) {
			fprintf(stderr, "Cannot open init segment %s: %s\n", init_url, gf_error_to_string( gf_isom_last_error(NULL) ) );
			goto exit;
		}
		gf_dash_set_group_udta(dsim->dash, group_idx, the_file);
	}

	gf_isom_reset_tables(the_file, GF_TRUE);

	gf_isom_push_corrupted_byte_range(the_file, 0, 0);

	e = gf_isom_open_segment(the_file, url, start_range, end_range, 0);
	if (e) {
		fprintf(stderr, "Cannot open segment %s: %s\n", url, gf_error_to_string( gf_isom_last_error(NULL) ) );
		goto exit;
	}

	nb_samples = gf_isom_get_sample_count(the_file, 1);
	nb_samples_no_index = nb_samples_index = nb_samples;
	nb_corrupted_samples_no_index = nb_corrupted_samples_index = 0;
	duration = gf_isom_get_media_duration(the_file, 1) - gf_isom_get_current_tfdt(the_file, 1);
	duration /= gf_isom_get_media_timescale(the_file, 1);


	if (dsim->corrupte_byte_window) {
		u32 first_file_pck = dsim->bytes_processed / dsim->mtu;
		u32 last_file_byte = (dsim->bytes_processed+file_size);
		while (1) {
			assert(dsim->next_corrupted_byte >= dsim->bytes_processed);
			if (dsim->next_corrupted_byte > last_file_byte) {
				break;
			}
			corrupted ++;

			corr_start_range = dsim->next_corrupted_byte - dsim->mtu * first_file_pck;
			corr_end_range = corr_start_range + dsim->mtu;

			gf_isom_push_corrupted_byte_range(the_file, corr_start_range, corr_end_range);

			dsim->next_corrupted_byte = dsim->nb_windows * dsim->corrupte_byte_window + get_random(dsim) * dsim->corrupte_byte_window;
			dsim->nb_windows++;
		}
	}
	dsim->bytes_processed += file_size;
	if (!corrupted) goto exit;

	//check without index simulation
	gf_isom_reset_tables(the_file, GF_TRUE);
	gf_isom_set_corrupted_byte_mode(the_file, GF_FALSE);
	gf_isom_open_segment(the_file, url, start_range, end_range, 0);

	nb_samples_no_index = gf_isom_get_sample_count(the_file, 1);
	nb_corrupted_samples_no_index = 0;
	for (i=0; i<nb_samples_no_index; i++) {
		u32 di;
		GF_ISOSample *samp = gf_isom_get_sample(the_file, 1, i+1, &di);
		if (!samp) nb_corrupted_samples_no_index++;
	}

	//check with index simulation
	gf_isom_reset_tables(the_file, GF_TRUE);
	gf_isom_set_corrupted_byte_mode(the_file, GF_TRUE);
	gf_isom_open_segment(the_file, url, start_range, end_range, 0);

	nb_samples_index = gf_isom_get_sample_count(the_file, 1);
	nb_corrupted_samples_index = 0;
	for (i=0; i<nb_samples_index; i++) {
		u32 di;
		GF_ISOSample *samp = gf_isom_get_sample(the_file, 1, i+1, &di);
		if (!samp) nb_corrupted_samples_index++;
	}

exit:

	if (e==GF_OK) {
		if (dsim->trace) {
			fprintf(dsim->trace, "%s\t%u\t%g\t%u\t%u\t%u\t%u\t%u\t%u\t%u\n", url, file_size, duration, corrupted, nb_samples, corrupted ? 0 : nb_samples, nb_samples_no_index, nb_corrupted_samples_no_index, nb_samples_index, nb_corrupted_samples_index);
		}

		dsim->nb_samples += nb_samples;
		dsim->nb_seg_lost += corrupted ? 1 : 0;
		dsim->nb_samples_lost_full += corrupted ? nb_samples : 0;
		dsim->nb_samples_lost_no_index += (nb_samples-nb_samples_no_index);
		dsim->nb_samples_corr_no_index += nb_corrupted_samples_no_index;
		dsim->nb_samples_lost_index += (nb_samples-nb_samples_index);
		dsim->nb_samples_corr_index += nb_corrupted_samples_index;

		dsim->duration += duration;
	}

	gf_dash_group_discard_segment(dsim->dash, group_idx);
}

GF_Err dashsim_io_on_dash_event(GF_DASHFileIO *dashio, GF_DASHEventType dash_evt, s32 group_idx, GF_Err error_code)
{
	u32 i;
	GF_DashSim *dsim = (GF_DashSim *)dashio->udta;

	if (dash_evt==GF_DASH_EVENT_PERIOD_SETUP_ERROR) {
		if (!dsim->connection_ack) {
			fprintf(stderr, "Cannot connect to DASH session: error %s\n", gf_error_to_string(error_code) );
			dsim->connection_ack=1;
			dsim->do_run = 0;
		}
		return GF_OK;
	}

	if (dash_evt==GF_DASH_EVENT_SELECT_GROUPS) {
		for (i=0; i<gf_dash_get_group_count(dsim->dash); i++) {
			/*finally select group based on user criteria*/
			gf_dash_group_select(dsim->dash, i, 1);
		}
		return GF_OK;
	}

	/*for all selected groups, create input service and connect to init/first segment*/
	if (dash_evt==GF_DASH_EVENT_CREATE_PLAYBACK) {
		/*select input services if possible*/
		for (i=0; i<gf_dash_get_group_count(dsim->dash); i++) {
			const char *mime, *init_segment;
			
			if (!gf_dash_is_group_selected(dsim->dash, i))
				continue;

			mime = gf_dash_group_get_segment_mime(dsim->dash, i);
			init_segment = gf_dash_group_get_segment_init_url(dsim->dash, i, NULL, NULL);

			/*this is a mistake in DASH*/
			if (!mime) {
				fprintf(stderr, "Cannot retrieve MIME type for group - not supported\n");
				dsim->do_run=GF_FALSE;
			}
			else if (!strcmp(mime, "video/mpeg-2") || !strcmp(mime, "video/mp2t") || !strcmp(mime, "video/mpeg")) {
				fprintf(stderr, "MPEG-2 Transport Stream detected, not supported by loss simulator\n");
				dsim->do_run=GF_FALSE;
			}
			else if (!strcmp(mime, "video/mp4") || !strcmp(mime, "video/3gp") || !strcmp(mime, "video/3gp2")
					|| !strcmp(mime, "audio/mp4") || !strcmp(mime, "audio/3gp") || !strcmp(mime, "audio/3gp2")
					|| (init_segment && gf_isom_probe_file(init_segment)) 
				) {
				gf_dash_group_select(dsim->dash, i, 1);
			}
			else {
				fprintf(stderr, "Unknown media file %s detected - skipping group\n", mime);
				dsim->do_run=GF_FALSE;
			}
		}
		if (!dsim->connection_ack) {
			dsim->connection_ack=1;
		}
		//if we were in a period, we're now ready to go
		return GF_OK;
	}

	/*for all running services, stop service*/
	else if (dash_evt==GF_DASH_EVENT_DESTROY_PLAYBACK) {
		//we only handle single period in the simulator , so this will exit
		dsim->do_run = 0;
		for (i=0; i<gf_dash_get_group_count(dsim->dash); i++) {
			GF_ISOFile *the_file = gf_dash_get_group_udta(dsim->dash, i);
			if (the_file) gf_isom_delete(the_file);
		}
		return GF_OK;
	}
	/*DASH client is buffering*/
	else if (dash_evt==GF_DASH_EVENT_BUFFERING) {
		return GF_OK;
	}
	/*DASH client is done buffering*/
	else if (dash_evt==GF_DASH_EVENT_BUFFER_DONE) {
		return GF_OK;
	}

	/*a new DASH segment is completely received*/
	else if (dash_evt==GF_DASH_EVENT_SEGMENT_AVAILABLE) {
		if (group_idx>=0)
			dashsim_process_file(dsim, group_idx);
	}
	

	return GF_OK;
}

static void on_gpac_logs(void *cbk, u32 ll, u32 lm, const char *fmt, va_list list)
{
	FILE *logs = cbk;
	fprintf(logs, "[UTC "LLU" THID %d]", gf_net_get_utc(), gf_th_id() );
	vfprintf(logs, fmt, list);
	fflush(logs);
}

int main(int argc, char **argv)
{
	/********************/
	/*   declarations   */
	/********************/
	GF_Err e;
	GF_DASHInitialSelectionMode mode = GF_DASH_SELECT_QUALITY_LOWEST;
	const char *config_file = NULL;
	const char *mpd_url = NULL;
	u32 i, last_check_time, cache_duration = 10;
	Bool enable_buffering = 0;
	Bool keep_files = 0;
	Bool mem_tracking = 0;
	Bool disable_switching = 0;
	u32 retry_timeout = 0;
	u32 initial_timeshift_buffer_percent = 50;
	char *cache_directory = NULL;
	FILE *logfile = NULL;
	GF_DashSim dsim;
	Bool is_short=GF_FALSE;
	Bool show_header=GF_FALSE;

#ifdef GPAC_MEMORY_TRACKING
	for (i=1; i<(u32) argc; i++) {
		if (!strcmp(argv[i], "-mem-track")) {
			mem_tracking = 1;
			break;
		}
	}
#endif
	/*****************/
	/*   gpac init   */
	/*****************/
	gf_sys_init(mem_tracking);
	gf_rand_init(GF_FALSE);
	gf_log_set_tool_level(GF_LOG_ALL, GF_LOG_WARNING);
	

	memset(&dsim, 0, sizeof(dsim));
	mode = GF_DASH_SELECT_BANDWIDTH_HIGHEST;

	/***********************/
	/*   parse arguments   */
	/***********************/
	for (i=1; i<(u32) argc; i++) {
		char *arg = argv[i];
		if (arg[0]!='-') {
			mpd_url = arg;
		}
		else if (!strnicmp(arg, "-logs=", 6)) {
			if (gf_log_set_tools_levels(arg+6) != GF_OK)
				return 0;
		}
		else if (!strnicmp(arg, "-logf=", 6)) {
			logfile = gf_fopen(arg+6, "wt");
			gf_log_set_callback(logfile, on_gpac_logs);
		}
		else if (!strnicmp(arg, "-trace=", 7)) {
			dsim.trace = gf_fopen(arg+7, "wt");
		}
		else if (!strnicmp(arg, "-cache=", 7)) {
			cache_directory = arg+7;
		} 
		else if (!strnicmp(arg, "-config=", 8)) {
			config_file = arg+8;
		} 
#ifndef GPAC_MEMORY_TRACKING
		else if (!strcmp(arg, "-mem-track")) {
			fprintf(stderr, "WARNING - GPAC not compiled with Memory Tracker - ignoring \"-mem-track\"\n"); 
		}
#endif
		else if (!strnicmp(arg, "-retry-timeout=", 15)) {
			retry_timeout = atoi(arg+15);
		}
		else if (!strnicmp(arg, "-ber=", 5)) {
			dsim.ber = atof(arg+5);
		}
		else if (!strnicmp(arg, "-mtu=", 5)) {
			dsim.mtu = atoi(arg+5);
		}
		else if (!strnicmp(arg, "-store-rand=", 12)) {
			dsim.rand_dist_file = fopen(arg+12, "wt");
			if (!dsim.rand_dist_file) {
				fprintf(stderr, "Cannot open random dist trace %s for record\n", arg+11);
				goto exit;
			}
		}
		else if (!strnicmp(arg, "-play-rand=", 11)) {
			dsim.rand_dist_file = fopen(arg+11, "rt");
			if (!dsim.rand_dist_file) {
				fprintf(stderr, "Cannot open random dist trace %s for playback\n", arg+11);
				goto exit;
			}
			dsim.use_recorded_dist = GF_TRUE;
		}
		else if (!strcmp(arg, "-short")) {
			is_short=GF_TRUE;
		}
		else if (!strcmp(arg, "-header")) {
			show_header=GF_TRUE;
		}
		else if (!strcmp(arg, "-h")) {
			PrintUsage(argv[0]);
			goto exit;
		}
	}

	if (!mpd_url) {
		fprintf(stderr, "No MPD URL specified - exiting.\n");
		PrintUsage(argv[0]);
	}

	if (!cache_directory || !strcmp(cache_directory, "none")) {
		cache_directory = NULL;
		dsim.disable_cache = 1;
	}

	if (!dsim.mtu) dsim.mtu = 1460;

	if (dsim.ber) {
		Double correct_bytes = 1/dsim.ber;
		dsim.corrupte_byte_window = ceil(correct_bytes/8);
		dsim.next_corrupted_byte = get_random(&dsim) * dsim.corrupte_byte_window;
		dsim.nb_windows=1;

		if (!is_short) {
			fprintf(stdout, "MTU %u - BER %g - One corrupted byte per %d bytes window\n", dsim.mtu, dsim.ber, dsim.corrupte_byte_window);
		}
	}
	/***************************/
	/*   create downloader     */
	/***************************/
	dsim.cfg = gf_cfg_init(config_file ? config_file : "dsim.cfg", NULL);
	if (cache_directory)
		gf_cfg_set_key(dsim.cfg, "General", "CacheDirectory", cache_directory);

	gf_cfg_set_key(dsim.cfg, "Downloader", "CleanCache", "no");
	gf_cfg_set_key(dsim.cfg, "HTTPProxy", "Enabled", "no");
	
	dsim.downloader = gf_dm_new(dsim.cfg);


	/***************************/
	/*   create dash client    */
	/***************************/
	dsim.dash_io.udta = &dsim;
	dsim.dash_io.dash_codec_supported = dashsim_io_codec_supported;
	dsim.dash_io.delete_cache_file = dashsim_io_delete_cache_file;
	dsim.dash_io.create = dashsim_io_create;
	dsim.dash_io.del = dashsim_io_del;
	dsim.dash_io.abort = dashsim_io_abort;
	dsim.dash_io.setup_from_url = dashsim_io_setup_from_url;
	dsim.dash_io.set_range = dashsim_io_set_range;
	dsim.dash_io.init = dashsim_io_init;
	dsim.dash_io.run = dashsim_io_run;
	dsim.dash_io.get_url = dashsim_io_get_url;
	dsim.dash_io.get_cache_name = dashsim_io_get_cache_name;
	dsim.dash_io.get_mime = dashsim_io_get_mime;
	dsim.dash_io.get_bytes_per_sec = dashsim_io_get_bytes_per_sec;
	dsim.dash_io.get_total_size = dashsim_io_get_total_size;
	dsim.dash_io.on_dash_event = dashsim_io_on_dash_event;
	dsim.dash_io.get_header_value = dashsim_io_get_header_value;
	
	
	dsim.dash = gf_dash_new(&dsim.dash_io, cache_duration, 0, keep_files, disable_switching, mode, enable_buffering, initial_timeshift_buffer_percent);

	if (!dsim.dash) {
		goto exit;
	}

	if (retry_timeout)
		gf_dash_set_min_timeout_between_404(dsim.dash, retry_timeout);

	//wait 200ms after end of a segment (ie AST + seg_duration) on a 404 to consider the segment is dead
	gf_dash_set_segment_expiration_threshold(dsim.dash, 200);

	//you may want to enable UTC compensation using	gf_dash_enable_utc_drift_compensation
	gf_dash_enable_utc_drift_compensation(dsim.dash, 1);
	//if set, the client will look for "Server-UTC: UTC_in_milliseconds" or "Date: XX:XX:..." in the HTTP header of the reply of the MPD fetch
	//to estimate the server UTC clock, rather than use his own UTC.
	e = gf_dash_open(dsim.dash, mpd_url);
	if (e) goto exit;

	//In live mode, the first segment fetched may have started some time ago - use gf_dash_get_playback_start_range to get the seek point into the segment, in MPD time 


	if (dsim.trace)
		fprintf(dsim.trace, "URL\tSize\tDuration\tCorrupted Blocks\tNb Samples\tNb Samples Full Seg Lost\tNb Samples No Index\tCorruted Samples No Index\tNb Samples With Index\tCorruted Samples With Index\n");

	last_check_time = 0;
	dsim.do_run = 1;
	while (dsim.do_run) {
		if (!dsim.connection_ack) {
			gf_sleep(10);
			continue;
		}
		if (gf_prompt_has_input()) {
			char c = gf_prompt_get_char();
			if (c=='q') {
				dsim.do_run = 0;
				break;
			}
		}
		if (! gf_dash_is_running(dsim.dash))
			break;

	}
	dsim.do_run = 0;
	gf_dash_close(dsim.dash);

	if (is_short) {
		if (show_header) {
			fprintf(stdout, "MTU\tBER\tSamples\tBytes\tDuration\tFullLostSegs\tFullLostSamples\tNoIndexSampleLost\tNoIndexSampleCorrupted\tIndexSampleLost\tIndexSampleCorrupted\n");
		}
		fprintf(stdout, "%u\t%g\t"LLU"\t"LLU"\t%g\t%u\t"LLU"\t"LLU"\t"LLU"\t"LLU"\t"LLU"", dsim.mtu, dsim.ber, dsim.nb_samples, dsim.bytes_processed, dsim.duration, dsim.nb_seg_lost, dsim.nb_samples_lost_full, dsim.nb_samples_lost_no_index, dsim.nb_samples_corr_no_index, dsim.nb_samples_lost_index, dsim.nb_samples_corr_index);
	} else {

		fprintf(stdout, "Number samples processed: "LLU" - total bytes "LLU" - bitrate %u kbps\n", dsim.nb_samples, dsim.bytes_processed, dsim.duration ? (u32) (8*dsim.bytes_processed / dsim.duration / 1000) : 0);

		fprintf(stdout, "Full segment lost: segments lost %u - samples lost: "LLU" (%g %%)\n", dsim.nb_seg_lost, dsim.nb_samples_lost_full, (Double) dsim.nb_samples_lost_full*100.0/dsim.nb_samples);


		fprintf(stdout, "No index: samples lost: "LLU" (%g %%) - corrupted "LLU" (%g %%) - total loss "LLU" (%g %%)\n", dsim.nb_samples_lost_no_index, (Double) dsim.nb_samples_lost_no_index*100.0/dsim.nb_samples, dsim.nb_samples_corr_no_index, (Double) dsim.nb_samples_corr_no_index*100.0/dsim.nb_samples, (dsim.nb_samples_lost_no_index+dsim.nb_samples_corr_no_index), (dsim.nb_samples_lost_no_index+dsim.nb_samples_corr_no_index)*100.0/dsim.nb_samples);

		fprintf(stdout, "With index: samples lost: "LLU" (%g %%) - corrupted "LLU" (%g %%) - total loss "LLU" (%g %%)\n", dsim.nb_samples_lost_index, (Double) dsim.nb_samples_lost_index*100.0/dsim.nb_samples, dsim.nb_samples_corr_index, (Double) dsim.nb_samples_corr_index*100.0/dsim.nb_samples, (dsim.nb_samples_lost_index+dsim.nb_samples_corr_index), (dsim.nb_samples_lost_index+dsim.nb_samples_corr_index)*100.0/dsim.nb_samples);
	}


exit:

	/*cleanup*/
	if (dsim.dash) gf_dash_del(dsim.dash);
	if (dsim.downloader) gf_dm_del(dsim.downloader);
	if (dsim.cfg) gf_cfg_del(dsim.cfg);
	gf_log_set_tool_level(GF_LOG_MEMORY, GF_LOG_DEBUG);
	gf_sys_close();
	if (logfile) fclose(logfile);
	if (dsim.trace) gf_fclose(dsim.trace);
	return 0;
}

