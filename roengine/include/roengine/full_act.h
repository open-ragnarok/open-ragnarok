#ifndef __ROENGINE_FULLACT_H
#define __ROENGINE_FULLACT_H

#include "texturemanager.h"
#include "ro/types/act.h"
#include "ro/types/spr.h"

class FullAct {
protected:
	const RO::ACT* m_act;
	rogl::Texture::PointerCache m_textures;

public:
	FullAct();
	FullAct(const FullAct&);
	FullAct(const RO::ACT*, const rogl::Texture::PointerCache&);

	FullAct& operator = (const FullAct&);

	bool Load(const std::string& act, ROObjectCache& m_ro_objects, FileManager& m_filemanager, TextureManager& m_texturemanager);
	void Clear();

	const RO::ACT* getAct() const;
	const rogl::Texture::PointerCache& getTextures() const;
	const RO::ACT* operator -> () const;
};

void DrawFullAct(const FullAct&, float x, float y, int act_no = 0, int pat_no = 0, bool ext = false, const FullAct* parent = NULL, bool v_mirror = false, bool cross = false);

#endif /* __ROENGINE_FULLACT_H */
