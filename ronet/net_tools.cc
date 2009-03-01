/* $Id$ */
#include "stdafx.h"

#include "ronet/net_tools.h"

#include <sys/types.h>
#include <stdio.h>
#include <string.h>

int ResolveIP(const char* hostname, char* errbuf) {
	if (errbuf)
		errbuf[0] = 0;
		
	if (hostname == 0) {
		if (errbuf)
			sprintf(errbuf, "ResolveIP(): hostname == 0");
		return(0);
	}
	
	struct sockaddr_in server_sin;
	struct hostent* phostent = NULL;
	server_sin.sin_family = AF_INET;
	phostent = gethostbyname(hostname);
	if (phostent == NULL) {
		if (errbuf)
			sprintf(errbuf, "Unable to resolve hostname %s. Error: %s", hostname, strerror(errno));
		return(0);
	}
	
	memcpy ((char*)&(server_sin.sin_addr), phostent->h_addr, phostent->h_length);
	
	return(server_sin.sin_addr.s_addr);		
}
