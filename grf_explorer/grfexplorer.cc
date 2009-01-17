#include "grfexplorer.h"

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_Quit, MyFrame::OnQuit)
    EVT_MENU(ID_About, MyFrame::OnAbout)
    EVT_MENU(ID_GRF_Open, MyFrame::OnOpen)
END_EVENT_TABLE()


MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame((wxFrame *)NULL, -1, title, pos, size) {
    wxMenu *menuFile;
	wxMenuBar *menuBar = new wxMenuBar;

	// GRF
    menuFile = new wxMenu;
	menuFile->Append( ID_GRF_Open, _T("Open...") );
    menuFile->Append( ID_GRF_Close, _T("Close") );
    menuFile->AppendSeparator();
    menuFile->Append( ID_Quit, _T("E&xit") );

    menuBar->Append( menuFile, _T("&File") );

	// Help
	menuFile = new wxMenu;
    menuFile->Append( ID_About, _T("&About...") );
	menuBar->Append( menuFile, _T("&Help") );

    SetMenuBar( menuBar );

    CreateStatusBar();
    SetStatusText( _T("Welcome to wxWindows!") );

	table = new GRFList(this, ID_Table);
	//grid->CreateGrid(5,4,wxGrid::wxGridSelectRows);
	//grid->EnableEditing(false);
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
    Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
	wxMessageBox(_T("Open-Ragnarok GRF Explorer\nhttp://www.open-ragnarok.org"),
        _T("About GRF Explorer"), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnOpen(wxCommandEvent& WXUNUSED(event)) {
	wxFileDialog * openFileDialog = new wxFileDialog(this);
	if (openFileDialog->ShowModal() == wxID_OK){
		wxString fileName = openFileDialog->GetPath();
		if (!m_grf.open(fileName.c_str().AsChar())) {
			wxMessageDialog *dial = new wxMessageDialog(NULL, wxString::Format("Erorr loading file %s", fileName),
				wxT("Error"), wxOK | wxICON_ERROR);
			dial->ShowModal();
			return;
		}
		table->LoadGRF(&m_grf);
	}
}