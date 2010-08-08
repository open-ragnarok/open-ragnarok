/* $Id: dropdownlist.cc $ */
#include "stdafx.h"

#include "roengine/gui/dropdownlist.h"
#include "roengine/gui/gui.h"

namespace GUI {

DropDownList::DropDownList(Element* parent, const TiXmlElement* e, CacheManager& cache) : List(parent, e, cache) {
}

DropDownList::~DropDownList() {
}

}