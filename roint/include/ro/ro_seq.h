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
#ifndef __RO_SEQ_H
#define __RO_SEQ_H

namespace RO {

template <typename T>
class ROINT_DLLAPI Seq {
protected:
	template <typename Tt = T>
	class ROINT_DLLAPI Storage {
	protected:
		Tt data;
	public:
		Storage<Tt> *next;
		Storage<Tt> *prev;

		Storage(Tt d) {
			data = d;
			next = this;
			prev = this;
		}

		Storage(Tt d, Storage<Tt>* first) {
			data = d;
			next = first;
			prev = first->prev;
			prev->next = this;
			next->prev = this;
		}

		~Storage() {
			if (next != this) {
				next->prev = prev;
				prev->next = next;
			}
		}

		Tt& operator *() { return(data); }
		const Tt& operator *() const { return(data); }

		Tt& getData() { return(data); }
		const Tt& getData() const { return(data); }
	};

	unsigned int _size;
	Storage<T> *_first;

	Storage<T>* getIndex(const unsigned int& n) {
		if (_first == NULL)
			return(NULL);

		Storage<T>* r = _first;
		for (unsigned int i = 0; i < n; i++)
			r = r->next;

		return(r);
	}

	const Storage<T>* getIndex(const unsigned int& n) const {
		if (_first == NULL)
			return(NULL);

		Storage<T>* r = _first;
		for (unsigned int i = 0; i < n; i++)
			r = r->next;

		return(r);
	}


public:
	Seq() {
		_size = 0;
		_first = NULL;
	}

	~Seq() {
		clear();
	}

	bool push_back(T data) {
		if (_first == NULL)
			_first = new Storage<T>(data);
		else
			new Storage<T>(data, _first);

		_size++;

		return(true);
	}

	unsigned int size() const { return(_size); }

	T& operator[] (unsigned int x) {
		Storage<T>* n = getIndex(x);
		return(n->getData());		
	}

	const T& operator[] (unsigned int x) const {
		const Storage<T>* n = getIndex(x);
		return(n->getData());		
	}

	void clear() {
		while (_size > 0) {
			delete(_first->next);
			_size--;
		}
		_first = NULL;
	}
};

};

#endif /* __RO_SEQ_H */
