/* $Id: dropdownlist.h $ */
#ifndef __GUI_DROPDOWNLIST_H
#define __GUI_DROPDOWNLIST_H

#include "list.h"

namespace GUI {

class DropDownList : public List {
public:
	DropDownList(Element* parent, const TiXmlElement* e, CacheManager& cache);
	virtual ~DropDownList();

};

}

#endif /* __GUI_DROPDOWNLIST_H */
