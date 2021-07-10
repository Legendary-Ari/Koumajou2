#pragma once
#include "afxwin.h"



// CMapping 폼 뷰입니다.

class CMapping : public CFormView
{
	DECLARE_DYNCREATE(CMapping)

protected:
	CMapping();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMapping();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPPING };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void LoadPrefab();
public:
	UINT m_uiWidth;
	UINT m_uiHeight;
	UINT m_uiStageFirst;
	UINT m_uiStageSecond;
	CStatic m_Pic_Prefab;
	CListBox m_Listbox_Prefab;
	const OBJECTINFO* m_pSelectedPrefab;
public:
	afx_msg void OnBnClickedButtonMApply();
	afx_msg void OnBnClickedButtonMSave();
	afx_msg void OnBnClickedButtonMLoad();
	afx_msg void OnLbnSelchangeListMPrefab();
	
	afx_msg void OnDestroy();
	virtual void OnInitialUpdate();
};


