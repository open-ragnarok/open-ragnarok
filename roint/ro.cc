/* $Id$ */
#include "stdafx.h"

#include "ro.h"

#define VERSION_MAJOR 0
#define VERSION_MINOR 7
#define VERSION_S ((VERSION_MAJOR << 8) | VERSION_MINOR)

namespace RO {
	char __strVer[32];
}

unsigned short RO::Version() {
	return(VERSION_S);
}

void RO::printVersion() {
	printf("0x%04x", strVersion());
}

const char* RO::strVersion() {
	//sprintf(__strVer, "%d", VERSION_S);
	sprintf(__strVer, "%d.%d.svn", VERSION_MAJOR, VERSION_MINOR);
	return(__strVer);
}