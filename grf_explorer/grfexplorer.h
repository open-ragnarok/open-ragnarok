#ifndef __GRFEXPLORER_H
#define __GRFEXPLORER_H

#include "wx_support.h" 
#include "grflist.h"

enum {
    ID_Quit = 1,
	ID_About,
	ID_GRF_Open,
	ID_GRF_Close,
};

class MyFrame: public wxFrame {
protected:
	GRFList* table;
	RO::GRF m_grf;
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);

    DECLARE_EVENT_TABLE()

	enum {
		ID_Table
	};
};

#endif /* __GRFEXPLORER_H */