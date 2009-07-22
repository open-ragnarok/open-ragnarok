/* $Id$ */
#ifndef __RONET_TCP_CONNECTION_H
#define __RONET_TCP_CONNECTION_H

#include "ronet_import.h"
#include "connection.h"

namespace ronet {
	class RONET_DLLAPI TcpConnection : public Connection {
	protected:
		bool RecvData();
		bool SendData();

	public:
		/** Used when creating outbound connections */
		TcpConnection();
		
		/**
		 * Create connections from an existing socket.
		 * Used mainly when a new connection arrives at the server. No new thread
		 * is started for this connection. The creator must handle it himself.
		 */
		TcpConnection(unsigned long ID, int socket, unsigned int ip, unsigned short port);
		
		/**
		 * Closes the connection upon object destruction
		 */
		virtual ~TcpConnection();

		bool Process();
		bool Close();

		bool Connect(const char* hostname, const unsigned int port);
		bool Connect(const unsigned long ip, const unsigned int port);

		bool isConnected() const;
	};
}

#endif /* __RONET_TCP_CONNECTION_H */
