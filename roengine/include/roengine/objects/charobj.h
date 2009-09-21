#ifndef __ROENGINE_CHAROBJ_H
#define __ROENGINE_CHAROBJ_H

#include "rogl/actgl.h"
#include "roengine/cache_manager.h"

#include "rsw_object.h"

class CharObj : public GLObject {
protected:
	rogl::ActGL m_bodyact;
	rogl::ActGL m_headact;

	RO::CJob m_job;
	RO::CSex m_sex;
	unsigned short m_hair;

	RswObject* m_map;

	/** Current action to draw */
	unsigned short m_act;

	/** Current frame in the action */
	unsigned short m_frame;

public:
	int map_x, map_y;

	CharObj();
	virtual ~CharObj();

	virtual void Draw();

	bool valid() const;

	void setMap(RswObject*);

	bool open(CacheManager& cache, RO::CJob job, RO::CSex sex = RO::S_MALE);
};

#endif /* __ROENGINE_CHAROBJ_H */
