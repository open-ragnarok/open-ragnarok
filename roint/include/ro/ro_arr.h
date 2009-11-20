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
#ifndef __RO_ARR_H
#define __RO_ARR_H

namespace ro {

/** Array.
 * The data type must support 'a = b'.
 */
template <typename T>
class ROINT_DLLAPI Arr {
protected:
	unsigned int _size;
	T* _data;
public:
	typedef T type;

	Arr() {
		_size = 0;
		_data = 0;
	}

	~Arr() {
		clear();
	}

	void resize(unsigned int n) {
		T* newData = 0;
		if (n > 0) {
			newData = new T[n];
			if (_size > 0) {
				unsigned int i, end = (_size < n? _size: n);
				for (i = 0; i < end; i++) {
					newData[i] = _data[i];
				}
				delete[] _data;
			}
		}
		_size = n;
		_data = newData;
	}

	void resize(unsigned int n, const T& value) {
		T* newData = 0;
		if (n > 0) {
			unsigned int i = 0;
			newData = new T[n];
			if (_size > 0) {
				unsigned int end = (_size < n? _size: n);
				for (; i < end; i++) {
					newData[i] = _data[i];
				}
				delete[] _data;
			}
			for (; i < n; i++) {
				newData[i] = value;
			}
		}
		_size = n;
		_data = newData;
	}

	unsigned int size() const {
		return(_size);
	}

	void operator = (const Arr<T>& a) {
		clear();
		_size = a._size;
		if (_size > 0) {
			_data = new T[_size];
			for (unsigned int i = 0; i < _size; i++) {
				_data[i] = a._data[i];
			}
		}
	}

	type& operator[] (unsigned int i) {
		return(_data[i]);
	}

	const type& operator[] (unsigned int i) const {
		return(_data[i]);
	}

	void clear() {
		if (_size > 0) {
			delete[] _data;
			_data = 0;
			_size = 0;
		}
	}
};

} /* namespace ro */

#endif /* __RO_ARR_H */
