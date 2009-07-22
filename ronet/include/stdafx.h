/* $Id$ */
#ifndef __STDAFX_H
#define __STDAFX_H

#ifdef WIN32
#	define _CRT_SECURE_NO_WARNINGS
#endif

#define _RONET_DLLAPI_
#define _RONET_NOAUTOLIB_

#include "ronet/packet_vector.h"

#include "ronet/ronet_import.h"
#include "ronet/ronet.h"
#include "ronet/buffer.h"

#ifdef WIN32
	#include <winsock.h>
	#include <windows.h>
	#include <process.h>
#else
	#include <unistd.h>
	#include <netdb.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <errno.h>
	#include <fcntl.h>
	#define INVALID_SOCKET -1
	#define SOCKET_ERROR -1
#endif

#endif /* __STDAFX_H */
