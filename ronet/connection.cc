/* $Id$ */
#include "stdafx.h"
#include "ronet/connection.h"

#if defined(WIN32) || defined(_MSC_VER)

namespace ronet {

InitWinsock::InitWinsock() {
	WORD version = MAKEWORD (1,1);
	WSADATA wsadata;
	WSAStartup (version, &wsadata);
}

InitWinsock::~InitWinsock(){
	WSACleanup();
}

}

#endif

