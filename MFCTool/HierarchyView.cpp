// HierarchyView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "HierarchyView.h"


// CHierarchyView

IMPLEMENT_DYNCREATE(CHierarchyView, CTreeView)

CHierarchyView::CHierarchyView()
{

}

CHierarchyView::~CHierarchyView()
{
}

BEGIN_MESSAGE_MAP(CHierarchyView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CHierarchyView::OnTvnSelchanged)
	ON_WM_ERASEBKGND()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CHierarchyView::OnTvnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
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


void CHierarchyView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void CHierarchyView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();
	CTreeCtrl& tree = GetTreeCtrl();

	tree.SetBkColor(RGB(230, 230, 230));

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

	HTREEITEM temp = tree.InsertItem(L"배고파", 1, 1, TVI_ROOT, TVI_SORT);
	HTREEITEM temp2 = tree.InsertItem(L"그러게", 0, 1, temp, TVI_SORT);

	TV_INSERTSTRUCT tvi;

	//tvi.item.stateMask |= TVIS_BOLD;
	tvi.item.state;


}


BOOL CHierarchyView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= TVS_HASBUTTONS | TVS_LINESATROOT | TVS_EDITLABELS;
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
	ClientToScreen(&pt);
	ScreenToClient(&pt);
	
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
