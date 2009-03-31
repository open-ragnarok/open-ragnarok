/* $Id$ */
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
