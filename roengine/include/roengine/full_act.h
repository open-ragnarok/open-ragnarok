#ifndef __ROENGINE_FULLACT_H
#define __ROENGINE_FULLACT_H

#include "cache_manager.h"
#include "ro/types/act.h"
#include "ro/types/spr.h"
#include "rogl/sprgl.h"

class FullAct {
protected:
	const RO::ACT* m_act;
	rogl::SprGL m_spr;

public:
	FullAct();
	FullAct(const FullAct&);
	FullAct(const RO::ACT*, const rogl::SprGL&);

	FullAct& operator = (const FullAct&);

	bool Load(const std::string& act, CacheManager& cache);
	void Clear();

	const RO::ACT* getAct() const;
	const sdle::Texture& getTextures() const;
	const rogl::SprGL& getSpr() const;
	const RO::ACT* operator -> () const;
};

void DrawFullAct(const FullAct&, float x, float y, int act_no = 0, int pat_no = 0, bool ext = false, const FullAct* parent = NULL, bool v_mirror = false, bool cross = false);

#endif /* __ROENGINE_FULLACT_H */
