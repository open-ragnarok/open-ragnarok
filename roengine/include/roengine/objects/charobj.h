#ifndef __ROENGINE_CHAROBJ_H
#define __ROENGINE_CHAROBJ_H

#include "actor.h"
#include "roengine/cache_manager.h"

class CharObj : public Actor {
protected:
	rogl::ActGL m_bodyact;
	rogl::ActGL m_headact;

	ro::CJob m_job;
	ro::CSex m_sex;
	unsigned short m_hair;

public:
	CharObj();
	virtual ~CharObj();

//	virtual void setAction(unsigned short);

	virtual void Draw();

	virtual bool valid() const;

	bool open(CacheManager& cache, ro::CJob job, ro::CSex sex = ro::S_MALE, int hair = 1);
};

#endif /* __ROENGINE_CHAROBJ_H */
