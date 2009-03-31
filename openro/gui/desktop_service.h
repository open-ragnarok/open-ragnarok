#ifndef __DESKTOP_SERVICE_H
#define __DESKTOP_SERVICE_H

// Forward declaration
class DesktopService;

#include "ro_desktop.h"
#include "gui/list.h"

class DesktopService : public RODesktop {
protected:
	GUI::List* slist;
public:
	DesktopService(OpenRO*);

	bool handleBtnOk(GUI::Event&);
	bool handleBtnCancel(GUI::Event&);

	void clear();
	void add(const std::string&);
	int getSelected() const;
};

#endif /* __DESKTOP_SERVICe_H */
