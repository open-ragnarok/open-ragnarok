/* $Id$ */
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

	inline const T& operator[](unsigned int index) const {
		if (index >= i) {
			fprintf(stderr, "Bad index on Blob::[]");
			return(buffer[0]);
		}
		return (buffer[index]);
	}


	inline operator T*() {
		return (buffer);
	}

	inline T& get(unsigned int index) {
		if (index >= i) {
			fprintf(stderr, "Bad index on Blob::[]");
			return(buffer[0]);
		}
		return(buffer[index]);
	} 
	
	inline const T& get(unsigned int index) const {
		if (index >= i) {
			fprintf(stderr, "Bad index on Blob::[]");
			return(buffer[0]);
		}
		return(buffer[index]);
	} 

	unsigned long size() const {
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


#endif /* __BLOB_H */
