/* $id$ */
#include "stdafx.h"

#include "ro.h"

#define VERSION_S 0x0001

unsigned short RO::Version() {
	return(VERSION_S);
}

void RO::printVersion() {
	printf("0x%04x", VERSION_S);
}
