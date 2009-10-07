/* $Id$ */
#ifndef __GUI_DIALOG_H
#define __GUI_DIALOG_H

#include "desktop.h"
#include "button.h"

namespace GUI {
typedef enum {
	BTN_OK = 0x01,
	BTN_CANCEL = 0x02,
	BTN_EXIT = 0x04
} eDialogButtons;

/**
 * Generic modal dialog box to display messages and requiring the user to take an action before being able
 * to do anything else.
 */
class Dialog : public Desktop {
protected:
	Button* btnOk;
	Button* btnCancel;
	Button* btnExit;
public:
	Dialog(const std::string& title, const std::string& msg, CacheManager&);
	virtual ~Dialog();
	bool handleSelect(GUI::Event&);
};
}

#endif /* __GUI_DIALOG_H */

