/* $Id$ */
#include "stdafx.h"

#include "ro/python/pystrings.h"

namespace RO {
	std::string __pyret;
}

const char* RO::euc2utf8(const char* euc) {
	std::string s = euc;
	__pyret = euc2utf8(s);
	return(__pyret.c_str());
}

const char* RO::utf82euc(const char* utf) {
	std::string s = utf;
	__pyret = utf82euc(s);
	return(__pyret.c_str());
}
