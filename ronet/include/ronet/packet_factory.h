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
	/**
	 * The maker of all (incoming) packets.
	 * Transforms received data input into packet classes for easy handling
	 */
	class RONET_DLLAPI PacketFactory {
	private:
		/** Packets in the queue */
		PacketVector packets;
	protected:

		/** Handler function definition */
		typedef bool (PacketFactory::*Handler)(ucBuffer& p);

		/**
		 * 
		 */
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

		//Add new packets here
		bool Handle_AttackRange(ucBuffer&);
		bool Handle_GuildMessage(ucBuffer&);
		bool Handle_DisplayStat(ucBuffer&);
		bool Handle_UpdateStatus(ucBuffer&);
		bool Handle_ServerList(ucBuffer&);
		bool Handle_CharCreateError(ucBuffer&);
		bool Handle_CharList(ucBuffer&);
		bool Handle_LoginError(ucBuffer&);
		bool Handle_AuthFailed(ucBuffer&);
		bool Handle_CharCreated(ucBuffer&);
		bool Handle_CharPosition(ucBuffer&);
		bool Handle_MapAcctSend(ucBuffer&);
		bool Handle_MapLoginSuccess(ucBuffer&);
		bool Handle_OwnSpeech(ucBuffer&);
		bool Handle_SkillList(ucBuffer&);
		bool Handle_MapMoveOk(ucBuffer&);

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
