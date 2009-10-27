#ifndef __ROENGINE_GUI_BUTTONBAR_H
#define __ROENGINE_GUI_BUTTONBAR_H

#include "element.h"

namespace GUI {

/**
 * A button bar. Used in npc dialogs mostly.
 */
class ButtonBar : public Element {
protected:
	sdle::Texture tex_left, tex_right, tex_mid, tex_arrow;

	/** Display the arrow? */
	bool m_arrow;

	/** Display the arrow at the beggining? */
	bool m_arrow_begin;

	void CheckMinimumSize();

public:
	ButtonBar(Element* parent, CacheManager&);
	ButtonBar(Element* parent, const TiXmlElement* node, CacheManager& cache);
	virtual ~ButtonBar();

	virtual void Draw(unsigned int delay = 0);
};

}

#endif /* __ROENGINE_GUI_CHATWINDOW_H */
