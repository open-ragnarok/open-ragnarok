/* $Id$ */
#ifndef __BASIC_OBJECTS_H
#define __BASIC_OBJECTS_H

#include "object.h"

template <typename T>
class BasicObject : public Object {
protected:
	typedef T t_stor;

	t_stor data;
public:
	BasicObject() { data = 0; }
	BasicObject(const t_stor& d) { data = d; }
	BasicObject(const BasicObject<T>& o) { data = o.data; }
	BasicObject(const BasicObject<T>* o) { data = o->data; }
	virtual ~BasicObject() {}
	T operator *() { return(data); }
	T operator = (const T& d) { data = d; return(data); }

	BasicObject<T> operator = (const BasicObject<T>& d) {
		data = d.data;
		return(*this);
	}

	bool operator == (const t_stor& d) const { return(data == d); }
	bool operator != (const t_stor& d) const { return(data != d); }
	bool operator >  (const t_stor& d) const { return(data > d);  }
	bool operator >= (const t_stor& d) const { return(data >= d); }
	bool operator <  (const t_stor& d) const { return(data < d);  }
	bool operator <= (const t_stor& d) const { return(data <= d); }

	bool operator == (const BasicObject<T>& d) const { return(data == d.data); }
	bool operator != (const BasicObject<T>& d) const { return(data != d.data); }
	bool operator >  (const BasicObject<T>& d) const { return(data > d.data);  }
	bool operator >= (const BasicObject<T>& d) const { return(data >= d.data); }
	bool operator <  (const BasicObject<T>& d) const { return(data < d.data);  }
	bool operator <= (const BasicObject<T>& d) const { return(data <= d.data); }
};

class EmptyObject : public Object {
	virtual const std::string getObjectType() const;
	virtual const std::string toString() const;
};

class TextObject : public Object {
protected:
	std::string data;
public:
	TextObject();
	TextObject(const std::string&);
	virtual ~TextObject();
	virtual const std::string getObjectType() const;
	virtual const std::string toString() const;

	TextObject operator = (const std::string&);
	TextObject operator = (const TextObject&);
};

class IntObject : public BasicObject<int> {
public:
	IntObject();
	IntObject(const t_stor& d);
	IntObject(const IntObject& o);
	IntObject(const IntObject* o);
	IntObject(const std::string& s);
	virtual ~IntObject();
	virtual const std::string getObjectType() const;
	virtual const std::string toString() const;
};

class FloatObject : public BasicObject<float> {
public:
	FloatObject();
	FloatObject(const t_stor& d);
	FloatObject(const FloatObject& o);
	FloatObject(const FloatObject* o);
	FloatObject(const std::string& s);
	virtual ~FloatObject();
	virtual const std::string getObjectType() const;
	virtual const std::string toString() const;
};

#endif /* __BASIC_OBJECTS_H */

// EOF