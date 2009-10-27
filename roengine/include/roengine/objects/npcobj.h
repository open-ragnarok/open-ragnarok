#ifndef __ROENGINE_NPCOBJ_H
#define __ROENGINE_NPCOBJ_H

#include "actor.h"
#include "roengine/cache_manager.h"

class NpcObj : public Actor {
protected:
	rogl::ActGL m_npc;

public:
	NpcObj();
	virtual ~NpcObj();

	virtual void Draw();

	virtual bool valid() const;

	bool open(CacheManager& cache, std::string name);
};

#endif /* __ROENGINE_CHAROBJ_H */
