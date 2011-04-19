#include "grfexplorer.h"

#include <wx/srchctrl.h>
#include <wx/splitter.h>
#include <wx/propgrid/propgrid.h>

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_Quit, MyFrame::OnQuit)
    EVT_MENU(ID_About, MyFrame::OnAbout)
    EVT_MENU(ID_GRF_Open, MyFrame::OnOpen)
	EVT_MENU(ID_GRF_Close, MyFrame::OnClose)
	EVT_MENU(ID_GRF_Extract, MyFrame::OnExtract)
	EVT_MENU(ID_GRF_Info, MyFrame::OnInfo)
	EVT_MENU(ID_GRF_XMLInfo, MyFrame::OnXMLInfo)
    EVT_MENU(ID_View_SearchBar, MyFrame::OnShowSearchBar)
    EVT_MENU(ID_View_Tree, MyFrame::OnShowTree)
    EVT_MENU(ID_View_Property, MyFrame::OnShowProperty)
	EVT_SEARCHCTRL_SEARCH_BTN(ID_Search, MyFrame::OnSearch)
	EVT_SEARCHCTRL_CANCEL_BTN(ID_Search, MyFrame::OnSearchCancel)
	EVT_TEXT_ENTER(ID_Search, MyFrame::OnEnter)
	EVT_TEXT(ID_Search, MyFrame::OnText)
    EVT_AUI_PANE_CLOSE(MyFrame::OnPaneClose)
END_EVENT_TABLE()


MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame((wxFrame *)NULL, -1, title, pos, size) {
	m_auimgr.SetManagedWindow(this);

    wxMenu *menuFile;

	menuBar = new wxMenuBar;

	// GRF
    menuFile = new wxMenu;
	menuFile->Append( ID_GRF_Open, _T("Open...") );
    menuFile->Append( ID_GRF_Close, _T("Close") );
    menuFile->Append( ID_GRF_Info, _T("Info...") );
    menuFile->AppendSeparator();
	menuFile->Append(ID_GRF_Extract, _T("Extract"));
	menuFile->Append(ID_GRF_XMLInfo, _T("XML Info"));
	menuFile->AppendSeparator();
    menuFile->Append( ID_Quit, _T("E&xit") );
	menuBar->Append( menuFile, _T("&GRF") );

	// View
	menuFile = new wxMenu;
	menuFile->Append( ID_View_SearchBar, _T("&Search Bar"), wxEmptyString, true);
	menuFile->Append( ID_View_Tree, _T("&Tree"), wxEmptyString, true);
	menuFile->Append( ID_View_Property, _T("&Property"), wxEmptyString, true);
	menuFile->AppendSeparator();
	menuFile->Append( ID_View_Property, _T("Auto Show Property") );
	menuFile->Check(ID_View_SearchBar, true);
	menuFile->Check(ID_View_Tree, true);
	menuFile->Check(ID_View_Property, true);
	menuBar->Append( menuFile, _T("&View") );

	// Help
	menuFile = new wxMenu;
    menuFile->Append( ID_About, _T("&About...") );
	menuBar->Append( menuFile, _T("&Help") );

	SetMenuBar(menuBar);

    CreateStatusBar();
//	statusBar = CreateStatusBar();
    SetStatusText( _T("Welcome to Open-Ragnarok GRF Explorer!") );

	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	wxPanel *panel = new wxPanel(this, wxID_ANY);
	wxPanel *panel2 = new wxPanel(panel, wxID_ANY);
	searchCtrl = new wxSearchCtrl(panel2, ID_Search, wxEmptyString,
                             wxDefaultPosition, wxDefaultSize,
                             wxTE_PROCESS_ENTER);
	searchCtrl->ShowSearchButton(true);

	menuFile = new wxMenu;
    menuFile->Append(ID_Search_Incremental, _T("&Incremental Search"), wxEmptyString, true);
    menuFile->Append(ID_Search_Wildcard, _T("&Wild Card"), wxEmptyString, true);
	menuFile->Check(ID_Search_Wildcard, true);
    menuFile->AppendSeparator();
    menuFile->Append(ID_Search_Recents, _T("&Recent searches..."));
	menuFile->Enable(ID_Search_Recents, false);
	searchCtrl->SetMenu(menuFile);

	table = new GRFList(panel, ID_List);

	hbox->Add(searchCtrl, 1, wxEXPAND);
	panel2->SetSizer(hbox);

	vbox->Add(panel2, 0, wxEXPAND);
	vbox->Add(table, 1, wxEXPAND | wxTOP, 4);
	panel->SetSizer(vbox);
	
//	tree = new GRFTree(this, ID_Tree);
	tree = new GRFTree(this, ID_Tree, GetStatusBar());

	wxPropertyGrid* pg = new wxPropertyGrid(
		this, // parent
		ID_PropGrid, // id
		wxDefaultPosition, // position
	//	wxDefaultSize, // size
		wxSize(300, 200), // size
		// Here are just some of the supported window styles
		wxPG_AUTO_SORT | // Automatic sorting after items added
		wxPG_SPLITTER_AUTO_CENTER | // Automatically center splitter until user manually adjusts it
		wxPG_TOOLTIPS |
		wxPG_TOOLBAR |
		wxNO_BORDER |
		// Default style
		wxPG_DEFAULT_STYLE );

    // Add int property
    pg->Append( new wxIntProperty("IntProperty", wxPG_LABEL, 12345678) );

    // Add float property (value type is actually double)
    pg->Append( new wxFloatProperty("FloatProperty", wxPG_LABEL, 12345.678) );

    // Add a bool property
    pg->Append( new wxBoolProperty("BoolProperty", wxPG_LABEL, false) );

    // A string property that can be edited in a separate editor dialog.
    pg->Append( new wxLongStringProperty("LongStringProperty",
                                         wxPG_LABEL,
                                         "This is much longer string than the "
                                         "first one. Edit it by clicking the button."));

    // String editor with dir selector button.
    pg->Append( new wxDirProperty("DirProperty", wxPG_LABEL, ::wxGetUserHome()) );

    // wxArrayStringProperty embeds a wxArrayString.
    pg->Append( new wxArrayStringProperty("Label of ArrayStringProperty",
                                          "NameOfArrayStringProp"));

    // A file selector property.
    pg->Append( new wxFileProperty("FileProperty", wxPG_LABEL, wxEmptyString) );

    // Extra: set wild card for file property (format same as in wxFileDialog).
    pg->SetPropertyAttribute( "FileProperty",
                              wxPG_FILE_WILDCARD,
                              "All files (*.*)|*.*" );

	m_auimgr.AddPane(tree, 
		wxAuiPaneInfo().
		Name(wxT("tree")).
		Caption(wxT("Tree")).
		Left().
	//	PaneBorder(false).
	//	BestSize(200, 400).
		Layer(1));

	m_auimgr.AddPane(pg, 
		wxAuiPaneInfo().
		Name(wxT("property")).
		Caption(wxT("Property")).
		Left().
	//	PaneBorder(false).
	//	BestSize(200, 400).
		Layer(1));

	m_auimgr.AddPane(panel, 
		wxAuiPaneInfo().
		Name(wxT("list")).
		CaptionVisible( false ).
		Center().
		PaneBorder(false));

	tree->SetListCtrl(table);

//	table->SetFocus();
	tree->SetFocus();

	m_auimgr.SetFlags(m_auimgr.GetFlags() | wxAUI_MGR_ALLOW_ACTIVE_PANE);
	m_auimgr.Update();

	UpdateMenus();

	SetSize(800, 600);
}

MyFrame::~MyFrame() {
	m_auimgr.UnInit();

	if (m_grf.isOpen())
		m_grf.close();
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
    Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
	wxMessageBox(wxString::Format(_T("Open-Ragnarok GRF Explorer\nROInt version %s\nhttp://www.open-ragnarok.org"), wxString::FromUTF8(ro::strVersion())),
        _T("About GRF Explorer"), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnOpen(wxCommandEvent& WXUNUSED(event)) {
	wxFileDialog openFileDialog(this);
	openFileDialog.SetWildcard("GRF Files|*.grf|All Files|*.*");
	if (openFileDialog.ShowModal() == wxID_OK){
		wxString fileName = openFileDialog.GetPath();
		std::string fn = fileName.utf8_str();
		if (!m_grf.open(fn)) {
			wxMessageDialog dial(NULL, wxString::Format(L"Error loading file %s", fileName),
				wxT("Error"), wxOK | wxICON_ERROR);
			dial.ShowModal();
			return;
		}
		wxStatusBar *statusBar = GetStatusBar();
		int widths[] = {120, 90, -1};
		statusBar->SetFieldsCount(3, widths);
#if 0
		if (!table->LoadGRF(&m_grf)) {
#else
		if (!tree->LoadGRF(&m_grf, fileName)) {
#endif
			m_grf.close();
			wxMessageDialog dial(NULL, wxString::Format(L"Error loading file %s", fileName),
				wxT("Error"), wxOK | wxICON_ERROR);
			dial.ShowModal();
			return;
		}
		statusBar->SetStatusText(wxString::Format(_T("Entries Total: %d"), m_grf.getCount()), 0);
//		statusBar->SetStatusText(wxString::Format(_T("Current: %d"), m_grf.getCount()), 1);
	}
	UpdateMenus();
}

void MyFrame::OnClose(wxCommandEvent& WXUNUSED(event)) {
	m_grf.close();
	tree->Clear();
	table->Clear();
	UpdateMenus();
	wxStatusBar *statusBar = GetStatusBar();
	statusBar->SetFieldsCount(1);
	SetStatusText( _T("Welcome to Open-Ragnarok GRF Explorer!") );
}

void MyFrame::OnInfo(wxCommandEvent& event) {
	if (!m_grf.isOpen()) {
		wxMessageDialog dial(NULL, _T("No GRF file opened."),
			wxT("Error"), wxOK | wxICON_ERROR);
		dial.ShowModal();
		return;
	}
}

void MyFrame::OnExtract(wxCommandEvent& event) {
	long idx = -1;
	unsigned long x;
	int ptr;
	wxString t;
	idx = table->GetNextItem(idx, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	t = table->GetItemText(idx);
	t.ToULong(&x);

	const ro::GRF::FileTableItem& item = m_grf.getItem(x);
	if (item.flags == 0x02) {
		wxMessageDialog dial(NULL, "Can't extract a directory.",
			wxT("Error"), wxOK | wxICON_ERROR);
		dial.ShowModal();
		return;
	}
	wxFileDialog fileDialog(this, "Save File", "", "", "*.*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	std::string fn = item.filename;
	fileDialog.SetFilename(wxFileNameFromPath(wxString(item.filename, wxCSConv("EUC-KR"))));
	if (fileDialog.ShowModal() == wxID_OK) {
		std::wstring outfn = fileDialog.GetPath().wc_str();
		if (!m_grf.save(item.filename, outfn)) {
			wxMessageDialog dial(NULL, wxString::Format(_T("Failed to save file %s."), wxString(outfn.c_str())),
			wxT("Error"), wxOK | wxICON_ERROR);
			dial.ShowModal();
		}
		else {
		//	wxMessageDialog dial(NULL, wxString::Format(_T("Saved to file %s."), wxString(outfn.c_str())),
		//		wxT("Success"), wxOK | wxICON_INFORMATION);
		//	dial.ShowModal();
		}
	}
}

void MyFrame::OnXMLInfo(wxCommandEvent& event) {
}

void MyFrame::UpdateMenus() {
	bool x = m_grf.isOpen();

	menuBar->Enable(ID_GRF_Open, !x);
	menuBar->Enable(ID_GRF_Close, x);
	menuBar->Enable(ID_GRF_Info,  x);
	menuBar->Enable(ID_GRF_Extract,  x);
	menuBar->Enable(ID_GRF_XMLInfo,  x);
}

void MyFrame::OnShowSearchBar(wxCommandEvent& event) {
	 searchCtrl->Show(event.IsChecked());
	 Layout();
}

void MyFrame::OnShowTree(wxCommandEvent& event) {
	m_auimgr.GetPane(wxT("tree")).Show(event.IsChecked());
	m_auimgr.Update();
}

void MyFrame::OnShowProperty(wxCommandEvent& event) {
	m_auimgr.GetPane(wxT("property")).Show(event.IsChecked());
	m_auimgr.Update();
}

void MyFrame::OnSearch(wxCommandEvent& event) {
}

void MyFrame::OnSearchCancel(wxCommandEvent& event) {
	searchCtrl->SetValue(wxT(""));
	table->Search(wxT(""));
}

void MyFrame::OnEnter(wxCommandEvent& event) {
	int found = table->Search(searchCtrl->GetValue());
	SetStatusText(wxString::Format(wxT("Found: %d"), found), 2);
}

void MyFrame::OnText(wxCommandEvent& event) {
	bool cancel = searchCtrl->GetValue() != wxT("");
	searchCtrl->ShowCancelButton(cancel);
}

void MyFrame::OnPaneClose(wxAuiManagerEvent& event)
{
	if (event.pane->name == wxT("tree"))
	{
		menuBar->Check(ID_View_Tree, false);
	}
	else if (event.pane->name == wxT("property"))
	{
		menuBar->Check(ID_View_Property, false);
	}
}
