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
#ifndef __RONET_CONNECTION_H
#define __RONET_CONNECTION_H

#include "ucbuffer.h"

/** \file connection.h
 * 
 * \ingroup Network
 */
namespace ronet {
	typedef enum {
		Ready = 0,
		Connecting = 1,
		Connected = 100,
		Disconnecting = 200,
		Disconnected = 201,
		Closing = 250,
		Error = 255
	} connection_state;
	
	typedef enum {
		Outgoing,
		Incoming
	} connection_direction;

	typedef enum {
		Unknown,
		TCP,
		UDP
	} connection_protocol;

#if defined(WIN32) || defined(_MSC_VER)
	class RONET_DLLAPI InitWinsock {
	public:
		InitWinsock();
		~InitWinsock();
	};
#endif

	/**
	 * Generic Connection class, implementing basic instructions
	 * 
	 * @ingroup Network
	 */
	class RONET_DLLAPI Connection {
	protected:
		/** The state of our connection */
		connection_state m_state;

		/** The direction of our connection: Incoming or Outgoing */
		connection_direction m_direction;

		connection_protocol m_protocol;

		/** Socket Identifier */	
		int m_socket;
		
		/** IP address we are connected to */
		unsigned int m_ip;
		
		/** Port we are connected to */
		unsigned short m_port;
	
	public:
		/** Data received */
		ronet::ucBuffer bufInput;

		/** Data left to send */
		ronet::ucBuffer bufOutput;
	};

}

#endif /* __RONET_CONNECTION_H */
