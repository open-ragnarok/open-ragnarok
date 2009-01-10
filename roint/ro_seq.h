/* $Id$ */
#ifndef __RO_SEQ_H
#define __RO_SEQ_H


namespace RO {

template <typename T>
class MYLIB_DLLAPI Seq {
protected:
	template <typename T>
	class MYLIB_DLLAPI Storage {
	protected:
		T data;
	public:
		Storage<T> *next;
		Storage<T> *prev;

		Storage(T d) {
			data = d;
			next = this;
			prev = this;
		}

		Storage(T d, Storage<T>* first) {
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

		T& operator *() { return(data); }
		const T& operator *() const { return(data); }

		T& getData() { return(data); }
		const T& getData() const { return(data); }
	};

	unsigned int _size;
	Storage<T> *first;

	Storage<T>* getIndex(const unsigned int& n) {
		if (first == NULL)
			return(NULL);

		Storage<T>* r = first;
		for (unsigned int i = 0; i < n; i++)
			r = r->next;

		return(r);
	}

	const Storage<T>* getIndex(const unsigned int& n) const {
		if (first == NULL)
			return(NULL);

		Storage<T>* r = first;
		for (unsigned int i = 0; i < n; i++)
			r = r->next;

		return(r);
	}


public:
	Seq() {
		_size = 0;
		first = NULL;
	}

	~Seq() {
		clear();
	}

	bool push_back(T data) {
		if (first == NULL)
			first = new Storage<T>(data);
		else {
			new Storage<T>(data, first);
		}
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
			delete(first->next);
			_size--;
		}
		first = NULL;
	}
};

};

#endif /* __RO_SEQ_H */