#include "stdafx.h"

#include "ronet/ucbuffer.h"

ronet::ucBuffer& ronet::ucBuffer::operator << (std::istream& s) {
	write(s);
	return(*this);
}

ronet::ucBuffer& ronet::ucBuffer::operator << (ucBuffer& b) {
	write(b);
	return(*this);
}

ronet::ucBuffer& ronet::ucBuffer::operator >> (std::ostream& s) {
	s.write((char*)data + dataStart, dataSize());
	alloc(0);
	return(*this);
}

ronet::ucBuffer& ronet::ucBuffer::operator >> (unsigned char& c) {
	if (dataSize() == 0)
		return(*this);
	c = *(data+dataStart);
	dataStart++;
	return(*this);
}

ronet::ucBuffer& ronet::ucBuffer::operator >> (short& c) {
	c = 0;
	if (dataSize() < sizeof(short))
		return(*this);

	c = *(short*)(data+dataStart);

	dataStart += sizeof(short);
	return(*this);
}

ronet::ucBuffer& ronet::ucBuffer::operator >> (unsigned short& c) {
	c = 0;
	if (dataSize() < sizeof(unsigned short))
		return(*this);

	c = *(unsigned short*)(data+dataStart);

	dataStart += sizeof(unsigned short);
	return(*this);
}

ronet::ucBuffer& ronet::ucBuffer::operator >> (int& c) {
	c = 0;
	if (dataSize() < sizeof(int))
		return(*this);

	c = *(int*)(data+dataStart);
	dataStart += sizeof(int);
	return(*this);
}

ronet::ucBuffer& ronet::ucBuffer::operator >> (unsigned int& c) {
	c = 0;
	if (dataSize() < sizeof(unsigned int))
		return(*this);

	c = *(unsigned int*)(data+dataStart);
	dataStart += sizeof(unsigned int);
	return(*this);
}

ronet::ucBuffer& ronet::ucBuffer::operator >> (char& c) {
	c = 0;
	if (dataSize() < sizeof(char))
		return(*this);

	c = *(unsigned int*)(data+dataStart);
	dataStart += sizeof(char);
	return(*this);
}