#ifndef __OPENRO_RO_DESKTOP_H
#define __OPENRO_RO_DESKTOP_H

#include "roengine/gui/desktop.h"

class OpenRO;

class RODesktop : public GUI::Desktop {
protected:
	OpenRO* m_ro;

public:
	RODesktop(const std::string&, OpenRO*);
};

#endif /* __OPENRO_RO_DESKTOP_H */
