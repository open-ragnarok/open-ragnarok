#ifndef __RONET_COMMON_H
#define __RONET_COMMON_H

#include "ronet_import.h"

#include <vector>
#include "packet.h"

namespace ronet {
	class RONET_DLLAPI PacketVector : public std::vector<Packet*> {
	public:
		Packet* pop();
	};
}

#endif /* __RONET_COMMON_H */
