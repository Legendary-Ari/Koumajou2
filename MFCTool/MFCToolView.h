
// MFCToolView.h : CMFCToolView Ŭ������ �������̽�
//

#pragma once

class CTerrain; 
class CMFCToolDoc;
class CUiTool;
class CMFCToolView : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	CMFCToolView();
	DECLARE_DYNCREATE(CMFCToolView)

// Ư���Դϴ�.
public:
	CMFCToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	CTerrain* m_pTerrain; 
	CUiTool* m_pUiTool;
	float m_fAngle = 0.f;
	float m_fAlpha = 0.f; 
	float m_fBulletSpeed = 10.f; 
// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CMFCToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // MFCToolView.cpp�� ����� ����
inline CMFCToolDoc* CMFCToolView::GetDocument() const
   { return reinterpret_cast<CMFCToolDoc*>(m_pDocument); }
#endif
