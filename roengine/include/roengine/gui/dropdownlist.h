/* $Id: dropdownlist.h $ */
#ifndef __GUI_DROPDOWNLIST_H
#define __GUI_DROPDOWNLIST_H

#include "element.h"
#include "list.h"
#include "label.h"
#include "button.h"

namespace GUI {

//class DropDownList : public List {
//class DropDownList : public Label {
class DropDownList : public Element {
public:
	DropDownList(Element* parent, const TiXmlElement* e, CacheManager& cache);
	virtual ~DropDownList();

	Button *button;
	Label *label;
	List *list;

	std::string m_text;
	std::vector<std::string> m_items;
	int selected;

	void Add(std::string s);
	void Clear();
//	void Click();
	bool HandleMouseDown(int x, int y, int button);
	void Draw(unsigned int delay);
};

}

#endif /* __GUI_DROPDOWNLIST_H */
