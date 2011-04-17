/* $Id: inventorywindow.h $ */
#ifndef __ROENGINE_GUI_INVENTORYWINDOW_H
#define __ROENGINE_GUI_INVENTORYWINDOW_H

#include "element.h"
#include "button.h"
#include "buttonbar.h"

namespace GUI {

class InventoryWindow : public Element {
protected:
//	sdle::Texture tex_start, tex_mid, tex_end;
//	sdle::Texture title_start, title_mid, title_end;
	// l = left, m = mid, r = right
	// u = up, m = mid, d = down
	sdle::Texture tex_l, tex_m, tex_r, tex_invert;

	std::vector<std::string> m_text;

public:
	InventoryWindow(Element* parent, CacheManager&);
	InventoryWindow(Element* parent, const TiXmlElement* node, CacheManager& cache);
	virtual ~InventoryWindow();

	void Add(std::string);
	void Clear();

	void WindowSeq(float x, float y, float w, float h, const sdle::Texture& tex);
	void WindowSeq(float x, float y, float w, float h, const sdle::Texture& start, const sdle::Texture& mid, const sdle::Texture& end);

	virtual void Draw(unsigned int delay = 0);
};

}

#endif /* __ROENGINE_GUI_INVENTORYWINDOW_H */
