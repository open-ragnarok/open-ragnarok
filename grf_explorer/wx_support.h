#ifndef __WX_SUPPORT_H
#define __WX_SUPPORT_H

#include "wx/wx.h"

#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"rpcrt4.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"advapi32.lib")
#pragma comment(lib,"wsock32.lib")

//#define __WXVERSION "28"
#define __WXVERSION "29"

#ifdef _DEBUG
#	ifdef _UNICODE
#		pragma comment(lib,"wxmsw"__WXVERSION"ud_core.lib")
#		pragma comment(lib,"wxbase"__WXVERSION"ud.lib")
#	else
#		pragma comment(lib,"wxmsw"__WXVERSION"d_core.lib")
#		pragma comment(lib,"wxbase"__WXVERSION"d.lib")
#	endif
#else
#	ifdef _UNICODE
#		pragma comment(lib,"wxmsw"__WXVERSION"u_core.lib")
#		pragma comment(lib,"wxbase"__WXVERSION"u.lib")
#	else
#		pragma comment(lib,"wxmsw"__WXVERSION"_core.lib")
#		pragma comment(lib,"wxbase"__WXVERSION".lib")
#	endif
#endif

#endif