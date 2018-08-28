/*
 *			GPAC - Multimedia Framework C SDK
 *
 *			Authors: Jean Le Feuvre
 *			Copyright (c) Telecom ParisTech 2018
 *					All rights reserved
 *
 *  This file is part of GPAC / MPEG-H 3D Audio IIS decoder module
 *
 *  GPAC is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  GPAC is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include <gpac/setup.h>

#include <gpac/modules/codec.h>
#include <gpac/constants.h>
#include <gpac/avparse.h>

#include "../../extra_lib/include/mpegh_iis/aacdecoder_lib.h"
#include "../../extra_lib/include/mpegh_iis/coresup.h"

#define IN_BUF_SIZE   ( 8192*256 )
#define OUT_BUF_SIZE  ( 1024*32*2 )

#define MHAS_FRAME_INDEX_FIRST 0
#define MHAS_FRAME_INDEX_LAST 2556


typedef struct
{
	HANDLE_AACDECODER codec;

	u32 sample_rate, out_size, num_samples;
	u8 num_channels;
	/*no support for scalability in FAAD yet*/
	u16 ES_ID;
	Bool signal_mc;
	Bool is_sbr;

	char ch_reorder[16];
	GF_ESD *esd;
} MPEGHIISDec;


#define MPEGHIISCTX() MPEGHIISDec *ctx = (MPEGHIISDec *) ifcg->privateStack

static GF_Err MPEGHIIS_AttachStream(GF_BaseDecoder *ifcg, GF_ESD *esd)
{
	MPEGHIISCTX();

	if (ctx->ES_ID && ctx->ES_ID!=esd->ESID) return GF_NOT_SUPPORTED;

	if (!ctx->esd) {
		ctx->esd = esd;
		GF_LOG(GF_LOG_DEBUG, GF_LOG_CODEC, ("[MPEGHIISDec] Attaching stream %d\n", esd->ESID));
	}

	if (ctx->codec) aacDecoder_Close(ctx->codec);
	ctx->codec = aacDecoder_Open(TT_MHAS_PACKETIZED, 1);
	if (!ctx->codec) {
		GF_LOG(GF_LOG_ERROR, GF_LOG_CODEC, ("[MPEGHIISDec] Error initializing decoder\n"));
		return GF_IO_ERR;
	}
	aacDecoder_SetParam(ctx->codec, AAC_TARGET_LAYOUT_CICP, 2);
//	aacDecoder_SetParam(ctx->codec, AAC_TARGET_LAYOUT_CICP, -1);
	
	if (esd->decoderConfig->decoderSpecificInfo && esd->decoderConfig->decoderSpecificInfo->dataLength) {
		//push config
	 	aacDecoder_ConfigRaw(ctx->codec, (UCHAR**)&esd->decoderConfig->decoderSpecificInfo->data, esd->decoderConfig->decoderSpecificInfo->dataLength);
	}


	ctx->num_samples = 1024;
	ctx->out_size = 2 * ctx->num_samples * ctx->num_channels;
	ctx->ES_ID = esd->ESID;
	ctx->signal_mc = ctx->num_channels>2 ? GF_TRUE : GF_FALSE;
	return GF_OK;
}

static GF_Err MPEGHIIS_DetachStream(GF_BaseDecoder *ifcg, u16 ES_ID)
{
	MPEGHIISCTX();
	if (ES_ID != ctx->ES_ID) return GF_BAD_PARAM;
	if (ctx->codec) aacDecoder_Close(ctx->codec);
	ctx->codec = NULL;
	ctx->ES_ID = 0;
	ctx->sample_rate = ctx->out_size = ctx->num_samples = 0;
	ctx->num_channels = 0;
	return GF_OK;
}
static GF_Err MPEGHIIS_GetCapabilities(GF_BaseDecoder *ifcg, GF_CodecCapability *capability)
{
	MPEGHIISCTX();
	switch (capability->CapCode) {
	/*not tested yet*/
	case GF_CODEC_RESILIENT:
		capability->cap.valueInt = 1;
		break;
	case GF_CODEC_OUTPUT_SIZE:
		capability->cap.valueInt = ctx->out_size;
		break;
	case GF_CODEC_SAMPLERATE:
		capability->cap.valueInt = ctx->sample_rate;
		break;
	case GF_CODEC_NB_CHAN:
		capability->cap.valueInt = ctx->num_channels;
		break;
	case GF_CODEC_BITS_PER_SAMPLE:
		capability->cap.valueInt = 16;
		break;
	case GF_CODEC_BUFFER_MIN:
		capability->cap.valueInt = 4;
		break;
	case GF_CODEC_BUFFER_MAX:
		capability->cap.valueInt = 12;
		break;
	/*by default AAC access unit lasts num_samples (timescale being sampleRate)*/
	case GF_CODEC_CU_DURATION:
		capability->cap.valueInt = ctx->num_samples;
		break;
	/*to refine, it seems that 4 bytes padding is not enough on all streams ?*/
	case GF_CODEC_PADDING_BYTES:
		capability->cap.valueInt = 8;
		break;
	case GF_CODEC_CHANNEL_CONFIG:
		capability->cap.valueInt = 0;
		capability->cap.valueInt |= GF_AUDIO_CH_FRONT_LEFT | GF_AUDIO_CH_FRONT_RIGHT;
		break;
	default:
		capability->cap.valueInt = 0;
		break;
	}
	return GF_OK;
}
static GF_Err MPEGHIIS_SetCapabilities(GF_BaseDecoder *ifcg, GF_CodecCapability capability)
{
	/*return unsupported to avoid confusion by the player (like SR changing ...) */
	return GF_NOT_SUPPORTED;
}

static GF_Err MPEGHIIS_ProcessData(GF_MediaDecoder *ifcg,
                               char *inBuffer, u32 inBufferLength,
                               u16 ES_ID, u32 *CTS,
                               char *outBuffer, u32 *outBufferLength,
                               u8 PaddingBits, u32 mmlevel)
{
	CStreamInfo * p_si;
	MPEGHIISCTX();

	/*if late or seeking don't decode*/
	switch (mmlevel) {
	case GF_CODEC_LEVEL_SEEK:
//	case GF_CODEC_LEVEL_DROP:
		*outBufferLength = 0;
		return GF_OK;
	default:
		break;
	}

	if (ctx->out_size > *outBufferLength) {
		*outBufferLength = ctx->out_size;
		return GF_BUFFER_TOO_SMALL;
	}
	UCHAR* iis_inBuffer[2] = {0};
	UINT  bytesValid[2] = {0};
	UINT  bytesRead[2] = {0};
	iis_inBuffer[0] = (UCHAR *) inBuffer;
	bytesRead[0] = inBufferLength;
	bytesValid[0] = inBufferLength;

	GF_LOG(GF_LOG_DEBUG, GF_LOG_CODEC, ("[MPEGHIISDec] Filling buffers\n"));
	AAC_DECODER_ERROR err = aacDecoder_Fill(ctx->codec, iis_inBuffer, bytesRead, bytesValid);
	if (err || bytesValid[0]) {
		GF_LOG(GF_LOG_ERROR, GF_LOG_CODEC, ("[MPEGHIISDec] filling buffer failed with code 0x%x\n", err));
		return GF_IO_ERR;
	}
	GF_LOG(GF_LOG_DEBUG, GF_LOG_CODEC, ("[MPEGHIISDec] Decoding AU\n"));

	Bool needs_reconfig = GF_FALSE;
	INT_PCM pcm_buffer[OUT_BUF_SIZE];
	Bool isDone = GF_FALSE;
	while(!isDone) {
		err = aacDecoder_DecodeFrame (ctx->codec, pcm_buffer, OUT_BUF_SIZE, 0);
		switch (err) {
		case AAC_DEC_OK:
			GF_LOG(GF_LOG_DEBUG, GF_LOG_CODEC, ("[MPEGHIISDec] decoding OK\n"));
			isDone = GF_TRUE;
		case AAC_DEC_INTERMEDIATE_OK:
			p_si = aacDecoder_GetStreamInfo(ctx->codec);
			//copy to output
			if (p_si->numChannels != ctx->num_channels) {
				needs_reconfig = GF_TRUE;
				ctx->num_channels = p_si->numChannels;
			}
			if (p_si->sampleRate != ctx->sample_rate) {
				needs_reconfig = GF_TRUE;
				ctx->sample_rate = p_si->sampleRate;
			}
			if (p_si->mpeghAUSize && (p_si->mpeghAUSize != ctx->num_samples)) {
				needs_reconfig = GF_TRUE;
				ctx->num_samples = p_si->mpeghAUSize;
			}
			else if (p_si->frameSize && (p_si->frameSize != ctx->num_samples)) {
				needs_reconfig = GF_TRUE;
				ctx->num_samples = p_si->frameSize;
			}

			if (needs_reconfig) {
				ctx->out_size = ctx->num_samples * ctx->num_channels * 2;
			}
			if (ctx->out_size > (*outBufferLength) ) {
				*outBufferLength = ctx->out_size;
				return GF_BUFFER_TOO_SMALL;
			}
			if (isDone) {
				assert(outBuffer);
				memcpy(outBuffer, pcm_buffer, ctx->out_size);
				*outBufferLength = ctx->out_size;
				return GF_OK;
			}
			break;
			//TODO: add error handling here
		default:
			GF_LOG(GF_LOG_ERROR, GF_LOG_CODEC, ("[MPEGHIISDec] decoding failed with code 0x%x\n", err));
			return GF_NON_COMPLIANT_BITSTREAM;
		}
	}


	GF_LOG(GF_LOG_DEBUG, GF_LOG_CODEC, ("[MPEGHIISDec] AU decoded\n"));


	return GF_OK;
}

static const char *MPEGHIIS_GetCodecName(GF_BaseDecoder *ifcg)
{
	return "MPEG-H 3D Audio by IIS";
}

static u32 MPEGHIIS_CanHandleStream(GF_BaseDecoder *dec, u32 StreamType, GF_ESD *esd, u8 PL)
{
	/*audio decs*/
	if (StreamType != GF_STREAM_AUDIO) return GF_CODEC_NOT_SUPPORTED;
	/*media type query*/
	if (!esd) return GF_CODEC_STREAM_TYPE_SUPPORTED;

	switch (esd->decoderConfig->objectTypeIndication) {
	case GPAC_OTI_AUDIO_MPEGH:
		return GF_CODEC_SUPPORTED;
	default:
		return GF_CODEC_NOT_SUPPORTED;
	}
	return GF_CODEC_SUPPORTED;
}

GF_BaseDecoder *NewMPEGHIISDec()
{
	GF_MediaDecoder *ifce;
	MPEGHIISDec *dec;

	GF_SAFEALLOC(ifce, GF_MediaDecoder);
	if (!ifce) return NULL;
	GF_SAFEALLOC(dec, MPEGHIISDec);
	if (!dec) {
		gf_free(dec);
		return NULL;
	}
	GF_REGISTER_MODULE_INTERFACE(ifce, GF_MEDIA_DECODER_INTERFACE, "MPEG-H 3D Audio Decoder by IIS", "gpac distribution")

	ifce->privateStack = dec;

	/*setup our own interface*/
	ifce->AttachStream = MPEGHIIS_AttachStream;
	ifce->DetachStream = MPEGHIIS_DetachStream;
	ifce->GetCapabilities = MPEGHIIS_GetCapabilities;
	ifce->SetCapabilities = MPEGHIIS_SetCapabilities;
	ifce->ProcessData = MPEGHIIS_ProcessData;
	ifce->CanHandleStream = MPEGHIIS_CanHandleStream;
	ifce->GetName = MPEGHIIS_GetCodecName;
	return (GF_BaseDecoder *) ifce;
}

void DeleteMPEGHIISDec(GF_BaseDecoder *ifcg)
{
	MPEGHIISCTX();
	if (ctx->codec) aacDecoder_Close(ctx->codec);

	gf_free(ctx);
	gf_free(ifcg);
}

#ifndef DONT_USE_TERMINAL_MODULE_API

GPAC_MODULE_EXPORT
const u32 *QueryInterfaces()
{
	static u32 si [] = {
		GF_MEDIA_DECODER_INTERFACE,
		0
	};

	return si;
}
GPAC_MODULE_EXPORT
GF_BaseInterface *LoadInterface(u32 InterfaceType)
{
	if (InterfaceType == GF_MEDIA_DECODER_INTERFACE) return (GF_BaseInterface *)NewMPEGHIISDec();
	return NULL;
}

GPAC_MODULE_EXPORT
void ShutdownInterface(GF_BaseInterface *ifce)
{
	switch (ifce->InterfaceType) {
	case GF_MEDIA_DECODER_INTERFACE:
		DeleteMPEGHIISDec((GF_BaseDecoder *)ifce);
		break;
	}
}

GPAC_MODULE_STATIC_DECLARATION( mpegh_iis_dec )

#endif
