#ifndef __RONET_UCBUFFER_H
#define __RONET_UCBUFFER_H

#include "ronet_import.h"
#include "buffer.h"

namespace ronet {
	class RONET_DLLAPI ucBuffer : public Buffer<unsigned char> {
	public:
		ucBuffer& operator << (std::istream& s);
		ucBuffer& operator << (ucBuffer& b);
		ucBuffer& operator >> (std::ostream& s);
		ucBuffer& operator >> (char& c);
		ucBuffer& operator >> (unsigned char& c);
		ucBuffer& operator >> (short& c);
		ucBuffer& operator >> (unsigned short& c);
		ucBuffer& operator >> (int& c);
		ucBuffer& operator >> (unsigned int& c);
	};
}

#endif /* __RONET_UCBUFFER_H */
