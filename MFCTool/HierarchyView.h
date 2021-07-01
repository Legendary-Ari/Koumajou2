#pragma once


// CHierarchyView 뷰입니다.
#include "HierarchyNewActorDialog.h"
class COptionView;
class CHierarchyView : public CTreeView
{
	DECLARE_DYNCREATE(CHierarchyView)

protected:
	CHierarchyView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CHierarchyView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
public:
		enum TVI_TYPE { FOLDER, ACTOR, TYPE_END };

protected:
	DECLARE_MESSAGE_MAP()
public:
	void CreateNewTreeItem(TVI_TYPE _type, CString& _cstrName, OBJECTINFO* _pPrefab = nullptr);
	void InsertTreeItem(TVI_TYPE _type, CString& _cstrName);
	void InsertNewEmptyActorToMap(CString& _cstrName, OBJECTINFO* _pPrefab);
private:
	bool		m_bDestroying;
	COptionView*	m_pOptionView;
	CString		m_cstrEditFrom;
public:
	CHierarchyNewActorDialog m_tNewActorDialog;
	map<CString, ACTORINFO*> m_mapActorInfo;
	map<CString, TVI_TYPE> m_mapTreeItem;
public:

	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelectedEditMenu();
	afx_msg void OnSelectedDeleteMenu();
	afx_msg void OnSelectedNewMenu();
	virtual void OnInitialUpdate();

	//CTreeCtrl	m_TreeCtrl;
	HTREEITEM m_RootTreeItem;
	CImageList m_ImageList;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);

	bool m_bDraging;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
};


