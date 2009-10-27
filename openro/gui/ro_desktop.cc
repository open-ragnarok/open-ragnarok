
/* $Id$ */#include "stdafx.h"

#include "ro_desktop.h"
#include "openro.h"

RODesktop::RODesktop(const std::string& fn, OpenRO* ro) : Desktop(fn, *ro) {
	m_ro = ro;
}
