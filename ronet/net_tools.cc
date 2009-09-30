/* $Id$ */
/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of The Open Ragnarok Project
    Copyright 2007 - 2009 The Open Ragnarok Team
    For the latest information visit http://www.open-ragnarok.org
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------
*/
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
