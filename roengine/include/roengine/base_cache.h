/* $Id$ */
#ifndef __BASE_CACHE_H
#define __BASE_CACHE_H

#include <string>
#include <map>

/**
 * Cache object for storing data associated with values.
 * It deletes all stored values on destruction.
 */
template <typename T, typename _Stor = std::string>
class BaseCache {
protected:
	typedef std::map<_Stor, T*> obj_t;
	typedef typename std::map<_Stor, T*>::iterator _itr;
	typedef typename std::map<_Stor, T*>::const_iterator _citr;
	obj_t objects;
public:
	BaseCache() {
	}

	~BaseCache() {
		clear();
	}

	unsigned int size() const {
		return(objects.size());
	}

	/**
	 * Clears all stored values deleting one by one
	 */
	void clear() {
		_itr itr;
		itr = objects.begin();
		while(itr != objects.end()) {
			std::cout << "Deleting object " << itr->first << std::endl;

			T* obj = itr->second;
			objects.erase(itr);

			delete(obj);

			itr = objects.begin();
		}
		objects.clear();
	}

	bool add(const _Stor& name, T* obj) {
		if (objects.find(name) != objects.end()) {
			std::cerr << "Object " << name << " already exists" << std::endl;
			return(false);
		}
		objects[name] = obj;
		return(true);
	}

	/**
	 * Renames an object.
	 * This function returns false if there is no "oldName" object or there already is a "newName" object.
	 */
	bool rename(const _Stor& oldName, const _Stor& newName) {
		T* obj;
		_itr itr;
		itr = objects.find(oldName);
		if (itr == objects.end())
			return(false);
		if (objects.find(newName) != objects.end())
			return(false);
		obj = itr->second;
		objects.erase(itr);

		return(add(newName, obj));
	}

	/**
	 * Removes an object from the cache and optionally deletes it.
	 * @param name Object Name
	 * @param del Boolean true to delete the object.
	 */
	bool remove(const _Stor& name, bool del = true) {
		_itr itr;
		
		itr = objects.find(name);
		if (itr == objects.end())
			return(false);
		if (del)
			delete(itr->second);

		objects.erase(name);
		return(true);
	}

	bool exists(const _Stor& name) const {
		return(objects.find(name) != objects.end());
	}

	const T* get(const _Stor& name) const {
		_citr itr = objects.find(name);
		if (itr == objects.end())
			return(NULL);
		return(itr->second);
	}

	T* get(const _Stor& name) {
		_citr itr = objects.find(name);
		if (itr == objects.end())
			return(NULL);
		return(itr->second);
	}

	const T* operator[] (const _Stor& name) const {
		_citr itr = objects.find(name);
		if (itr == objects.end())
			return(NULL);
		return(itr->second);
	}

	T* operator[] (const _Stor& name) {
		_itr itr = objects.find(name);
		if (itr == objects.end())
			return(NULL);
		return(itr->second);
	}

	const T* operator[] (const unsigned int& i) const {
		_itr itr = objects.begin();
		unsigned int j;
		for (j = 0 ; j < i; j++) {
			itr++;
		}
		return(itr->second);
	}

	T* operator[] (const unsigned int& i) {
		_itr itr = objects.begin();
		unsigned int j;
		for (j = 0 ; j < i; j++) {
			itr++;
		}
		return(itr->second);
	}
};

#endif /* __BASE_CACHE_H */
