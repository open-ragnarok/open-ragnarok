/* $Id$ */
#ifndef __RONET_PACKET_FACTORY_H
#define __RONET_PACKET_FACTORY_H

#include "packet_vector.h"
#include "packet.h"
#include "buffer.h"
#include "connection.h"

#include <vector>
#include <map>

namespace ronet {
	class RONET_DLLAPI PacketFactory {
	private:
		PacketVector packets;
	protected:

		typedef bool (PacketFactory::*Handler)(ucBuffer& p);

		class RONET_DLLAPI Dispatcher {
		private:
			std::map<unsigned short, Handler> m_calls;
		protected:
			PacketFactory* m_factory;
		public:
			Dispatcher(PacketFactory*);

			void Register(unsigned short, Handler);
			bool Call(unsigned short, ucBuffer&);
		};

		bool Handle_ServerList(ucBuffer&);
		bool Handle_CharCreateError(ucBuffer&);
		bool Handle_CharList(ucBuffer&);
		bool Handle_LoginError(ucBuffer&);
		bool Handle_AuthFailed(ucBuffer&);
		bool Handle_CharCreated(ucBuffer&);
		bool Handle_CharPosition(ucBuffer&);
		bool Handle_MapAcctSend(ucBuffer&);
		bool Handle_MapLoginSuccess(ucBuffer&);

		void push(Packet*);

		Dispatcher m_dispatcher;

	public:
		PacketFactory();
		~PacketFactory();

		unsigned int count() const;
		Packet* pop();

		void generatePackets(ucBuffer&);
		PacketFactory& operator << (ucBuffer&);
		PacketFactory& operator << (Connection&);
	};
}

#endif /* __RONET_PACKET_FACTORY_H */
