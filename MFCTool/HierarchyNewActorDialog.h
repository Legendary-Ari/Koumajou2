#pragma once
#include "afxwin.h"


// CHierarchyNewActorDialog ��ȭ �����Դϴ�.

class CHierarchyNewActorDialog : public CDialog
{
	DECLARE_DYNAMIC(CHierarchyNewActorDialog)

public:
	CHierarchyNewActorDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CHierarchyNewActorDialog();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HIERARCHYNEWACTORDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:

	
	CComboBox m_ComboNewActor;

	virtual BOOL OnInitDialog();

public:
	map<CString, OBJECTINFO*>* m_pmapPrefab;
	afx_msg void OnBnClickedNewactorOk();
	CString m_cstrTreeName;
};
