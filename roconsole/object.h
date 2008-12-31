/* $Id$ */

#ifndef __OBJECT_H
#define __OBJECT_H

#include <string>

class Object {
public:
	Object();
	virtual ~Object();
	virtual const std::string getObjectType() const = 0;
	virtual const std::string toString() const = 0;
};

#endif /* __OBJECT_H */

// EOF