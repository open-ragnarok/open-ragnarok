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

public:
	int map_x, map_y;

	CharObj();
	virtual ~CharObj();

	void Draw(RswObject* map, unsigned long ticks);
	virtual void Draw();

	bool valid() const;

	bool open(CacheManager& cache, RO::CJob job, RO::CSex sex = RO::S_MALE);
};

#endif /* __ROENGINE_CHAROBJ_H */
