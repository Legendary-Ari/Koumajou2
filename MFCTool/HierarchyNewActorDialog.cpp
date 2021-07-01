// CHierarchyNewActorDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "HierarchyNewActorDialog.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "Form.h"
#include "HierarchyView.h"


// CHierarchyNewActorDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CHierarchyNewActorDialog, CDialog)

CHierarchyNewActorDialog::CHierarchyNewActorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_HIERARCHYNEWACTORDIALOG, pParent)
	, m_cstrTreeName(_T(""))
{

}

CHierarchyNewActorDialog::~CHierarchyNewActorDialog()
{
}

void CHierarchyNewActorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_NEWACTOR, m_ComboNewActor);
	DDX_Text(pDX, IDC_EDIT_NEWACTOR_NAME, m_cstrTreeName);
}


BEGIN_MESSAGE_MAP(CHierarchyNewActorDialog, CDialog)
	ON_BN_CLICKED(ID_NEWACTOR_OK, &CHierarchyNewActorDialog::OnBnClickedNewactorOk)
END_MESSAGE_MAP()


// CHierarchyNewActorDialog �޽��� ó�����Դϴ�.


BOOL CHierarchyNewActorDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
	m_pmapPrefab = &(pForm->m_tObjectTool.m_mapObject);

	m_ComboNewActor.ResetContent();
	m_ComboNewActor.AddString(L"0 - Folder");
	for (auto& rPair : *m_pmapPrefab)
	{
		m_ComboNewActor.AddString(rPair.first);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CHierarchyNewActorDialog::OnBnClickedNewactorOk()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CHierarchyView*	pHierarchyView = dynamic_cast<CHierarchyView*>(pMain->m_tRightSplitter.GetPane(0, 0));

	int iSelectedIdx = m_ComboNewActor.GetCurSel();
	if (iSelectedIdx == CB_ERR)
	{
		ERR_MSG(L"�׸��� �������ּ���");
		return;
	}
	else if (iSelectedIdx == 0)
	{
		UpdateData(TRUE);
		pHierarchyView->CreateNewTreeItem(CHierarchyView::FOLDER, m_cstrTreeName, nullptr);
	}
	else
	{
		CString cstrPrefabName;
		m_ComboNewActor.GetLBText(iSelectedIdx, cstrPrefabName);
		auto& iter_find = m_pmapPrefab->find(cstrPrefabName);
		if (iter_find == m_pmapPrefab->end())
		{
			ERR_MSG(L"�׷� �̸��� �������� �ʿ� �����ϴ�.");
			return;
		}
		UpdateData(TRUE);
		pHierarchyView->CreateNewTreeItem(CHierarchyView::ACTOR, m_cstrTreeName, iter_find->second);
	}
	OnOK();
}
