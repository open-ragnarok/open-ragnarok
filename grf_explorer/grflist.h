#ifndef __GRFLIST_H
#define __GRFLIST_H

#include "wx_support.h"
#include <wx/listctrl.h>

#include "ro.h"

class GRFList : public wxListCtrl {
protected:
	RO::GRF* m_grf;
public:
    GRFList(wxWindow *parent, const wxWindowID id);

	bool LoadGRF(RO::GRF* grf);

	DECLARE_NO_COPY_CLASS(GRFList)
    //DECLARE_EVENT_TABLE()
};

#endif /* __GRFLIST_H */