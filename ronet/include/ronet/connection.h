/* $Id$ */
#ifndef __RONET_CONNECTION_H
#define __RONET_CONNECTION_H

#include "buffer.h"

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

		/** Socket Identifier */	
		int m_socket;
		
		/** IP address we are connected to */
		unsigned int m_ip;
		
		/** Port we are connected to */
		unsigned short m_port;
	
	public:
		ucBuffer bufInput;
		ucBuffer bufOutput;
	};
}

#endif /* __RONET_CONNECTION_H */