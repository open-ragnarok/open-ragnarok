/* $Id$ */
#include "stdafx.h"

#include "ro/python/pyact.h"
#include <fstream>

namespace RO {

ACT* ACT_new() {
	return(new ACT());
}

void ACT_del(ACT* act) {
	delete(act);
}

bool ACT_read(ACT* act, const char* fn) {
	return(act->read(fn));
}

void ACT_dump(const ACT* act, const char* pfx) {
	act->Dump(std::cout, pfx);
}

unsigned int ACT_count(const ACT* act) {
	return(act->count());
}

}
