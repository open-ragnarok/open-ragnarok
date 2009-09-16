#ifndef __ROENGINE_CHAROBJ_H
#define __ROENGINE_CHAROBJ_H

#include "rogl/actgl.h"
#include "roengine/gl_object.h"

#include "rsw_object.h"

class CharObj : public GLObject {
protected:
public:
	int map_x, map_y;
	rogl::ActGL m_actgl;

	CharObj();
	virtual ~CharObj();

	void Draw(RswObject* map, unsigned long ticks);
	virtual void Draw();
};

#endif /* __ROENGINE_CHAROBJ_H */
