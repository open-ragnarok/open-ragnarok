#ifndef __ROENGINE_MOBOBJ_H
#define __ROENGINE_MOBOBJ_H

#include "actor.h"
#include "roengine/cache_manager.h"

class MobObj : public Actor {
protected:
	rogl::ActGL m_mob;
	bool shadowLoaded;

public:
	MobObj();
	virtual ~MobObj();

	virtual void setAction(unsigned short);
	virtual void Attack();
	virtual void Damage();
	virtual void Dead();

	virtual void Draw();

	virtual bool valid() const;

	bool open(CacheManager& cache, std::string name);
};

#endif /* __ROENGINE_MOBOBJ_H */
