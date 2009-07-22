/* $Id$ */
#include "stdafx.h"

#include "ronet/packet.h"

ronet::Packet::Packet() : DynamicBlob<unsigned char>() {
	id = 0;
}

ronet::Packet::Packet(unsigned short pktid) : DynamicBlob<unsigned char>() {
	id = pktid;
}

ronet::Packet::~Packet() {
}

void ronet::Packet::setSize(const unsigned long& size) {
	DynamicBlob<unsigned char>::setSize(size);
	memset(buffer, 0, dataSize);
}

bool ronet::Packet::PrepareData() {
	return(false);
}

ronet::Packet& ronet::Packet::operator >> (ronet::ucBuffer& b) {
	if (!PrepareData())
		return(*this);

	if (blobSize() > 0)
		b.write(buffer, blobSize());
	return(*this);
}

void ronet::Packet::Dump() {
	if (!PrepareData()) {
		std::cerr << "Packet::Dump(): Error preparing data" << std::endl;
		return;
	}

	unsigned int i;
	for (i = 0; i < dataSize; i++)
		printf("%02x", buffer[i]);
}

bool ronet::Packet::Decode(ucBuffer&) {
	return(false);
}

unsigned short ronet::Packet::getID() const {
	return(id);
}

bool ronet::Packet::CheckID(const ucBuffer& buf) const {
	if (buf.dataSize() < 2)
		return(false);

	unsigned short buf_id;
	buf.peek((unsigned char*)&buf_id, 2);
	if (buf_id != id) {
		fprintf(stderr, "Wrong packet id! (%04x != %04x)\n", id, buf_id);
		return(false);
	}

	return(true);
}
