/* $Id$ */

#include "stdafx.h"

#include "ro/python/pygrf.h"

#include <fstream>

namespace RO {

GRF* GRF_new() {
	return(new GRF());
}

void GRF_del(GRF* grf) {
	delete(grf);
}

bool GRF_open(GRF* grf, const char* fn) {
	return(grf->open(fn));
}

void GRF_close(GRF* grf) {
	grf->close();
}

bool GRF_isOpen(GRF* grf) {
	return(grf->isOpen());
}

char __grf_getfilename[256];
const char* GRF_getFilename(const GRF* grf, unsigned int i) {
	// Python wouldn't handle deleting the string when it was no longer used, so, I'm using a static variable that
	// will forcibly be cleared out once the dll is unloaded.
	// Be sure to copy the received pointer into some other piece of memory or it will get toasted
	// with the next call of this function.
	std::string fn;
	fn = grf->getFilename(i);
	strcpy(__grf_getfilename, fn.c_str());
	return(__grf_getfilename);
}

unsigned int GRF_getCount(const GRF* grf) {
	return(grf->getCount());
}

}