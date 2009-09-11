/* $Id$ */
#ifndef __STDAFX_H
#define __STDAFX_H

#ifdef _MSC_VER
#	define _CRT_SECURE_NO_WARNINGS
#endif

#define RONET_INTERNAL

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

#pragma comment(lib, "wsock32.lib")

#endif /* __STDAFX_H */
