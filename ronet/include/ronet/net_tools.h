/* $Id$ */

/** \file net_tools.h
 * Non-object functions related to network
 * \ingroup Network
 */
#ifndef __NET_TOOLS_H
#define __NET_TOOLS_H

#include "ronet_import.h"

/** Translates a name to an IP address
 * @ingroup Network
 */
int RONET_DLLAPI ResolveIP(const char* hostname, char* errbuf = 0);

#ifdef WIN32
#	define ThreadReturnType void
#else
#	define ThreadReturnType void*
#endif

#endif /* __NET_TOOLS_H */
