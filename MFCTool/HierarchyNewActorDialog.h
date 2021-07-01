#pragma once
#include "afxwin.h"


// CHierarchyNewActorDialog 대화 상자입니다.

class CHierarchyNewActorDialog : public CDialog
{
	DECLARE_DYNAMIC(CHierarchyNewActorDialog)

public:
	CHierarchyNewActorDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CHierarchyNewActorDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HIERARCHYNEWACTORDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	
	CComboBox m_ComboNewActor;

	virtual BOOL OnInitDialog();

public:
	map<CString, OBJECTINFO*>* m_pmapPrefab;
	afx_msg void OnBnClickedNewactorOk();
	CString m_cstrTreeName;
};
