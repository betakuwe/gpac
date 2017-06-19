/*
 *			GPAC - Multimedia Framework C SDK
 *
 *			Authors: Jean Le Feuvre
 *			Copyright (c) Telecom ParisTech 2011-2012
 *			All rights reserved
 *
 *  This file is part of GPAC / Sampe On-Scvreen Display sub-project
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

#include <gpac/modules/term_ext.h>
#include <gpac/internal/terminal_dev.h>
#include <gpac/internal/compositor_dev.h>
#include <gpac/nodes_mpeg4.h>
#include <gpac/network.h>

typedef struct
{
	GF_Terminal *term;
	GF_Socket *sock;
} GF_OPENGAZER;

static Bool opengazer_process(GF_TermExt *termext, u32 action, void *param)
{
	const char *sOpt, *IP_ADRESS;
	int SERVER_PORT;
	GF_OPENGAZER *opengazer = termext->udta;
	char message[1024];
	GF_Err e;
	u32 bytes;

	switch (action) {
	case GF_TERM_EXT_START:
		opengazer->sock = gf_sk_new(GF_SOCK_TYPE_UDP);
		opengazer->term = (GF_Terminal *) param;

		sOpt = gf_modules_get_option((GF_BaseInterface*)termext, "Opengazer", "Enabled");
		printf("opt = %s\n",sOpt);
		if (!sOpt || strcmp(sOpt, "yes")) return 0;
		sOpt = gf_cfg_get_key(opengazer->term->compositor->user->config, "Opengazer", "IP_ADRESS");
		if (sOpt) IP_ADRESS = sOpt;
		else IP_ADRESS = "127.0.0.1";
		sOpt = gf_cfg_get_key(opengazer->term->compositor->user->config, "Opengazer", "SERVER_PORT");
		if (sOpt) SERVER_PORT = atoi(sOpt);
		else SERVER_PORT = 20320;

		termext->caps |= GF_TERM_EXTENSION_NOT_THREADED;

		if (opengazer->sock < 0)   {
			perror("Opening datagram socket");
			exit(1);
		}
		  
		e = gf_sk_bind(opengazer->sock, IP_ADRESS, SERVER_PORT, NULL, 0, 0);
		if (e != GF_OK) {
			if (opengazer->sock) gf_sk_del(opengazer->sock);
			opengazer->sock = NULL;
		}
		  
		return 1;

	case GF_TERM_EXT_STOP:
		if (opengazer->sock) gf_sk_del(opengazer->sock);
		break;

	case GF_TERM_EXT_PROCESS:

		if (gf_sk_receive(opengazer->sock, message, 1024, 0, &bytes)==GF_OK) {
			if(bytes>0){
				message[bytes] = '\0';
				sscanf(message, "%*s %d %*s %d", &opengazer->term->compositor->gazeX, &opengazer->term->compositor->gazeY);
			}
  		}
		break;
	}
	return 0;
}


GF_TermExt *opengazer_new()
{
	GF_TermExt *dr;
	GF_OPENGAZER *opengazer;
	dr = (GF_TermExt*)gf_malloc(sizeof(GF_TermExt));
	memset(dr, 0, sizeof(GF_TermExt));
	GF_REGISTER_MODULE_INTERFACE(dr, GF_TERM_EXT_INTERFACE, "GPAC OnScreen Display", "gpac distribution");

	GF_SAFEALLOC(opengazer, GF_OPENGAZER);
	dr->process = opengazer_process;
	dr->udta = opengazer;
	return dr;
}


void opengazer_delete(GF_BaseInterface *ifce)
{
	GF_TermExt *dr = (GF_TermExt *) ifce;
	GF_OPENGAZER *opengazer = dr->udta;
	gf_free(opengazer);
	gf_free(dr);
}

GPAC_MODULE_EXPORT
const u32 *QueryInterfaces()
{
	static u32 si [] = {
		GF_TERM_EXT_INTERFACE,
		0
	};
	return si;
}

GPAC_MODULE_EXPORT
GF_BaseInterface *LoadInterface(u32 InterfaceType)
{
	if (InterfaceType == GF_TERM_EXT_INTERFACE) return (GF_BaseInterface *)opengazer_new();
	return NULL;
}

GPAC_MODULE_EXPORT
void ShutdownInterface(GF_BaseInterface *ifce)
{
	switch (ifce->InterfaceType) {
	case GF_TERM_EXT_INTERFACE:
		opengazer_delete(ifce);
		break;
	}
}

GPAC_MODULE_STATIC_DECLARATION( opengazer )
