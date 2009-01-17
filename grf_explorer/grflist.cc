#include "grflist.h"

char *grflist_headers[] = {
	"idx", "size", "compressed", "flag", "filename"
};

GRFList::GRFList(wxWindow *parent, const wxWindowID id) : wxListCtrl(parent, id, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL) {
    wxListItem itemCol;
	unsigned int i;

	m_grf = NULL;

	itemCol.SetImage(-1);
	for (i = 0; i < 5; i++) {
		InsertColumn(i, grflist_headers[i]);
		SetColumnWidth(i, 80);
	}
}

bool GRFList::LoadGRF(RO::GRF* grf) {
	unsigned int i;
	long tmp;
	wxListItem itemCol;
	m_grf = grf;

	ClearAll();

	for (i = 0; i < 5; i++) {
		InsertColumn(i, grflist_headers[i]);
		SetColumnWidth(i, 80);
	}

	Hide();

	for (i = 0; i < grf->getCount(); i++) {
		itemCol.SetText(wxString::Format(_T("%d"), i));
		// tmp = InsertItem(itemCol);
		tmp = InsertItem(i, wxString::Format(_T("%d"), i));

		const RO::GRF::FileTableItem& item = grf->getItem(i);

		std::string fn = RO::euc2utf8(item.filename);
		SetItem(tmp, 1, wxString::Format(_T("%d"), item.uncompressedLength));
		SetItem(tmp, 2, wxString::Format(_T("%d"), item.compressedLengthAligned));
		SetItem(tmp, 3, wxString::Format(_T("0x%x"), item.flags));
		SetItem(tmp, 4, fn);
	}

	Show();

	return(true);
}