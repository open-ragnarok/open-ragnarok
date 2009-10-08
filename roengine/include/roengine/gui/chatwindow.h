#ifndef __ROENGINE_GUI_CHATWINDOW_H
#define __ROENGINE_GUI_CHATWINDOW_H

#include "element.h"
#include "button.h"
#include "buttonbar.h"

namespace GUI {

class ChatWindow : public Element {
protected:
	sdle::Texture tex_start, tex_mid, tex_end;
	sdle::Texture title_start, title_mid, title_end;

	std::vector<std::string> m_text;

public:
	ChatWindow(Element* parent, CacheManager&);
	ChatWindow(Element* parent, const TiXmlElement* node, CacheManager& cache);
	virtual ~ChatWindow();

	void Add(std::string);
	void Clear();

	virtual void Draw(unsigned int delay = 0);
};

}

#endif /* __ROENGINE_GUI_CHATWINDOW_H */