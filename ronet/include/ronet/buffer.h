/* $Id$ */
#ifndef __RONET_BUFFER_H
#define __RONET_BUFFER_H

#include "ronet_import.h"

#include <iostream>

namespace ronet {
	template <typename T, int _StorSize = 2048>
	class Buffer {
	protected:
		T* data;
		unsigned int allocSize;

		/** Where are we reading now.
		 * Anything before this point is considered read and done for.
		 */
		unsigned int dataStart;

		/** Where we've written so far..
		 * Anything after this point is considered unused.
		 */
		unsigned int dataEnd;

		void alloc(const unsigned int& size) {
			if (size == 0) {
				// Clear everything
				if (data != NULL)
					delete[] data;
				data = NULL;
				allocSize = 0;
				dataStart = 0;
				dataEnd = 0;
				return;
			}

			unsigned int r = size % _StorSize;

			// What's the size of the new buffer?
			unsigned int newsize = size;
			if (r > 0)
				newsize += _StorSize - r; // Make is a multiple of _StorSize

			// No need to grow the memory buffer.
			if (newsize == allocSize) {
				// Does the data fits in the current buffer?
				if (dataStart + size <= allocSize) {
					// The data fits. No need for panic.
					return;
				}

				// Moving the useful data to the beggining of the buffer...
				memmove(data, data+dataStart, dataSize());
				dataEnd = dataSize();
				dataStart = 0;
				return;
			}

			T* newdata = new T[newsize];
			if (data != NULL) {
				// How much are we going to copy?
				unsigned int copysize = (dataSize() > size)?size:dataSize();
				dataEnd = copysize;
				if (copysize > 0)
					memcpy(newdata, data + dataStart, copysize);
			}

			delete[] data;
			data = newdata;
			allocSize = newsize;
			dataStart = 0;
		}

	public:
		Buffer() {
			data = NULL;
			dataStart = 0;
			dataEnd = 0;
			allocSize = 0;
		}

		Buffer(const Buffer<T>& b) {
			data = NULL;
			dataStart = 0;
			dataEnd = 0;
			alloc(b.dataSize());
			memcpy(data, b.getBuffer(), b.dataSize());
			dataEnd += b.dataSize();
		}

		Buffer(const T* src, int size) {
			data = NULL;
			dataStart = 0;
			dataEnd = 0;
			alloc(size);
			memcpy(data, src, size);
			dataEnd += size;
		}

		~Buffer() {
			clear();
		}

		void clear() {
			alloc(0);
		}

		unsigned int dataSize() const {
			return(dataEnd - dataStart);
		}

		unsigned int write(const T* src, int size) {
			alloc(dataSize() + size);
			memcpy(data + dataEnd, src, size);
			dataEnd += size;
			return(size);
		}

		unsigned int write(const Buffer<T>& b) {
			return(write(b.getBuffer(), b.dataSize()));
		}

		unsigned int write(std::istream& s, const unsigned int& size) {
			T buf[_StorSize];
			int k = 0;
			unsigned int wrote = 0;

			while (!s.eof() && (wrote <= size)) {
				buf[k++] = s.get();
				wrote++;
				if (k >= _StorSize) {
					write(buf, k);
					k = 0;
				}
			}
			if (k > 0)
				write(buf, k);

			return(wrote);
		}

		unsigned int write(std::istream& s) {
			T buf[_StorSize];
			int size = 0;

			while (!s.eof()) {
				buf[size++] = s.get();
				if (size >= _StorSize) {
					write(buf, size);
					size = 0;
				}
			}
			if (size > 0) {
				write(buf, size);
			}

			return(size);
		}

		/**
		 * Grabs the next bytes without changing the pointers
		 */
		unsigned int peek(T* dest, const unsigned int& size) const {
			unsigned int ret = size;
			if (size > dataSize())
				ret = dataSize();
			if (ret == 0)
				return(0);

			memcpy(dest, data + dataStart, ret);

			return(ret);
		}

		unsigned int read(T* dest, const unsigned int& size) {
			unsigned int ret = size;
			if (size > dataSize())
				ret = dataSize();
			if (ret == 0)
				return(0);

			memcpy(dest, data + dataStart, ret);
			dataStart += size;
			alloc(dataSize());
			return(ret);
		}

		unsigned int read(Buffer<T>& b) {
			unsigned int ret = b.write(*this);
			alloc(0);
			return(ret);
		}

		unsigned int read(std::ostream& dest) {
			int ret = dataSize();
			dest.write((char*)data + dataStart, ret);
			alloc(0);
			return(ret);
		}

		void ignore(const unsigned int& size) {
			dataStart += size;
			if (dataStart > dataEnd)
				dataStart = dataEnd;
		}

		unsigned int read(std::ostream& dest, const unsigned int& size) {
			int ret = size;
			if (dataSize() < size) {
				ret = dataSize();
			}
			dest.write((char*)data + dataStart, ret);
			dataStart += ret;
			alloc(dataSize());
			return(ret);
		}

		const T* getBuffer() const {
			const T* ret = data;
			ret += dataStart;

			return(ret);
		}

		Buffer<T, _StorSize>& operator << (std::istream& s) {
			write(s);
			return(*this);
		}

		Buffer<T, _StorSize>& operator << (Buffer<T>& b) {
			write(b);
			return(*this);
		}

		Buffer<T, _StorSize>& operator >> (std::ostream& s) {
			s.write((char*)data + dataStart, dataSize());
			alloc(0);
			return(*this);
		}

		Buffer<T, _StorSize>& operator >> (T& c) {
			if (dataSize() == 0)
				return(*this);
			c = *(data+dataStart);
			dataStart++;
			return(*this);
		}

		Buffer<T, _StorSize>& operator >> (short& c) {
			c = 0;
			if (dataSize() < sizeof(short))
				return(*this);

			c = *(short*)(data+dataStart);

			dataStart += sizeof(short);
			return(*this);
		}

		Buffer<T, _StorSize>& operator >> (unsigned short& c) {
			c = 0;
			if (dataSize() < sizeof(unsigned short))
				return(*this);

			c = *(unsigned short*)(data+dataStart);

			dataStart += sizeof(unsigned short);
			return(*this);
		}

		Buffer<T, _StorSize>& operator >> (int& c) {
			c = 0;
			if (dataSize() < sizeof(int))
				return(*this);

			c = *(int*)(data+dataStart);
			dataStart += sizeof(int);
			return(*this);
		}

		Buffer<T, _StorSize>& operator >> (unsigned int& c) {
			c = 0;
			if (dataSize() < sizeof(unsigned int))
				return(*this);

			c = *(unsigned int*)(data+dataStart);
			dataStart += sizeof(unsigned int);
			return(*this);
		}


	};

	class RONET_DLLAPI ucBuffer : public Buffer<unsigned char> {};
}

#endif /* __RONET_BUFFER_H */