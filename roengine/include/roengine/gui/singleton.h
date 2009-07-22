/* $Id$ */
#ifndef __SINGLETON_H
#define __SINGLETON_H

namespace GUI {

template<class T>
class Singleton {
protected:
	static T* m_singleton;
public:
	Singleton() {
		if (m_singleton != NULL) {
			std::cerr << "Creating two instances of singleton" << std::endl;
			return;
		}
		//use a cunning trick to get the singleton pointing to the start of the whole, rather than
		//the start of the CSingleton part of the object
		int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
		m_singleton = (T*)((int)this + offset);
	}

	~Singleton() {
		m_singleton = NULL;
	}

	/*!
		Aquire singleton (reference)
	*/
	static inline T& getSingleton() {
		if (m_singleton == NULL)
			new T();

		return(*m_singleton);
	}


	/*!
		Aquire singleton (pointer)
	*/
	static inline T* getSingletonPtr() {
		if (m_singleton == NULL)
			new T();

		return(m_singleton);
	}
};

template <typename T> T* Singleton <T>::m_singleton = 0;

}


#endif /* __SINGLETON_H */
