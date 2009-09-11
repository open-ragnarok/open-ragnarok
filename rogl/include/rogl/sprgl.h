#ifndef __ROGL_SPR_TEXTURE_H
#define __ROGL_SPR_TEXTURE_H

#include "sdle/texture.h"
#include "ro/types/spr.h"
#include "ro/types/act.h"

namespace rogl {

typedef struct sprInfo {
	// Startu and Startv
	float su, sv;
	// End U and End V
	float eu, ev;
	// Width and Height
	int w, h;
} _sprInfo;

class ROGL_DLLAPI SprGL {
protected:
	sdle::Texture m_texture;
	unsigned int m_framecount;
	sprInfo* m_info;

public:
	SprGL();
	SprGL(const SprGL&);
	virtual ~SprGL();

	bool open(const RO::SPR*);

	void Draw(unsigned int idx) const;
	void Draw(const RO::ACT::Pat& cpat, unsigned int sprno, float& x, float& y, bool v_mirror, bool ext) const;

	// Deletes all info
	void release();

	SprGL& operator = (const SprGL&);
	sdle::Texture& getTexture();
	const sdle::Texture& getTexture() const;
};

}

#endif /* __ROGL_SPR_TEXTURE_H */