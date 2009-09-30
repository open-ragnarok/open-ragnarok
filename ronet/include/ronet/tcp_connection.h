/* $Id$ */
/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of The Open Ragnarok Project
    Copyright 2007 - 2009 The Open Ragnarok Team
    For the latest information visit http://www.open-ragnarok.org
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------
*/
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
