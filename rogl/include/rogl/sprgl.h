#ifndef __ROGL_SPR_TEXTURE_H
#define __ROGL_SPR_TEXTURE_H

#include "sdle/texture.h"
#include "sdle/structures.h"
#include "rogl/globject.h"
#include "ro/types/spr.h"
#include "ro/types/act.h"
//#include "globject.h"

namespace rogl {

typedef struct sprInfo {
	// Start U and Start V
	float su, sv;
	// End U and End V
	float eu, ev;
	// Width and Height
	int w, h;
} _sprInfo;

/**
 * Utility class to properly load and create a SPR texture.
 */
class ROGL_DLLAPI SprGL : public GLObject {
private:
	bool m_filtering;
protected:
	sdle::Texture m_texture;
	unsigned int m_palCount;
	unsigned int m_rgbaCount;
	ro::Arr<sprInfo> m_info;

public:
	SprGL();
	SprGL(const SprGL&);
	virtual ~SprGL();

	bool open(const ro::SPR* spr, const ro::PAL* pal=NULL);

	void Draw() const;
	void Draw(unsigned int idx, bool xmirror = false) const;
	void Draw(unsigned int idx, float width, float height, bool xmirror = false, bool ymirror = false) const;
	void Draw(unsigned int idx, struct Rect rect, bool xmirror = false, bool ymirror = false, bool filtering = true, float opacity = 1.0f) const;
	void Draw(const ro::ACT::Motion& cmot, unsigned int clpno, float& x, float& y, bool v_mirror, bool ext, bool filtering = true, float opacity = 1.0f) const;

	// Deletes all info
	void release();

	unsigned int getFrameCount() const;
	/** Returns the index of an image or -1 if not valid (pal images, then rgba images) */
	unsigned int getIndex(int num, int type) const;

	SprGL& operator = (const SprGL&);
	bool operator == (const SprGL&) const;
	/**
	 * Returns information on how to draw a frame
	 * @param frame the frame index to get the parameters from
	 */
	const sprInfo& getFrameInfo(unsigned int frame) const;
	sdle::Texture& getTexture();
	const sdle::Texture& getTexture() const;
};

}

#endif /* __ROGL_SPR_TEXTURE_H */
