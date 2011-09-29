/* $Id$
 * blob.h
 *
 *  Created on: 2009-09-09
 *      Author: Sergio Moura
 *    Revision: $Rev$
 */
#ifndef __SDLENGINE_BLOB_H
#define __SDLENGINE_BLOB_H

#include "sdle_import.h"

#include <string>
#include <cassert>
#include <stdio.h>// stderr
#include <string.h>// memcpy

namespace sdle {
/**
 * Provides instantiation and helper functions for
 * arrays of known, static sizes.
 */
template<class T, int i>
class SDLENGINE_DLLAPI Blob {
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

/**
 * Provides instantiation and helper functions for
 * arrays with mutable sizes.
 */
template<class T>
class SDLENGINE_DLLAPI base_DynamicBlob {
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

	base_DynamicBlob(const unsigned long& size = 0) {
		dataSize = size;
		buffer = 0;
		if (size) {
			buffer = new T[size];
			assert(buffer!=0 && "DynamicBlob buffer could not be created - out of memory");
		}
	}
	
	/**
	 * Changes the size of the object.
	 * As much data as possible will be retained.
	 */
	void setSize(const unsigned long& size) {
		T* auxbuf = NULL;
		if (size > 0) {
			unsigned long minsize;
			auxbuf = new T[size];
			minsize = (size > dataSize)?dataSize:size;
			if (minsize > 0)
				memcpy((char*)auxbuf, (char*)buffer, sizeof(T) * minsize);
		}
		dataSize = size;
		if (buffer)
			delete[] buffer;
		buffer = auxbuf;
	}
	
	virtual ~base_DynamicBlob() {
		if (buffer)
			delete[] buffer;
		buffer = 0;
	}

	/** Returns the amount of memory used by this object */
	unsigned long size() const {
		return (dataSize * sizeof(T) + sizeof(this));
	}

	/** Returns the size of the data (number of elements in the array */
	unsigned long blobSize() const {
		return(dataSize);
	}
	
	T* getBuffer() {
		return(buffer);
	}

	const T* getBuffer() const {
		return(buffer);
	}

	T* operator *() {
		return(buffer);
	}

	const T* operator *() const {
		return(buffer);
	}
};

class SDLENGINE_DLLAPI DynamicBlob : public base_DynamicBlob<char> {
public:
	DynamicBlob();
	DynamicBlob(const char* data, unsigned int size);
	DynamicBlob(unsigned int size);
	DynamicBlob(const DynamicBlob&);
	virtual ~DynamicBlob();
};

}

#endif /* _SDLENGINE_BLOB_H */
