/* $Id$ */
#include "stdafx.h"

#include "ro/python/pyrsw.h"

namespace RO {

	RSW* new_RSW() {
		return(new RSW());
	}

	void del_RSW(RSW* rsw) {
		delete(rsw);
	}

	bool read_RSW(RSW* rsw, const char* filename) {
		return(rsw->read(filename));
	}

	void dump_RSW(const RSW* rsw, const char* pfx) {
		rsw->Dump(std::cout, pfx);
	}

	bool write_RSW(const RSW*, const char*) {
		return(false);
	}

	void clear_RSW(RSW* rsw) {
		rsw->Clear();
	}

	char* get_RSW_ini(RSW* rsw) {
		return(rsw->ini_file);
	}

	char* get_RSW_gnd(RSW* rsw) {
		return(rsw->gnd_file);
	}

	char* get_RSW_gat(RSW* rsw) {
		return(rsw->gat_file);
	}

	char* get_RSW_scr(RSW* rsw) {
		return(rsw->scr_file);
	}
} // namespace RO
