/* $Id$ */
#ifndef __CUBE_OBJECT_H
#define __CUBE_OBJECT_H

#include "../gl_object.h"

class CubeObject : public GLObject {
protected:
	float m_size;

public:
	CubeObject(const float& size = 1.0f);

	virtual void Draw();
};

#endif /* __CUBE_OBJECT_H */
