/* $Id: sysbox.h $ */
#ifndef __ROENGINE_GUI_SYSBOX_H
#define __ROENGINE_GUI_SYSBOX_H

#include "element.h"

namespace GUI {

class SysBox : public Element {
protected:
	// l = left, m = mid, r = right
	// u = up, m = mid, d = down
	sdle::Texture tex_lu, tex_lm, tex_ld, tex_mu, tex_mm, tex_md, tex_ru, tex_rm, tex_rd, tex_arrow_l, tex_arrow_r;

public:
	SysBox(Element* parent, CacheManager&);
	SysBox(Element* parent, const TiXmlElement* node, CacheManager& cache);
	virtual ~SysBox();

	virtual void Draw(unsigned int delay = 0);
};

}

#endif /* __ROENGINE_GUI_SYSBOX_H */
