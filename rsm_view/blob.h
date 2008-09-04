/* $Id$ */
/*
 Open Ragnarok Project 

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2 of the License.
  
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY except by those people which sell it, which
 are required to give you total support for your newly bought product;
 without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
	
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __BLOB_H
#define __BLOB_H

#include <string>
#include <cassert>

template<class T, int i>
class Blob {
protected:
	T buffer[i];
public:
	inline T& operator[](unsigned int index) {
		if (index >= i) {
			fprintf(stderr, "Bad index on Blob::[]");
			return(buffer[0]);
		}
		return (buffer[index]);
	}

	inline operator T*() {
		return (buffer);
	}
	
	inline const T& getConst(unsigned int index) const {
		if (index >= i) {
			fprintf(stderr, "Bad index on Blob::[]");
			return(buffer[0]);
		}
		return(buffer[index]);
	} 

	unsigned long size() {
		return (sizeof(this));
	}
};

template<class T>
class DynamicBlob {
protected:
	unsigned long dataSize;
	T *buffer;
public:
	inline T& operator[](int index) {
		unsigned long d = index;
		if (d >= dataSize) {
			fprintf(stderr, "Bad index on DynamicBlob::[]");
			return(buffer[0]);
		}
		return (buffer[index]);
	}

	inline const T& operator[](int index) const {
		unsigned long d = index;
		if (d >= dataSize) {
			fprintf(stderr, "Bad index on DynamicBlob::[]");
			return(buffer[0]);
		}
		return (buffer[index]);
	}

	inline operator T*() {
		return (buffer);
	}

	inline operator const T*() const {
		return(buffer);
	}

	DynamicBlob(const unsigned long& size = 0) {
		dataSize = size;
		buffer = 0;
		if (size) {
			buffer = new T[size];
			assert(buffer!=0 && "DynamicBlob buffer could not be created - out of memory");
		}
	}
	
	/** Changes the size of the object */
	void setSize(const unsigned long& size) {
		dataSize = size;
		if (buffer) {
			delete[] buffer;
			buffer = NULL;
		}
		if (size) {
			buffer = new T[size];
			assert(buffer!=0 && "DynamicBlob buffer could not be created - out of memory");
		}
	}
	
	~DynamicBlob() {
		if (buffer)
			delete[] buffer;
		buffer = 0;
	}

	/** Returns the amount of memory used by this object */
	unsigned long size() const {
		return (dataSize * sizeof(T) + sizeof(this));
	}

	unsigned long blobSize() const {
		return(dataSize);
	}
	
	T* getBuffer() {
		return(buffer);
	}

	const T* getBuffer() const {
		return(buffer);
	}
};

#endif /* __BLOB_H */
