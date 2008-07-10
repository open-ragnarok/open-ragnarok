/* $id$ */
/*
 * struct_io.h
 *
 *  Created on: Jun 5, 2008
 *      Author: sergio
 */

#ifndef __STRUCT_IO_H
#define __STRUCT_IO_H

#include <iostream>

namespace RO {
	template <typename T>
	class MYLIB_DLLAPI StructIO {
	protected:
		int count;
		T* data;
		T unused;

	public:
		bool m_error;

		int size() const {
			return(count);
		}

		StructIO() {
			data = NULL;
			count = 0;
			m_error = false;
		}

		void Clear() {
			if (data != NULL)
				delete[] data;
			data = NULL;
			count = 0;
		}

		~StructIO() {
			Clear();
		}

		bool readStream(std::istream& s) {
			Clear();
			m_error = false;

			s.read((char*)&count, sizeof(int));

			if (s.eof() && count > 0) {
				count = 0;
				m_error = true;
				return(false);
			}
			if (count < 0)
				count = 0;

			if (count > 0) {
				int dsize = sizeof(T) * count;
				data = new T[count];
				s.read((char*)data, dsize);
				if (s.gcount() != dsize) {
					Clear();
					m_error = true;
					return(false);
				}
			}
			return(true);
		}

		StructIO(std::istream& s) {
			data = NULL;
			count = 0;
			m_error = false;
			readStream(s);
		}

		StructIO(const StructIO& s) {
			*this = s;
		}

		StructIO<T>& operator = (const StructIO& s) {
			count = s.getCount();
			data = NULL;
			if (count > 0) {
				data = new T[count];
				memcpy(data, s.data, sizeof(T) * count);
			}

			return(*this);
		}

		StructIO<T>& operator << (std::istream& s) {
			readStream(s);
			return(*this);
		}

		StructIO<T>& operator >> (std::ostream& s) const {
			Write(s);
			return(*this);
		}

		void Write(std::ostream& s) const {
			char* ptr = (char*)&count;
			s.write(ptr, 4);
			if (count > 0) {
		  ptr = (char*)data;
		  s.write((char*)data, sizeof(T) * count);
		}
		}

		int getCount() const {
			return(count);
		}

		T& getData(int i) {
			if (i < count)
				return(data[i]);
			return(unused);
		}

		const T& getData(int i) const {
			if (i < count)
				return(data[i]);

			return(unused);
		}

		T& operator [] (int i) {
			return(getData(i));
		}

		const T& operator [] (int i) const {
			return(getData(i));
		}

		void Dump(std::ostream& s = std::cout) const {
			for (int i = 0; i < count; i++) {
				s << data[i] << " ";
			}
			s << std::endl;
		}
	};
}

#endif /* __STRUCT_IO_H */
