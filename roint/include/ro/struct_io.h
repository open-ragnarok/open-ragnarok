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
/*
 * struct_io.h
 *
 *  Created on: Jun 5, 2008
 *      Author: sergio
 *    Revision: $Rev: 20 $
 */

#ifndef __RO_STRUCT_IO_H
#define __RO_STRUCT_IO_H

#include <iostream>

namespace ro {

/**
 * Helps in reading and writing data that comes in the type &lt;count&gt;&lt;sequence of type&gt;, assuming
 * count is an int and is immediately followed by the sequence of structures
 */
template <typename T>
class ROINT_DLLAPI StructIO {
protected:
	/** Number of scructures that we have */
	int count;
	/** Stores our data */
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
		Clear();
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

	/**
	 * Writes our data to a stream in the same way as we read it.
	 */
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

} /* namespace ro */

#endif /* __RO_STRUCT_IO_H */
