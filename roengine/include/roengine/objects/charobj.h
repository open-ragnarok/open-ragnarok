#ifndef __ROENGINE_CHAROBJ_H
#define __ROENGINE_CHAROBJ_H

#include "actor.h"
#include "roengine/cache_manager.h"

class CharObj : public Actor {
protected:
	rogl::ActGL m_bodyact;
	rogl::ActGL m_headact;

	RO::CJob m_job;
	RO::CSex m_sex;
	unsigned short m_hair;

public:
	CharObj();
	virtual ~CharObj();

	virtual void Draw();

	virtual bool valid() const;

	bool open(CacheManager& cache, RO::CJob job, RO::CSex sex = RO::S_MALE);
};

#endif /* __ROENGINE_CHAROBJ_H */
