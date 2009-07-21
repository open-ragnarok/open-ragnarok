/* $Id$ */
#ifndef __PYSTRINGS_H
#define __PYSTRINGS_H

#include "../ro_strings.h"

namespace RO {
extern "C" {
	ROINT_DLLAPI const char* euc2utf8(const char* euc);
	ROINT_DLLAPI const char* utf82euc(const char* utf);
}
}

#endif /* __PYSTRINGS_H */
