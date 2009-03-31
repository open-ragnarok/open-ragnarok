/* $Id: blob.h 45 2009-02-12 13:24:41Z sergio $ */
#ifndef __ROGL_BLOB_H
#define __ROGL_BLOB_H

#include <string>
#include <cassert>

#include "rogl_import.h"

namespace rogl {
	template<class T, int i>
	class ROGL_DLLAPI Blob {
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

	class ROGL_DLLAPI DynamicBlob {
	protected:
		unsigned long dataSize;
		unsigned char* buffer;
	public:
		DynamicBlob(const unsigned long& size = 0);
		DynamicBlob(const DynamicBlob& o);
		virtual ~DynamicBlob();

		void setSize(const unsigned long& size);
		unsigned char& get(unsigned long index);
		const unsigned char& get(unsigned long index) const;
		unsigned long size() const;
		unsigned long blobSize() const;
		unsigned char* getBuffer();
		const unsigned char* getBuffer() const;
		void clear();
		void write(std::ostream& o) const;

		DynamicBlob& operator = (const DynamicBlob& o);
		unsigned char& operator[](int index);
		const unsigned char& operator[](int index) const;
		unsigned char* operator*();
	};

	typedef DynamicBlob DynamicBlobUChar;

	class ROGL_DLLAPI DynamicBlobChar : public DynamicBlob {
	public:
		DynamicBlobChar(const unsigned long& size = 0);
		DynamicBlobChar(const DynamicBlobChar& o);
		DynamicBlobChar(const DynamicBlob& o);

		DynamicBlobChar& operator = (const DynamicBlobChar& o);
		char& operator[](int index);
		const char& operator[](int index) const;
		char* operator*();
	};
}

#endif /* __ROGL_BLOB_H */
