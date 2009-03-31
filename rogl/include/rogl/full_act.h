#ifndef __ROGL_FULLACT_H
#define __ROGL_FULLACT_H

#include "texturemanager.h"
#include "ro/types/act.h"
#include "ro/types/spr.h"

class FullAct {
protected:
	const RO::ACT* m_act;
	Texture::PointerCache m_textures;

public:
	FullAct();
	FullAct(const FullAct&);
	FullAct(const RO::ACT*, const Texture::PointerCache&);

	FullAct& operator = (const FullAct&);

	bool Load(const std::string& act, ROObjectCache& m_ro_objects, FileManager& m_filemanager, TextureManager& m_texturemanager);

	const RO::ACT* getAct() const;
	const Texture::PointerCache& getTextures() const;
	const RO::ACT* operator -> () const;
};

#endif /* __ROGL_FULLACT_H */