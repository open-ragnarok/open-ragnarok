#ifndef __ROENGINE_GUI_STATUSWINDOW_H
#define __ROENGINE_GUI_STATUSWINDOW_H

#include "element.h"
#include "button.h"
#include "buttonbar.h"

namespace GUI {

class StatusWindow : public Element {
protected:
	// l = left, m = mid, r = right
	sdle::Texture title_l, title_m, title_r;
	sdle::Texture tex_1, tex_2;

//	std::vector<std::string> m_text;

public:
	StatusWindow(Element* parent, CacheManager&);
	StatusWindow(Element* parent, const TiXmlElement* node, CacheManager& cache);
	virtual ~StatusWindow();

	void Add(std::string);
	void Clear();

	virtual void Draw(unsigned int delay = 0);
};

}

#endif /* __ROENGINE_GUI_STATUSWINDOW_H */
