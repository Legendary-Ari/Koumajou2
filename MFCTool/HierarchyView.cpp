// HierarchyView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "HierarchyView.h"
#include "MainFrm.h"
#include "Form.h"
#include "OptionView.h"
#include "MFCToolView.h"

// CHierarchyView

IMPLEMENT_DYNCREATE(CHierarchyView, CTreeView)

CHierarchyView::CHierarchyView()
	:m_bDraging(false)
	, m_bDestroying(false)
	//,ID_HIERARCHY_EDIT_NAME(5001)
	//,ID_HIERARCHY_DELETE(5002)
{

}

CHierarchyView::~CHierarchyView()
{
	for (auto& rPair : m_mapActorInfo)
		Safe_Delete(rPair.second);
	m_mapActorInfo.clear();
}

BEGIN_MESSAGE_MAP(CHierarchyView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CHierarchyView::OnTvnSelchanged)
	ON_WM_ERASEBKGND()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CHierarchyView::OnTvnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, &CHierarchyView::OnTvnEndlabeledit)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, &CHierarchyView::OnTvnBeginlabeledit)
	ON_WM_CONTEXTMENU()
	ON_WM_MENUSELECT()
	ON_NOTIFY_REFLECT(NM_RCLICK, &CHierarchyView::OnNMRClick)
	ON_COMMAND(ID_HIERARCHY_EDIT_NAME, &CHierarchyView::OnSelectedEditMenu)
	ON_COMMAND(ID_HIERARCHY_DELETE, &CHierarchyView::OnSelectedDeleteMenu)
	ON_COMMAND(ID_HIERARCHY_NEW, &CHierarchyView::OnSelectedNewMenu)
	ON_NOTIFY_REFLECT(TVN_DELETEITEM, &CHierarchyView::OnTvnDeleteitem)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CHierarchyView 진단입니다.

#ifdef _DEBUG
void CHierarchyView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CHierarchyView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

#endif
#endif //_DEBUG


// CHierarchyView 메시지 처리기입니다.

void CHierarchyView::CreateNewTreeItem(TVI_TYPE _type, CString & _cstrName, OBJECTINFO * _pPrefab/* = nullptr */)
{
	
	if (_cstrName.IsEmpty())
	{
		_cstrName = _pPrefab->cstrName;
		int i = 0;
		while (true)
		{

			CString cstrIndex;
			CString ObjName = _cstrName.GetString();
			cstrIndex.Format(_T("_%d"), i);
			ObjName.Append(cstrIndex);
			auto& iter_mapPalce = m_mapTreeItem.find(ObjName);

			if (iter_mapPalce == m_mapTreeItem.end())
			{
				_cstrName = ObjName;
				break;
			}
			else
				i++;
		}
	}
	else if (m_mapTreeItem.find(_cstrName) != m_mapTreeItem.end())
	{
		int i = 0;
		while (true)
		{

			CString cstrIndex;
			CString ObjName = _cstrName.GetString();
			cstrIndex.Format(_T("_%d"), i);
			ObjName.Append(cstrIndex);
			auto& iter_mapPalce = m_mapTreeItem.find(ObjName);

			if (iter_mapPalce == m_mapTreeItem.end())
			{
				_cstrName = ObjName;
				break;
			}
			else
				i++;
		}
	}
	if (_type == CHierarchyView::ACTOR)
	{
		InsertNewEmptyActorToMap(_cstrName, _pPrefab);
	}
		
	InsertTreeItem(_type, _cstrName);
}

void CHierarchyView::InsertTreeItem(TVI_TYPE _type, CString& _cstrName)
{
	CTreeCtrl& tree = GetTreeCtrl();
	HTREEITEM selectedItem = tree.GetSelectedItem();
	TVINSERTSTRUCT tvInsert;

	tvInsert.hParent = ((selectedItem != NULL) ? selectedItem : m_RootTreeItem);
	tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvInsert.hInsertAfter = TVI_SORT;
	tvInsert.item.pszText = _cstrName.GetBuffer();
	
	switch (_type)
	{
	case CHierarchyView::FOLDER:
		tvInsert.item.iImage = 0;
		tvInsert.item.iSelectedImage = 0;
		break;
	case CHierarchyView::ACTOR:
		tvInsert.item.iImage = 1;
		tvInsert.item.iSelectedImage = 1;
		break;
	default:
		break;
	}

	m_mapTreeItem.emplace(_cstrName,_type);
	tree.InsertItem(&tvInsert);
}

void CHierarchyView::InsertNewEmptyActorToMap(CString & _cstrName, OBJECTINFO * _pPrefab)
{
	ACTORINFO* pActorInfo = new ACTORINFO{};
	pActorInfo->tInfo.vPos = { 400.f + m_pView->GetScrollPos(SB_HORZ), 300.f + m_pView->GetScrollPos(SB_VERT), 0.f };
	pActorInfo->tInfo.vSize = { 1.f, 1.f, 0.f };
	pActorInfo->wstrActorName = _cstrName;
	pActorInfo->wstrPrefabName = _pPrefab->cstrName;
	m_mapActorInfo.emplace(_cstrName, pActorInfo);
	m_pView->Invalidate(FALSE);
}

void CHierarchyView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	CTreeCtrl& tree = GetTreeCtrl();
	tree.SetItemState(pNMTreeView->itemOld.hItem, ~TVIS_BOLD, TVIS_BOLD);

	tree.SetItemState(pNMTreeView->itemNew.hItem, TVIS_BOLD, TVIS_BOLD);
	CString cstrSelectedTree = tree.GetItemText(pNMTreeView->itemNew.hItem);
	if (cstrSelectedTree == L"Root")
		return;
	auto& iter_find = m_mapTreeItem.find(cstrSelectedTree);
	if (iter_find != m_mapTreeItem.end())
	{
		if(iter_find->second == ACTOR)
			m_pOptionView->OnHirerachyTreeCtrlSelectChanged(tree.GetItemText(pNMTreeView->itemNew.hItem));
		else
			m_pOptionView->OnHirerachyTreeCtrlSelectChanged(nullptr);
	}
	else
	{
		POINT pt;
		GetCursorPos(&pt);
		ClientToScreen(&pt);
		if(tree.HitTest(pt))
			ERR_MSG(L"CHierarchyView::OnTvnSelchanged 에서 map탐색 실패");
	}


	*pResult = 0;
}

void CHierarchyView::OnSelectedEditMenu()
{
	CTreeCtrl& tree = GetTreeCtrl();



	GetTreeCtrl().EditLabel(GetTreeCtrl().GetSelectedItem());
}

void CHierarchyView::OnSelectedDeleteMenu()
{
	CTreeCtrl& tree = GetTreeCtrl();

	if (tree.GetItemText(tree.GetSelectedItem()) == L"Root")
	{
		ERR_MSG(L"루트는 지울 수 업습니다.");
		return;
	}

	HTREEITEM hitem = tree.GetSelectedItem();

	if (tree.ItemHasChildren(hitem))
	{
		int iReturn = AfxMessageBox(L"자식이 모두 삭제됩니다. 그래도 삭제하시겠습니까?", MB_YESNO);
		switch (iReturn)
		{
		case IDYES:
			break;
		case IDNO:
			return;
		default:
			return;
		}
	}

	tree.DeleteItem(GetTreeCtrl().GetSelectedItem());

	tree.Invalidate(TRUE);
}

void CHierarchyView::OnSelectedNewMenu()
{
	m_tNewActorDialog.DoModal();
}

void CHierarchyView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	m_pOptionView = dynamic_cast<COptionView*>(pMain->m_tRightSplitter.GetPane(1, 0));
	m_pView = dynamic_cast<CMFCToolView*>(pMain->m_tMainSplitter.GetPane(0, 1));

	//if (nullptr == m_tNewActorDialog.GetSafeHwnd())
	//	m_tNewActorDialog.CreateIndirect(IDD_HIERARCHYNEWACTORDIALOG,);

	CTreeCtrl& tree = GetTreeCtrl();
	tree.SetFocus();
	tree.SetBkColor(RGB(230, 230, 230));
	tree.SetRedraw(TRUE);

	SHFILEINFO sFileInfo = { 0 };
	const UINT FLAGS = SHGFI_ICON | SHGFI_USEFILEATTRIBUTES;
	LPCTSTR PATH = _T("C:\\Windows");

	m_ImageList.Create(16, 16, ILC_COLOR32, 0, 4);

	// Get the directory icon
	if (0 != ::SHGetFileInfo(PATH, FILE_ATTRIBUTE_DIRECTORY, &sFileInfo,
		sizeof(SHFILEINFO), FLAGS))
	{
		m_ImageList.Add(sFileInfo.hIcon);
	}
	

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_FOLDER);
	
	m_ImageList.SetBkColor(RGB(230, 230, 230));
	m_ImageList.Add(&bitmap,RGB(255,255,255));
	
	tree.SetImageList(&m_ImageList, TVSIL_NORMAL);

	m_RootTreeItem = tree.InsertItem(L"Root", 1, 1, TVI_ROOT, TVI_SORT);
	CreateNewTreeItem(FOLDER, CString(_T("자식1")));
	CreateNewTreeItem(FOLDER, CString(_T("자식2")));
}


BOOL CHierarchyView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CHILD | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_EDITLABELS | TVS_FULLROWSELECT;
	//cs.style |= TVS_TRACKSELECT;
	//cs.style |= TVS_DISABLEDRAGDROP;
	



	return CTreeView::PreCreateWindow(cs);
}


BOOL CHierarchyView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//CRect rect;
	//GetClientRect(&rect);

//	pDC->FillSolidRect(rect, RGB(230, 230, 230));

//	return true;

	return CTreeView::OnEraseBkgnd(pDC);
}


void CHierarchyView::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	HIMAGELIST himl;
	RECT rect;

	himl = TreeView_CreateDragImage(pNMHDR->hwndFrom, pNMTreeView->itemNew.hItem);

	TreeView_GetItemRect(pNMHDR->hwndFrom, pNMTreeView->itemNew.hItem, &rect, TRUE);

	ImageList_BeginDrag(himl, 0, 0, 0);
	ImageList_DragEnter(pNMHDR->hwndFrom, pNMTreeView->ptDrag.x, pNMTreeView->ptDrag.x);

	ShowCursor(FALSE);
	SetCapture();

	m_bDraging = true;

	*pResult = 0;
}


void CHierarchyView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	HTREEITEM htiTarget;  // Handle to target item. 
	TVHITTESTINFO tvht;   // Hit test information. 
	
	if (!m_bDraging)
		return CTreeView::OnMouseMove(nFlags, point);

	POINT pt = point;
	GetCursorPos(&pt);
	GetTreeCtrl().ScreenToClient(&pt);
	
	ImageList_DragMove(pt.x, pt.y);
	// Turn off the dragged image so the background can be refreshed.
	ImageList_DragShowNolock(FALSE);

	// Find out if the pointer is on the item. If it is, 
	// highlight the item as a drop target. 
	tvht.pt.x = pt.x;
	tvht.pt.y = pt.y;
	if ((htiTarget = TreeView_HitTest(GetSafeHwnd(), &tvht)) != NULL)
	{
		TreeView_SelectDropTarget(GetSafeHwnd(), htiTarget);
	}
	else
	{
		TreeView_SelectDropTarget(GetSafeHwnd(), NULL);
	}
	ImageList_DragShowNolock(TRUE);
	
	CTreeView::OnMouseMove(nFlags, point);
}


void CHierarchyView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bDraging)
	{
		// Get destination item.
		HTREEITEM htiDest = TreeView_GetDropHilight(GetSafeHwnd());
		if (htiDest != NULL)
		{
			// To do: handle the actual moving of the dragged node.
		}
		ImageList_EndDrag();
		TreeView_SelectDropTarget(GetSafeHwnd(), NULL);
		ReleaseCapture();
		ShowCursor(TRUE);
		m_bDraging = FALSE;
	}

	CTreeView::OnLButtonUp(nFlags, point);
}

void CHierarchyView::OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	CTreeCtrl& tree = GetTreeCtrl();
	m_cstrEditFrom = tree.GetItemText(pTVDispInfo->item.hItem);

	*pResult = 0;
}

void CHierarchyView::OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	CTreeCtrl& tree = GetTreeCtrl();

	CEdit *pEdit = tree.GetEditControl();
	if (!pEdit)
	{
		return;
	}
	CString cstrEdit;
	pEdit->GetWindowText(cstrEdit);
	if (cstrEdit.GetLength() <= 0 || cstrEdit == m_cstrEditFrom)
		return;
	auto& iter_Tree_Find = m_mapTreeItem.find(cstrEdit);
	if (iter_Tree_Find != m_mapTreeItem.end())
	{
		ERR_MSG(L"이미 있는 이름입니다.");
		*pResult = 0;
		return;
	}
	tree.SetItemText(pTVDispInfo->item.hItem, cstrEdit);
	iter_Tree_Find = m_mapTreeItem.find(m_cstrEditFrom);
	if (iter_Tree_Find == m_mapTreeItem.end())
	{
		ERR_MSG(L"이름은 변경되지만, 맵에 존재하지 않았던 이름이였습니다. Tree");
	}
	else
	{
		CHierarchyView::TVI_TYPE _type = iter_Tree_Find->second;
		map<CString, ACTORINFO*>::iterator iter_Actor_find;
		if (iter_Tree_Find->second == ACTOR)
		{
			iter_Actor_find = m_mapActorInfo.find(m_cstrEditFrom);
			ACTORINFO* pTempActorinfo;
			if (iter_Actor_find != m_mapActorInfo.end())
			{
				pTempActorinfo = iter_Actor_find->second;
				m_mapActorInfo.erase(m_cstrEditFrom);
			}
			else
			{
				ERR_MSG(L"이름은 변경되지만, 맵에 존재하지 않았던 이름이였습니다. Actor");
			}
			m_mapActorInfo.emplace(cstrEdit, pTempActorinfo);
		}
		m_mapTreeItem.erase(iter_Tree_Find);
		m_mapTreeItem.emplace(cstrEdit, _type);
	}

	*pResult = 0;
}





void CHierarchyView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{

	TVHITTESTINFO tvht;
	// Find out if the pointer is on the item. If it is, 
	// highlight the item as a drop target. 

	CTreeCtrl& tree = GetTreeCtrl();

	POINT pt = point;
	GetCursorPos(&pt);
	tree.ScreenToClient(&pt);

	tvht.pt.x = pt.x;
	tvht.pt.y = pt.y;

	HTREEITEM hit = tree.HitTest(tvht.pt);
	tree.SelectItem(hit);

	if (!hit)
	{
		return;
	}

	CMenu menu;
	menu.CreatePopupMenu();

	menu.AppendMenuW(MF_STRING, ID_HIERARCHY_EDIT_NAME, _T("이름 바꾸기 (F2)"));
	menu.AppendMenuW(MF_STRING, ID_HIERARCHY_NEW, _T("새 액터"));
	menu.AppendMenuW(MF_STRING, ID_HIERARCHY_DELETE, _T("삭제"));
	
	//menu.EnableMenuItem(ID_HIERARCHY_EDIT_NAME, MF_ENABLED);
	//menu.EnableMenuItem(ID_HIERARCHY_DELETE, MF_ENABLED);

	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		point.x,
		point.y,
		AfxGetMainWnd());
	menu.DestroyMenu();
	
}


void CHierarchyView::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
{
	CTreeView::OnMenuSelect(nItemID, nFlags, hSysMenu);

	CTreeCtrl& tree = GetTreeCtrl();

	switch (nItemID)
	{
	case ID_HIERARCHY_EDIT_NAME:
		tree.EditLabel(tree.GetSelectedItem());
		break;
	default:
		break;
	}
}


void CHierarchyView::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	CTreeCtrl & tree = GetTreeCtrl();
	/* Get the cursor position for this message */
	DWORD dwPos = GetMessagePos();
	/* Convert the co-ords into a CPoint structure */
	CPoint pt(GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos));
	OnContextMenu(this, pt);
}


void CHierarchyView::OnTvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	if (m_bDestroying)
	{
		*pResult = 0;
		return;
	}
	CTreeCtrl& tree = GetTreeCtrl();
	HTREEITEM hitem = tree.GetSelectedItem();
	auto& iter_find = m_mapActorInfo.find(tree.GetItemText(hitem));
	if (iter_find == m_mapActorInfo.end())
	{
		//ERR_MSG(L"m_mapActorInfo 에 존재하지 않는 항목을 삭제했습니다.");
	}
	else
	{
		Safe_Delete(iter_find->second);
		m_mapActorInfo.erase(iter_find);
	}
	auto& iter_TVI_find = m_mapTreeItem.find(tree.GetItemText(hitem));
	if (iter_TVI_find == m_mapTreeItem.end())
	{
		//ERR_MSG(L"m_mapTreeItem 에 존재하지 않는 항목을 삭제했습니다.");
	}
	else
	{
		m_mapTreeItem.erase(iter_TVI_find);
	}

	//GetTreeCtrl().DeleteTempMap();
	*pResult = 0;
}

void CHierarchyView::OnDestroy()
{
	m_bDestroying = true;
	CTreeView::OnDestroy();
}

