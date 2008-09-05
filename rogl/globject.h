/* $Id$ */
#ifndef __GLOBJECT_H
#define __GLOBJECT_H

#include "impdecl.h"

#ifdef WIN32
#	include <windows.h>
#	include <GL/gl.h>
#	include <GL/glu.h>
#else
#	include <GL/gl.h>
#	include <GL/glu.h>
#endif // WIN32

namespace ROGL {

class MYLIB_DLLAPI GLObject {
public:
	virtual void Draw() const = 0;
};

}

#endif /* __GLOBJECT_H */