#ifndef __GRFEXPLORER_H
#define __GRFEXPLORER_H

#include <wx/srchctrl.h>
#include <wx/splitter.h>
#include <wx/propgrid/propgrid.h>
#include <wx/aui/aui.h>

#include "wx_support.h" 
#include "grflist.h"

enum {
    ID_Quit = 1,
	ID_About,
	ID_GRF_Open,
	ID_GRF_Close,
	ID_GRF_Info,
	ID_GRF_Extract,
	ID_GRF_ExtractWithDirs,
	ID_GRF_ExtractAll,
	ID_GRF_ExtractAllWithDirs,
	ID_GRF_XMLInfo,
	ID_View_SearchBar,
	ID_View_Tree,
	ID_View_Property,
	ID_Search,
	ID_Search_Incremental,
	ID_Search_Wildcard,
	ID_Search_Recents,
	ID_PropGrid = 1000,
};

class MyFrame: public wxFrame {
protected:
	GRFList* table;
	GRFTree* tree;
	ro::GRF m_grf;
    wxAuiManager m_auimgr;

	wxSearchCtrl* searchCtrl;
	wxMenuBar* menuBar;
//	wxStatusBar* statusBar;
	wxSplitterWindow *m_splitter;
	wxSplitterWindow *m_splitter2;

	void UpdateMenus();

public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	virtual ~MyFrame();

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnClose(wxCommandEvent& event);
	void OnInfo(wxCommandEvent& event);
	void OnExtract(wxCommandEvent& event);
	void OnXMLInfo(wxCommandEvent& event);
    void OnShowSearchBar(wxCommandEvent& event);
    void OnShowTree(wxCommandEvent& event);
    void OnShowProperty(wxCommandEvent& event);
    void OnSearch(wxCommandEvent& event);
    void OnSearchCancel(wxCommandEvent& event);
    void OnEnter(wxCommandEvent& event);
    void OnText(wxCommandEvent& event);

	void OnPaneClose(wxAuiManagerEvent& event);

    DECLARE_EVENT_TABLE()

	enum {
		ID_Table
	};
};

#endif /* __GRFEXPLORER_H */