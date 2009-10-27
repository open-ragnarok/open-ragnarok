#ifndef __ROENGINE_HOMUNOBJ_H
#define __ROENGINE_HOMUNOBJ_H

#include "actor.h"
#include "roengine/cache_manager.h"

class HomunObj : public Actor {
protected:
	rogl::ActGL m_homun;

public:
	HomunObj();
	virtual ~HomunObj();

	virtual void Draw();

	virtual bool valid() const;

	bool open(CacheManager& cache, std::string name);
};

#endif /* __ROENGINE_HOMUNOBJ_H */
