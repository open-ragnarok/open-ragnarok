/* $Id$ */
#ifndef __RONET_PACKET_FACTORY_H
#define __RONET_PACKET_FACTORY_H

#include "packet.h"
#include "buffer.h"

#include <vector>
#include <map>

namespace ronet {
	class RONET_DLLAPI PacketFactory {
	private:
		std::vector<Packet*> packets;
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

		void push(Packet*);

		Dispatcher m_dispatcher;

	public:
		PacketFactory();
		~PacketFactory();

		unsigned int count() const;
		Packet* pop();

		void generatePackets(ucBuffer&);
	};
}

#endif /* __RONET_PACKET_FACTORY_H */