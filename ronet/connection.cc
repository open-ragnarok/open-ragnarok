/* $Id$ */
#include "stdafx.h"
#include "ronet/connection.h"

#ifdef WIN32
namespace ronet {
	class InitWinsock {
	public:
		InitWinsock() {
			WORD version = MAKEWORD (1,1);
			WSADATA wsadata;
			WSAStartup (version, &wsadata);
		}
		~InitWinsock(){
			WSACleanup();
		}
	};

	InitWinsock winsock;
}
#endif

