#include "grfexplorer.h"

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_Quit, MyFrame::OnQuit)
    EVT_MENU(ID_About, MyFrame::OnAbout)
    EVT_MENU(ID_GRF_Open, MyFrame::OnOpen)
	EVT_MENU(ID_GRF_Close, MyFrame::OnClose)
END_EVENT_TABLE()


MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame((wxFrame *)NULL, -1, title, pos, size) {
    wxMenu *menuFile;

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

    menuBar.Append( menuFile, _T("&GRF") );

	// Help
	menuFile = new wxMenu;
    menuFile->Append( ID_About, _T("&About...") );
	menuBar.Append( menuFile, _T("&Help") );

    SetMenuBar(&menuBar);

    CreateStatusBar();
    SetStatusText( _T("Welcome to wxWindows!") );

	table = new GRFList(this, ID_Table);
	//grid->CreateGrid(5,4,wxGrid::wxGridSelectRows);
	//grid->EnableEditing(false);

	UpdateMenus();
}

MyFrame::~MyFrame() {
	if (m_grf.isOpen())
		m_grf.close();
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
    Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
	wxMessageBox(wxString::Format(_T("Open-Ragnarok GRF Explorer\nROInt version %s\nhttp://www.open-ragnarok.org"), wxString::FromUTF8(RO::strVersion())),
        _T("About GRF Explorer"), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnOpen(wxCommandEvent& WXUNUSED(event)) {
	wxFileDialog * openFileDialog = new wxFileDialog(this);
	if (openFileDialog->ShowModal() == wxID_OK){
		wxString fileName = openFileDialog->GetPath();
		std::string fn = fileName.utf8_str();
		if (!m_grf.open(fn)) {
			wxMessageDialog *dial = new wxMessageDialog(NULL, wxString::Format(L"Error loading file %s", fileName),
				wxT("Error"), wxOK | wxICON_ERROR);
			dial->ShowModal();
			return;
		}
		if (!table->LoadGRF(&m_grf)) {
			m_grf.close();
			wxMessageDialog *dial = new wxMessageDialog(NULL, wxString::Format(L"Error loading file %s", fileName),
				wxT("Error"), wxOK | wxICON_ERROR);
			dial->ShowModal();
			return;
		}
	}
	UpdateMenus();
}

void MyFrame::OnClose(wxCommandEvent& WXUNUSED(event)) {
	m_grf.close();
	table->ClearAll();
	UpdateMenus();
}

void MyFrame::OnInfo(wxCommandEvent& event) {
	if (!m_grf.isOpen()) {
		wxMessageDialog *dial = new wxMessageDialog(NULL, _T("No GRF file opened."),
			wxT("Error"), wxOK | wxICON_ERROR);
		dial->ShowModal();
		return;
	}

}

void MyFrame::UpdateMenus() {
	bool x = m_grf.isOpen();

	menuBar.Enable(ID_GRF_Open, !x);
	menuBar.Enable(ID_GRF_Close, x);
	menuBar.Enable(ID_GRF_Info, x);
}
