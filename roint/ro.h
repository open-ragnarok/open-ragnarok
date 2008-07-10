/* $id$ */
#ifndef __RO_H
#define __RO_H

#define GRF_HEADER "Master of Magic"
#define SPR_HEADER "SP"
#define ACT_HEADER "AC"
#define RSM_HEADER "GRSM"
#define RSW_HEADER "GRSW"
#define GND_HEADER "GRGN"
#define GAT_HEADER "GRAT"

#include "impdecl.h"

namespace RO {
	MYLIB_DLLAPI unsigned short Version();
	MYLIB_DLLAPI void printVersion();
}

#include "ro_strings.h"
#include "constants.h"
#include "object.h"
#include "grf.h"
#include "spr.h"
#include "act.h"
#include "rsm.h"
#include "rsw.h"
#include "gnd.h"
#include "gat.h"

#endif // __RO_H
