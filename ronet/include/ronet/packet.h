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
#ifndef __RONET_PACKET_H
#define __RONET_PACKET_H

#include "ucbuffer.h"
#include "connection.h"
#include "ronet/blob.h"
#include "structures.h"
#include "ronet/packet_ids.h"

namespace ronet {

template<class T>
class DynamicBlob {
protected:
	unsigned long dataSize;
	T *buffer;
public:
	inline T& operator[](int index) {
		unsigned long d = index;
		if (d >= dataSize) {
			fprintf(stderr, "Bad index on Blob::[]");
			return(buffer[0]);
		}
		return (buffer[index]);
	}

	inline const T& operator[](int index) const {
		unsigned long d = index;
		if (d >= dataSize) {
			fprintf(stderr, "Bad index on Blob::[]");
			return(buffer[0]);
		}
		return (buffer[index]);
	}

	inline operator T*() {
		return (buffer);
	}

	DynamicBlob(const unsigned long& size = 0) {
		dataSize = size;
		buffer = 0;
		if (size) {
			buffer = new T[size];
			assert(buffer!=0 && "DynamicBlob buffer could not be created - out of memory");
		}
	}

	DynamicBlob(const DynamicBlob<T>& o) {
		buffer = NULL;
		setSize(o.dataSize);
		if (dataSize > 0)
			memcpy(buffer, o.buffer, dataSize);
	}
	
	void setSize(const unsigned long& size) {
		T* newdata = NULL;
		if (size) {
			newdata = new T[size];
			assert(newdata !=0 && "DynamicBlob buffer could not be created - out of memory");
		}
		if (buffer) {
			if (size > 0)
				memcpy(newdata, buffer, (dataSize>size)?size:dataSize);
			delete[] buffer;
		}
		dataSize = size;
		buffer = newdata;
	}
	
	inline T& get(unsigned long index) {
		if (index >= dataSize) {
			fprintf(stderr, "Bad index on Blob::[]");
			return(buffer[0]);
		}
		return(buffer[index]);
	}

	inline const T& get(unsigned long index) const {
		if (index >= dataSize) {
			fprintf(stderr, "Bad index on Blob::[]");
			return(buffer[0]);
		}
		return(buffer[index]);
	}
		
	virtual ~DynamicBlob() {
		if (buffer)
			delete[] buffer;
		buffer = 0;
	}

	unsigned long size() const {
		return (dataSize * sizeof(T) + sizeof(this));
	}

	unsigned long blobSize() const {
		return(dataSize);
	}
	

	const T* getBuffer() const {
		return(buffer);
	}

	T* getBuffer() {
		return(buffer);
	}

	void clear() {
		if (buffer)
			delete[] buffer;
		buffer = NULL;
		dataSize = 0;
	}

	DynamicBlob<T>& operator = (const DynamicBlob<T>& o) {
		clear();
		this->setSize(o.dataSize);
		if (dataSize > 0)
			memcpy(buffer, o.buffer, dataSize);
		return(*this);
	}

	void write(std::ostream& o) const {
		o.write((const char*)buffer, dataSize);
	}
};


	class RONET_DLLAPI Packet : public DynamicBlob<unsigned char> {
	protected:
		unsigned short pktID;

		virtual bool PrepareData();

		bool CheckID(const ucBuffer&) const;
	public:
		Packet();
		Packet(unsigned short pktID);
		virtual ~Packet();

		Packet& operator >> (ucBuffer&);

		virtual void Dump(); // For debugging purposes
		void setSize(const unsigned long& size);
		virtual bool Decode(ucBuffer&);
		unsigned short getID() const;
	};


	/*
	=====================================
	== DECLARATION FOR GENERIC PACKETS ==
	=====================================
	These declarations are good for inbound and outbound data.
	*/

/** This is for all packets that are like <packet_id>.short */
#define RONET_GENERIC_DECL(name) \
	class pkt ##name : public Packet { \
	protected: \
		virtual bool PrepareData(); \
	public: \
		pkt ##name (); \
		virtual bool Decode(ucBuffer&); \
	};

#define RONET_GENERIC_IMPL(name) \
pkt ##name ::pkt ##name () : Packet(pkt ##name ##ID) { setSize(2); } \
bool pkt ##name ::PrepareData() { return(true); } \
bool pkt ##name ::Decode(ucBuffer& buf) { \
	if (!CheckID(buf)) return(false); \
	buf.ignore(2); return(true); \
}

/** This is for all packets that are like <packet_id>.short <id>.uint */
#define RONET_GENERIC_ID_DECL(name) \
	class pkt ##name : public Packet { \
	protected: \
		unsigned int id; \
		virtual bool PrepareData(); \
	public: \
		pkt ##name (); \
		pkt ##name (unsigned int); \
		unsigned int getID() const; \
		virtual bool Decode(ucBuffer&); \
	};

#define RONET_GENERIC_ID_IMPL(name) \
pkt ##name ::pkt ##name () : Packet(pkt ##name ##ID) { this->id = id; setSize(6); } \
pkt ##name ::pkt ##name (unsigned int id) : Packet(pkt ##name ##ID) { this->id = id; setSize(6); } \
bool pkt ##name ::PrepareData() { unsigned char* ptr = buffer; ptr += sizeof(short); memcpy(ptr, (unsigned char*)&id, sizeof(int)); ptr += sizeof(int); return(true); } \
unsigned int pkt ##name ::getID() const { return(id); } \
bool pkt ##name ::Decode(ucBuffer& buf) { \
	if (!CheckID(buf)) return(false); \
	if (buf.dataSize() < 6) return(false); \
	buf.ignore(2); buf >> id; return(true); \
}

/**
 * This is for all packets that are like
 * <id> <packet_id>.short <param1>.type1
 */
#define RONET_GENERIC_1PARAM_DECL(name, type1) \
	class pkt ##name : public Packet { \
	protected: \
		type1 m_param1; \
		virtual bool PrepareData(); \
	public: \
		pkt ##name (); \
		pkt ##name (type1); \
		type1 getParam() const; \
		void setParam(type1); \
		virtual bool Decode(ucBuffer&); \
	};

#define RONET_GENERIC_1PARAM_IMPL(name, type1) \
pkt ##name ::pkt ##name () : Packet(pkt ##name ##ID) { m_param1 = 0; setSize(2 + sizeof(type1)); } \
pkt ##name ::pkt ##name (type1 v) : Packet(pkt ##name ##ID) { m_param1 = v; setSize(2 + sizeof(type1)); } \
bool pkt ##name ::PrepareData() { unsigned char* ptr = buffer; ptr += sizeof(short); memcpy(ptr, (unsigned char*)&m_param1, sizeof(type1)); ptr += sizeof(type1); return(true); } \
type1 pkt ##name ::getParam() const { return(m_param1); } \
void pkt ##name ::setParam(type1 v) { m_param1 = v; } \
bool pkt ##name ::Decode(ucBuffer& buf) { \
	if (!CheckID(buf)) return(false); \
	if (buf.dataSize() < (2 + sizeof(type1))) return(false); \
	buf.ignore(2); \
	buf >> m_param1; \
	return(true); \
}

/**
 * This is for all packets that are like
 * <id> <packet_id>.short <param1>.type1 <param2>.type2
 */
#define RONET_GENERIC_2PARAM_DECL(name, type1, type2) \
	class pkt ##name : public Packet { \
	protected: \
		type1 m_param1; \
		type2 m_param2; \
		virtual bool PrepareData(); \
	public: \
		pkt ##name (); \
		pkt ##name (type1, type2); \
		type1 getParam1() const; \
		void setParam1(type1); \
		type2 getParam2() const; \
		void setParam2(type2); \
		virtual bool Decode(ucBuffer&); \
	};

#define RONET_GENERIC_2PARAM_IMPL(name, type1, type2) \
pkt ##name ::pkt ##name () : Packet(pkt ##name ##ID) { m_param1 = m_param2 = 0; setSize(2 + sizeof(type1) + sizeof(type2)); } \
pkt ##name ::pkt ##name (type1 v1, type2 v2) : Packet(pkt ##name ##ID) { m_param1 = v1; m_param2 = v2; setSize(2 + sizeof(type1) + sizeof(type2)); } \
bool pkt ##name ::PrepareData() { unsigned char* ptr = buffer; ptr += sizeof(short); memcpy(ptr, (unsigned char*)&m_param1, sizeof(type1)); ptr += sizeof(type1); memcpy(ptr, (unsigned char*)&m_param2, sizeof(type2)); ptr += sizeof(type2); return(true); } \
type1 pkt ##name ::getParam1() const { return(m_param1); } \
void pkt ##name ::setParam1(type1 v) { m_param1 = v; } \
type2 pkt ##name ::getParam2() const { return(m_param2); } \
void pkt ##name ::setParam2(type2 v) { m_param2 = v; } \
bool pkt ##name ::Decode(ucBuffer& buf) { \
	if (!CheckID(buf)) return(false); \
	if (buf.dataSize() < (2 + sizeof(type1) + sizeof(type2))) return(false); \
	buf.ignore(2); \
	buf >> m_param1; \
	buf >> m_param2; \
	return(true); \
}

/**
 * This is for all packets that are like
 * <id> <packet_id>.short <param1>.type1 <param2>.type2 <param3>.type3
 */
#define RONET_GENERIC_3PARAM_DECL(name, type1, type2, type3) \
	class pkt ##name : public Packet { \
	protected: \
		type1 m_param1; \
		type2 m_param2; \
		type3 m_param3; \
		virtual bool PrepareData(); \
	public: \
		pkt ##name (); \
		pkt ##name (type1, type2, type3); \
		type1 getParam1() const; \
		void setParam1(type1); \
		type2 getParam2() const; \
		void setParam2(type2); \
		type3 getParam3() const; \
		void setParam3(type3); \
		virtual bool Decode(ucBuffer&); \
	};

#define RONET_GENERIC_3PARAM_IMPL(name, type1, type2, type3) \
pkt ##name ::pkt ##name () : Packet(pkt ##name ##ID) { m_param1 = m_param2 = m_param3 = 0; setSize(2 + sizeof(type1) + sizeof(type2) + sizeof(type3)); } \
pkt ##name ::pkt ##name (type1 v1, type2 v2, type3 v3) : Packet(pkt ##name ##ID) { m_param1 = v1; m_param2 = v2; m_param3 = v3; setSize(2 + sizeof(type1) + sizeof(type2) + sizeof(type3)); } \
bool pkt ##name ::PrepareData() { unsigned char* ptr = buffer; ptr += sizeof(short); memcpy(ptr, (unsigned char*)&m_param1, sizeof(type1)); ptr += sizeof(type1); memcpy(ptr, (unsigned char*)&m_param2, sizeof(type2)); ptr += sizeof(type2); memcpy(ptr, (unsigned char*)&m_param3, sizeof(type3)); ptr += sizeof(type3); return(true); } \
type1 pkt ##name ::getParam1() const { return(m_param1); } \
type2 pkt ##name ::getParam2() const { return(m_param2); } \
type3 pkt ##name ::getParam3() const { return(m_param3); } \
void pkt ##name ::setParam1(type1 v) { m_param1 = v; } \
void pkt ##name ::setParam2(type2 v) { m_param2 = v; } \
void pkt ##name ::setParam3(type3 v) { m_param3 = v; } \
bool pkt ##name ::Decode(ucBuffer& buf) { \
	if (!CheckID(buf)) return(false); \
	if (buf.dataSize() < (2 + sizeof(type1) + sizeof(type2) + sizeof(type3))) return(false); \
	buf.ignore(2); \
	buf >> m_param1; \
	buf >> m_param2; \
	buf >> m_param3; \
	return(true); \
}

/** This is for all packets that are like <packet_id>.short <id>.uint <trailing>.byte */
#define RONET_GENERIC_TRAILING_DECL(name) \
	class pkt ##name : public Packet { \
	protected: \
		unsigned int id; \
		unsigned char trail; \
		virtual bool PrepareData(); \
	public: \
		pkt ##name (); \
		pkt ##name (unsigned int, unsigned char); \
		unsigned int getID() const; \
		unsigned char getTrail() const; \
		virtual bool Decode(ucBuffer&); \
	}

#define RONET_GENERIC_TRAILING_IMPL(name) \
pkt ##name ::pkt ##name (unsigned int id, unsigned char trail) : Packet(pkt ##name ##ID) { this->id = id; this->trail = trail; setSize(7); } \
bool pkt ##name ::PrepareData() { unsigned char* ptr = buffer; ptr += sizeof(short); memcpy(ptr, (unsigned char*)&id, sizeof(int)); ptr += sizeof(int); *ptr = trail; return(true); } \
unsigned int pkt ##name ::getID() const { return(id); } \
unsigned char pkt ##name ::getTrail() const { return(trail); } \
bool pkt ##name ::Decode(ucBuffer& buf) { \
	if (!CheckID(buf)) return(false); \
	if (buf.dataSize() < 7) return(false); \
	buf.ignore(2); buf >> id; buf >>trail; return(true); \
}


}

#endif /* __RONET_PACKET_H */
