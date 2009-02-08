/* $Id$ */
#ifndef __BASE_CACHE_H
#define __BASE_CACHE_H

#include <string>
#include <map>

template <class T>
class BaseCache {
protected:
	typedef std::map<std::string, T*> obj_t;
	obj_t objects;
public:
	BaseCache() {
	}

	~BaseCache() {
		clear();
	}

	void clear() {
		obj_t::iterator itr = objects.begin();
		while(itr != objects.end()) {
			// std::cout << "Deleting object " << itr->first << std::endl;
			delete(itr->second);
			itr++;
		}
		objects.clear();
	}

	bool add(const std::string& name, T* obj) {
		if (objects.find(name) != objects.end()) {
			std::cerr << "Object " << name << " already exists" << std::endl;
			return(false);
		}
		objects[name] = obj;
		return(true);
	}

	bool remove(const std::string& name) {
		obj_t::iterator itr;
		itr = objects.find(name);
		if (itr == objects.end())
			return(false);
		delete(itr->second);
		objects.erase(itr);
		return(true);
	}

	bool exists(const std::string& name) const {
		if (objects.find(name) != objects.end())
			return(true);
		return(false);
	}

	const T* get(const std::string& name) const {
		obj_t::const_iterator itr = objects.find(name);
		if (itr == objects.end())
			return(NULL);
		return(itr->second);
	}

	T* get(const std::string& name) {
		obj_t::iterator itr = objects.find(name);
		if (itr == objects.end())
			return(NULL);
		return(itr->second);
	}

	const T* operator[] (const std::string& name) const {
		obj_t::const_iterator itr = objects.find(name);
		if (itr == objects.end())
			return(NULL);
		return(itr->second);
	}

	T* operator[] (const std::string& name) {
		obj_t::iterator itr = objects.find(name);
		if (itr == objects.end())
			return(NULL);
		return(itr->second);
	}

	const T* operator[] (const unsigned int& i) const {
		obj_t::const_iterator itr = objects.begin();
		unsigned int j;
		for (j = 0 ; j < i; j++) {
			itr++;
		}
		return(itr->second);
	}

	T* operator[] (const unsigned int& i) {
		obj_t::iterator itr = objects.begin();
		unsigned int j;
		for (j = 0 ; j < i; j++) {
			itr++;
		}
		return(itr->second);
	}
};

#endif /* __BASE_CACHE_H */