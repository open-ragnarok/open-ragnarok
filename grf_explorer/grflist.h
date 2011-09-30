#ifndef __GRFLIST_H
#define __GRFLIST_H

#include <vector>

#include "wx_support.h"
#include <wx/imaglist.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>

#include "ro/ro.h"

enum {
    ID_List = 1,
	ID_Tree,
	ID_List_Menu_Extract,
	ID_List_Menu_ExtractWithDirs,
	ID_List_Menu_ExtractAll,
	ID_List_Menu_ExtractAllWithDirs,
};

struct ListItem {
	int index;
	int size;
	int compressedSize;
	int flags;
	wxString filename;
	wxString ext;
};

class GRFList : public wxListCtrl {
protected:
	ro::GRF* m_grf;
public:
    GRFList(wxWindow *parent, const wxWindowID id);

	void SetColumnImage(int col, int image);
	void SetInternalList(std::vector<ListItem>& internalList);
	void AssignList(std::vector<int> &fileList, bool root = false);

	void Clear();
	unsigned int Search(wxString searchText);

private:
	bool m_enabledWildcard;
	bool m_enabledIncSearch;
	wxImageList m_imageList;
	int m_sortCol;
	bool m_sortAscending;
	std::vector<ListItem> m_internalList;
	std::vector<int> m_searchList;
	std::vector<int> m_emptyList;
	std::vector<int> m_fileList;
	bool m_root;
	wxString m_searchText;
	wxMenu m_columnMenu;

    void ShowContextMenu(const wxPoint& pos);
	void SortColumn(std::vector<int> &list);

	wxString OnGetItemText(long index, long column) const;
	void OnItemSelected(wxListEvent &event);
	void OnItemRightClick(wxListEvent &event);
    void OnColClick(wxListEvent& event);
    void OnColRightClick(wxListEvent& event);
    int OnGetItemImage(long item) const;
    void OnColumnMenu(wxCommandEvent& event);

	DECLARE_NO_COPY_CLASS(GRFList)
	DECLARE_EVENT_TABLE()
};

class MyTreeItemData : public wxTreeItemData
{
public:
    MyTreeItemData(std::vector<int>& fileList) : m_fileList(fileList) { }

//    void ShowInfo(wxTreeCtrl *tree);
    std::vector<int>& GetFileList() { return m_fileList; }

private:
    std::vector<int> m_fileList;
};

class GRFTree : public wxTreeCtrl {
public:
//	GRFTree(wxWindow *parent, const wxWindowID id);
	GRFTree(wxWindow *parent, const wxWindowID id, wxStatusBar* statusBar);

	void SetListCtrl(GRFList* list) { m_listCtrl = list; }

	bool LoadGRF(ro::GRF* grf, wxString &fn);
	void Clear();
private:
	wxStatusBar* m_statusBar;

	ro::GRF* m_grf;
	GRFList* m_listCtrl;
	std::vector<ListItem> m_internalList;
	std::vector<int> m_searchList;

	void ShowMenu(wxTreeItemId id, const wxPoint& pt);

    void OnSelChanged(wxTreeEvent& event);
	void OnContextMenu(wxContextMenuEvent& event);
	void OnItemMenu(wxTreeEvent& event);
	
	DECLARE_NO_COPY_CLASS(GRFTree)
	DECLARE_EVENT_TABLE()
};

#endif /* __GRFLIST_H */
