#include "grflist.h"

#include <algorithm>
//#include <math.h>
#include <set>
#include <map>
#include <vector>
#include <wx/tokenzr.h>

#include "up_arrow.xpm"
#include "down_arrow.xpm"

BEGIN_EVENT_TABLE(GRFList, wxListCtrl)
	EVT_LIST_ITEM_SELECTED(ID_List, GRFList::OnItemSelected)
    EVT_LIST_ITEM_RIGHT_CLICK(ID_List, GRFList::OnItemRightClick)
    EVT_LIST_COL_CLICK(ID_List, GRFList::OnColClick)
    EVT_LIST_COL_RIGHT_CLICK(ID_List, GRFList::OnColRightClick)
//    EVT_CONTEXT_MENU(GRFList::OnContextMenu)
    EVT_MENU(100 + 0, GRFList::OnColumnMenu)
    EVT_MENU(100 + 1, GRFList::OnColumnMenu)
    EVT_MENU(100 + 2, GRFList::OnColumnMenu)
    EVT_MENU(100 + 3, GRFList::OnColumnMenu)
    EVT_MENU(100 + 4, GRFList::OnColumnMenu)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(GRFTree, wxTreeCtrl)
//    EVT_MENU(100 + 0, GRFTree::OnColumnMenu)
//    EVT_MENU(100 + 1, GRFTree::OnColumnMenu)
//    EVT_MENU(100 + 2, GRFTree::OnColumnMenu)
//    EVT_MENU(100 + 3, GRFTree::OnColumnMenu)
//    EVT_MENU(100 + 4, GRFTree::OnColumnMenu)
    EVT_TREE_SEL_CHANGED(ID_Tree, GRFTree::OnSelChanged)
    EVT_TREE_ITEM_MENU(ID_Tree, GRFTree::OnItemMenu)
    EVT_CONTEXT_MENU(GRFTree::OnContextMenu)
END_EVENT_TABLE()

#define HEADER_COUNT 6
char *grflist_headers[HEADER_COUNT] = {
	"idx", 
	"size", 
	"compressed", 
	"flags", 
	"filename",
	"ext"
};

std::vector<ListItem> g_internalList;

GRFList::GRFList(wxWindow *parent, const wxWindowID id) 
	: wxListCtrl(parent, id, wxDefaultPosition, wxDefaultSize, wxLC_VIRTUAL | wxLC_REPORT | wxLC_SINGLE_SEL)
	, m_sortCol(0)
	, m_sortAscending(true)
	, m_imageList(16, 16)
	, m_enabledWildcard(true)
	, m_enabledIncSearch(false)
	, m_root(true)
	, m_fileList(m_emptyList)
{
    wxListItem itemCol;
	unsigned int i;

	m_grf = NULL;

	itemCol.SetAlign(wxLIST_FORMAT_RIGHT);
	for (i = 0; i < HEADER_COUNT; i++) {
		itemCol.SetText(wxString::FromUTF8(grflist_headers[i]));
		if (i == 4)
			itemCol.SetAlign(wxLIST_FORMAT_LEFT);
		InsertColumn(i, itemCol);
	}
	SetColumnWidth(0, 60);
	SetColumnWidth(1, 60);
	SetColumnWidth(2, 60);
	SetColumnWidth(3, 40);
	SetColumnWidth(4, 400);
	SetColumnWidth(5, 40);

	m_imageList.Add(wxBitmap(sort_asc_xpm), wxColor(255,0,0)); 
	m_imageList.Add(wxBitmap(sort_desc_xpm), wxColor(255,0,0)); 

	SetImageList(&m_imageList, wxIMAGE_LIST_SMALL); 

	for (int i = 0; i < HEADER_COUNT; i++) {
		m_columnMenu.Append(100 + i, grflist_headers[i], "", true);
		m_columnMenu.Check(100 + i, true);
	}
}

void GRFList::SetColumnImage(int col, int image) 
{ 
	wxListItem item; 
	item.SetMask(wxLIST_MASK_IMAGE); 
	item.SetImage(image); 
	SetColumn(col, item); 
} 

void GRFList::SetInternalList(std::vector<ListItem>& internalList) {
	m_internalList = internalList;
	g_internalList = internalList;
}

void GRFList::Clear()
{
	SetItemCount(0);
	Refresh();
	m_internalList.clear();
	m_grf = NULL;

	for (int i = 0; i < HEADER_COUNT; i++)
		SetColumnImage(i, -1); 
	m_sortCol = 0;
	m_sortAscending = true;
}

void GRFList::AssignList(std::vector<int> &fileList, bool root) {
	m_fileList = fileList;
	SetItemCount(fileList.size());

	m_root = root;

	m_searchList.clear(); // TODO
	m_searchText = ""; // TODO

	if (m_sortCol != 0)
		SortColumn(m_fileList);
	if (!m_sortAscending)
		std::reverse(m_fileList.begin(), m_fileList.end());

	Refresh();
}

wxString GRFList::OnGetItemText(long index, long column) const
{
	if (m_searchList.size() > 0)
		index = m_searchList[index];
	else if (m_fileList.size() > 0)
		index = m_fileList[index];

	switch (column) {
	case 0:
		return wxString::Format(_T("%d"), m_internalList[index].index);
	case 1:
		return wxString::Format(_T("%d"), m_internalList[index].size);
	case 2:
		return wxString::Format(_T("%d"), m_internalList[index].compressedSize);
	case 3:
		return wxString::Format(_T("0x%x"), m_internalList[index].flags);
	case 4:
		if (m_root)
			return m_internalList[index].filename;
		else
			return wxFileNameFromPath(m_internalList[index].filename);
	case 5:
		return m_internalList[index].ext;
	}
}

void GRFList::OnItemSelected(wxListEvent &event)
{
	int a = event.GetIndex();
	int b = event.GetId();
//	wxMessageBox(wxString::Format("dialog %d %d", a, b));
}

void GRFList::OnItemRightClick(wxListEvent &event)
{
	if (GetEditControl() == NULL)
	{
		wxPoint point = event.GetPoint();
		// If from keyboard
		if ( (point.x == -1) && (point.y == -1) )
		{
			wxSize size = GetSize();
			point.x = size.x / 2;
			point.y = size.y / 2;
		}
		else
		{
		//	point = ScreenToClient(point);
		}
		ShowContextMenu(point);
	}
	else
	{
		// the user is editing:
		// allow the text control to display its context menu
		// if it has one (it has on Windows) rather than display our one
		event.Skip();
	}
}

void GRFList::ShowContextMenu(const wxPoint& pos)
{
    wxMenu menu;

    menu.Append(wxID_ABOUT, wxT("&Extract"));
    menu.Append(wxID_ABOUT, wxT("Extract With &Directories"));
    menu.AppendSeparator();
    menu.Append(wxID_EXIT, wxT("Extract &All"));
    menu.Append(wxID_ABOUT, wxT("Extract All With &Directories"));

    PopupMenu(&menu, pos.x, pos.y);
}

#define USE_FUNCTOR 0
#if !USE_FUNCTOR
#	if 0
bool CompareIndex(const ListItem &item1, const ListItem &item2) { return item1.index < item2.index; }
bool CompareSize(const ListItem &item1, const ListItem &item2) { return item1.size < item2.size; }
bool CompareCompressedSize(const ListItem &item1, const ListItem &item2) { return item1.compressedSize < item2.compressedSize; }
bool CompareFlags(const ListItem &item1, const ListItem &item2) { return item1.flags < item2.flags; }
bool CompareFilename(const ListItem &item1, const ListItem &item2) { return item1.filename < item2.filename; }
#	else
bool CompareIndex(const int &item1, const int &item2) { return g_internalList[item1].index < g_internalList[item2].index; }
bool CompareSize(const int &item1, const int &item2) { return g_internalList[item1].size < g_internalList[item2].size; }
bool CompareCompressedSize(const int &item1, const int &item2) { return g_internalList[item1].compressedSize < g_internalList[item2].compressedSize; }
bool CompareFlags(const int &item1, const int &item2) { return g_internalList[item1].flags < g_internalList[item2].flags; }
bool CompareFilename(const int &item1, const int &item2) { return g_internalList[item1].filename < g_internalList[item2].filename; }
bool CompareExt(const int &item1, const int &item2) { return g_internalList[item1].ext < g_internalList[item2].ext; }
#	endif
#else
struct CompareIndex { bool operator()(const ListItem &item1, const ListItem &item2) { return item1.index < item2.index; } };
struct CompareSize { bool operator()(const ListItem &item1, const ListItem &item2) { return item1.size < item2.size; } };
struct CompareCompressedSize { bool operator()(const ListItem &item1, const ListItem &item2) { return item1.compressedSize < item2.compressedSize; } };
struct CompareFlags { bool operator()(const ListItem &item1, const ListItem &item2) { return item1.flags < item2.flags; } };
struct CompareFilename { bool operator()(const ListItem &item1, const ListItem &item2) { return item1.filename < item2.filename; } };
#endif

//void GRFList::SortColumn() {
void GRFList::SortColumn(std::vector<int> &list) {
#if !USE_FUNCTOR
#	if 0
		if (m_sortCol == 0)
			std::sort(m_internalList.begin(), m_internalList.end(), CompareIndex);
		else if (m_sortCol == 1)
			std::sort(m_internalList.begin(), m_internalList.end(), CompareSize);
 		else if (m_sortCol == 2)
			std::sort(m_internalList.begin(), m_internalList.end(), CompareCompressedSize);
 		else if (m_sortCol == 3)
			std::sort(m_internalList.begin(), m_internalList.end(), CompareFlags);
 		else if (m_sortCol == 4)
			std::sort(m_internalList.begin(), m_internalList.end(), CompareFilename);
#	else
/*		if (m_sortCol == 0)
			std::sort(m_fileList.begin(), m_fileList.end(), CompareIndex);
		else if (m_sortCol == 1)
			std::sort(m_fileList.begin(), m_fileList.end(), CompareSize);
 		else if (m_sortCol == 2)
			std::sort(m_fileList.begin(), m_fileList.end(), CompareCompressedSize);
 		else if (m_sortCol == 3)
			std::sort(m_fileList.begin(), m_fileList.end(), CompareFlags);
 		else if (m_sortCol == 4)
			std::sort(m_fileList.begin(), m_fileList.end(), CompareFilename);
 		else if (m_sortCol == 5)
			std::sort(m_fileList.begin(), m_fileList.end(), CompareExt);*/
		if (m_sortCol == 0)
			std::sort(list.begin(), list.end(), CompareIndex);
		else if (m_sortCol == 1)
			std::sort(list.begin(), list.end(), CompareSize);
 		else if (m_sortCol == 2)
			std::sort(list.begin(), list.end(), CompareCompressedSize);
 		else if (m_sortCol == 3)
			std::sort(list.begin(), list.end(), CompareFlags);
 		else if (m_sortCol == 4)
			std::sort(list.begin(), list.end(), CompareFilename);
 		else if (m_sortCol == 5)
			std::sort(list.begin(), list.end(), CompareExt);
#	endif
#else
		if (m_sortCol == 0)
			std::sort(m_internalList.begin(), m_internalList.end(), CompareIndex());
		else if (m_sortCol == 1)
			std::sort(m_internalList.begin(), m_internalList.end(), CompareSize());
 		else if (m_sortCol == 2)
			std::sort(m_internalList.begin(), m_internalList.end(), CompareCompressedSize());
 		else if (m_sortCol == 3)
			std::sort(m_internalList.begin(), m_internalList.end(), CompareFlags());
 		else if (m_sortCol == 4)
			std::sort(m_internalList.begin(), m_internalList.end(), CompareFilename());
#endif
}

void GRFList::OnColClick(wxListEvent& event)
{
	if (m_fileList.size() == 0)
		return;

    int col = event.GetColumn();

	std::vector<int> &list = (m_searchList.size() > 0) ? m_searchList : m_fileList;
	if (m_sortCol != col) {
		SetColumnImage(m_sortCol, -1);
		m_sortCol = col;
		m_sortAscending = true;

		SetColumnImage(m_sortCol, 0); 

		SortColumn(list);
	}
	else {
		m_sortAscending = !m_sortAscending;
		SetColumnImage(m_sortCol, m_sortAscending ? 0 : 1); 
	//	std::reverse(m_internalList.begin(), m_internalList.end());
	//	std::reverse(m_fileList.begin(), m_fileList.end());
		std::reverse(list.begin(), list.end());
	}

	Refresh();
}

void GRFList::OnColRightClick(wxListEvent& event)
{
	PopupMenu(&m_columnMenu, event.GetPoint());
}

void GRFList::OnColumnMenu(wxCommandEvent& event) {
	int id = event.GetId();
	bool checked = m_columnMenu.IsChecked(id);
	wxListItem item;
	item.SetText(grflist_headers[id - 100]);
	if (!checked)
		DeleteColumn(id - 100);
	else {
		InsertColumn(id - 100, item);
		wxArrayInt order;
		order.Alloc(5);
		order.Add(1);
		order.Add(0);
		order.Add(2);
		order.Add(3);
		order.Add(4);
	}
}

int GRFList::OnGetItemImage(long item) const
{
	return -1;
}

unsigned int GRFList::Search(wxString searchText) {
	if (m_internalList.size() == 0)
		return 0;

	if (m_searchText == searchText)
		return m_searchList.size();

	m_searchText = searchText;
	m_searchList.clear();

	SetItemCount(0);
	Refresh();

	if (m_fileList.size() == 0) {
		if (m_enabledWildcard && wxIsWild(searchText)) {
			for (int i = 0; i < m_internalList.size(); i++) {
				if (m_internalList[i].filename.Matches(searchText)) {
					m_searchList.push_back(i);
				}
			}
		}
		else {
			for (int i = 0; i < m_internalList.size(); i++) {
				if (m_internalList[i].filename.Contains(searchText)) {
					m_searchList.push_back(i);
				}
			}
		}
	}
	else {
		if (m_root) {
			if (m_enabledWildcard && wxIsWild(searchText)) {
				for (int i = 0; i < m_fileList.size(); i++) {
					int idx = m_fileList[i];
					if (m_internalList[idx].filename.Matches(searchText)) {
						m_searchList.push_back(idx);
					}
				}
			}
			else {
				for (int i = 0; i < m_fileList.size(); i++) {
					int idx = m_fileList[i];
					if (m_internalList[idx].filename.Contains(searchText)) {
						m_searchList.push_back(idx);
					}
				}
			}
		}
		else {
			if (m_enabledWildcard && wxIsWild(searchText)) {
				for (int i = 0; i < m_fileList.size(); i++) {
					int idx = m_fileList[i];
					if (wxFileNameFromPath(m_internalList[idx].filename).Matches(searchText)) {
						m_searchList.push_back(idx);
					}
				}
			}
			else {
				for (int i = 0; i < m_fileList.size(); i++) {
					int idx = m_fileList[i];
					if (wxFileNameFromPath(m_internalList[idx].filename).Contains(searchText)) {
						m_searchList.push_back(idx);
					}
				}
			}
		}
	}
	if (m_searchList.size()) {
		SetItemCount(m_searchList.size());
		Refresh();
	}

	return m_searchList.size();
}


// GRFTree

//GRFTree::GRFTree(wxWindow *parent, const wxWindowID id) 
GRFTree::GRFTree(wxWindow *parent, const wxWindowID id, wxStatusBar* statusBar) 
	: wxTreeCtrl(parent, id, wxDefaultPosition, wxSize(300, 200), wxTR_DEFAULT_STYLE | wxNO_BORDER)
	, m_statusBar(statusBar)
{
}


bool GRFTree::LoadGRF(ro::GRF* grf, wxString &fn) {
//bool GRFList::LoadGRF(ro::GRF* grf, wxStatusBar* statusBar) {
	unsigned int i;
	long tmp;
	wxListItem itemCol;
	m_grf = grf;

	wxProgressDialog diag(_T("Loading GRF"), _T("Reading file index..."), grf->getCount(), this->GetParent(), wxPD_CAN_ABORT | wxPD_AUTO_HIDE | wxPD_APP_MODAL
//	wxProgressDialog diag(_T("Loading GRF"), _T("Reading file index..."), 100, this->GetParent(), wxPD_CAN_ABORT | wxPD_AUTO_HIDE | wxPD_APP_MODAL
                           | wxPD_ELAPSED_TIME
                           | wxPD_ESTIMATED_TIME
                           | wxPD_REMAINING_TIME
                           | wxPD_SMOOTH // - makes indeterminate mode bar on WinXP very small
		);

	m_internalList.resize(grf->getCount());

	std::map<wxString, std::vector<int>> pathList;
	float step = (grf->getCount() / 100.f);
	float cur = step;
	wxCSConv euckrconv(wxT("EUC-KR"));

	std::vector<int> allFileList;
	for (i = 0; i < grf->getCount(); i++) {
		const ro::GRF::FileTableItem& item = grf->getItem(i);
		ListItem &listitem = m_internalList[i];
		listitem.index = i;
		listitem.size = item.uncompressedLength;
		listitem.compressedSize = item.compressedLengthAligned;
		listitem.flags = item.flags;
		listitem.filename = wxString(item.filename, euckrconv);
		wxString path, fn, ext;
		wxSplitPath(listitem.filename, &path, &fn, &ext);
		listitem.ext = ext;
		std::vector<int> &fileList = pathList[path];
		fileList.push_back(i);

		allFileList.push_back(i);

		if (i == (int)(cur + 0.5f)) {
			cur += step;
		if (!diag.Update(i+1)) {
			//	Clear();
			return(false);
		}
	}
	}

	m_listCtrl->SetInternalList(m_internalList);

	wxTreeItemId rootId = AddRoot(wxFileNameFromPath(fn), -1, -1, new MyTreeItemData(allFileList));
//	wxTreeItemId idParent = rootId;
	std::vector<int> emptyList;

	for (std::map<wxString, std::vector<int>>::iterator itr = pathList.begin();
		itr != pathList.end(); 
		itr++) 
	{
		diag.Pulse();
		const wxString &path = itr->first;
		std::vector<int> &fileList = itr->second;

		wxStringTokenizer tkz(path, "\\");
		wxTreeItemId _id = rootId;
		while ( tkz.HasMoreTokens() )
		{
		//	Expand(_id);
			wxString &token = tkz.GetNextToken();

			if (GetChildrenCount(_id, false) == 0) {
				if ( tkz.HasMoreTokens() )
					_id = AppendItem(_id, token, -1, -1, new MyTreeItemData(emptyList));
				else
					_id = AppendItem(_id, token, -1, -1, new MyTreeItemData(fileList));
			}
			else {
				wxTreeItemIdValue cookie;
				wxTreeItemId childId = GetFirstChild(_id, cookie);
				bool find = false;
				for (int i = 0; i < GetChildrenCount(_id, false); i++) {
					if (token == GetItemText(childId)) {
						find = true;
						break;
					}
					childId = GetNextChild(childId, cookie);
				}
				if (find)
					_id = childId;
				else {
					if ( tkz.HasMoreTokens() )
						_id = AppendItem(_id, token, -1, -1, new MyTreeItemData(emptyList));
					else
						_id = AppendItem(_id, token, -1, -1, new MyTreeItemData(fileList));
				}
			}
		}
	}
	SelectItem(rootId);

	return(true);
}

void GRFTree::Clear()
{
	DeleteAllItems();
//	Refresh();
	m_grf = NULL;
}

void GRFTree::OnSelChanged(wxTreeEvent& event)
{
    // show some info about this item
    wxTreeItemId itemId = event.GetItem();
    MyTreeItemData *item = (MyTreeItemData *)GetItemData(itemId);

	m_listCtrl->SetItemCount(0);
	m_listCtrl->Refresh();

    if ( item != NULL )
    {
//        item->ShowInfo(this);
		m_listCtrl->AssignList(item->GetFileList(), itemId == GetRootItem());
    }
	else
	{
	/*	if (itemId == GetRootItem())
			m_listCtrl->AssignList(NULL);
		else
			m_listCtrl->SetItemCount(0);*/
		//	m_listCtrl->AssignList(NULL, itemId == GetRootItem());
	}

	m_statusBar->SetStatusText(wxString::Format(_T("Current: %d"), item->GetFileList().size()), 1);
}

void GRFTree::OnItemMenu(wxTreeEvent& event)
{
	wxTreeItemId itemId = event.GetItem();
	MyTreeItemData *item = itemId.IsOk() ? (MyTreeItemData *)GetItemData(itemId)
										 : NULL;
	wxPoint clientpt = event.GetPoint();
	wxPoint screenpt = ClientToScreen(clientpt);

	ShowMenu(itemId, clientpt);
	event.Skip();
}

void GRFTree::OnContextMenu(wxContextMenuEvent& event)
{
//	if (GetRootItem().IsOk() && GetChildrenCount(GetRootItem()) == 0) {
	if (m_grf != NULL) {
		event.Skip();
		return;
	}

	wxPoint pt = event.GetPosition();
	wxPoint clientpt = ScreenToClient(pt);

	static const wxTreeItemId itemId;

	ShowMenu(itemId, clientpt);

//	event.Skip();
}

void GRFTree::ShowMenu(wxTreeItemId id, const wxPoint& pt)
{
//	wxString title;
	wxMenu menu;
	if ( id.IsOk() )
	{
	//    title << wxT("Menu for ") << GetItemText(id);
		menu.Append(1, wxT("&About..."));
		menu.AppendSeparator();
		menu.Append(2, wxT("&Highlight item"));
		menu.Append(3, wxT("&Dump"));
	}
	else
	{
	 //   title = wxT("Menu for no particular item");
	}
	menu.Append(1, wxT("&Expand All"));
	menu.Append(2, wxT("&Collapse All"));

	PopupMenu(&menu, pt);
}
