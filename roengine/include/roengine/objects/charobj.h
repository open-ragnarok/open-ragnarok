#ifndef __ROENGINE_CHAROBJ_H
#define __ROENGINE_CHAROBJ_H

#include "actor.h"
#include "roengine/cache_manager.h"

class CharObj : public Actor {
protected:
	rogl::ActGL m_bodyact;
	rogl::ActGL m_headact;

	rogl::ActGL m_helmtopact;
	rogl::ActGL m_helmmidact;
	rogl::ActGL m_helmbottomact;
	rogl::ActGL m_weaponrightact;
	rogl::ActGL m_weaponleftact;
	rogl::ActGL m_weaponrightlightact;
	ro::CJob m_job;
	ro::CSex m_sex;
	unsigned short m_hair;

public:
	CharObj();
	virtual ~CharObj();

	virtual void setAction(unsigned short);
	virtual void WalkTo(int x, int y);
	virtual void LookAt(int x, int y);
	virtual void Attack();
	virtual void Damage();
	virtual void Dead();

	virtual void Draw();

	virtual bool valid() const;

	bool open(CacheManager& cache, ro::CJob job, ro::CSex sex = ro::S_MALE, int hair = 1);

	bool setHelm(CacheManager& cache, const char *name);
	bool setWeapon(CacheManager& cache, int weapon, bool right);
};

#endif /* __ROENGINE_CHAROBJ_H */
