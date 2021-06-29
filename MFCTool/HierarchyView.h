#pragma once


// CHierarchyView ���Դϴ�.

class CHierarchyView : public CTreeView
{
	DECLARE_DYNCREATE(CHierarchyView)

protected:
	CHierarchyView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CHierarchyView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
private:
		enum TVI_TYPE { FOLDER, OBJECT, TYPE_END };
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnInitialUpdate();

	//CTreeCtrl	m_TreeCtrl;
	
	CImageList m_ImageList;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);

	bool m_bDraging;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


