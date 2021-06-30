// OptionView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "OptionView.h"
#include "MainFrm.h"
#include "Form.h"
#include "HierarchyView.h"


// COptionView

IMPLEMENT_DYNCREATE(COptionView, CFormView)

COptionView::COptionView()
	: CFormView(IDD_OPTIONVIEW)
	, m_fPosX(0)
	, m_fPosY(0)
	, m_fScaleX(0)
	, m_fScaleY(0)
	, m_fAngle(0)
{

}

COptionView::~COptionView()
{
}

void COptionView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_POSX, m_fPosX);
	DDX_Text(pDX, IDC_EDIT_POSY, m_fPosY);
	DDX_Text(pDX, IDC_EDIT_SCALEX, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT_SCALEY, m_fScaleY);
	DDX_Text(pDX, IDC_EDIT_OPTION_ANGLE, m_fAngle);
	DDX_Control(pDX, IDC_COMBO_OPTION_PREFAB, m_ComboBox_PrefabList);
}

BEGIN_MESSAGE_MAP(COptionView, CFormView)
	//ON_BN_CLICKED(IDC_BUTTON_OPTION_APPLY, &COptionView::OnBnClickedButtonApply)
	ON_EN_KILLFOCUS(IDC_EDIT_POSX, &COptionView::OnEnKillfocusEditPosx)
	ON_CBN_SELCHANGE(IDC_COMBO_OPTION_PREFAB, &COptionView::OnCbnSelchangeComboOptionPrefab)
END_MESSAGE_MAP()


// COptionView 진단입니다.

#ifdef _DEBUG
void COptionView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void COptionView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// COptionView 메시지 처리기입니다.


//void COptionView::OnBnClickedButtonApply()
//{
//	UpdateData(TRUE);
//	int iIdx = m_Result_ListBox.GetCurSel();
//	CString wstrName;
//	m_Result_ListBox.GetText(iIdx, wstrName);
//	auto& iter = m_mapActorInfo.find(wstrName);
//
//	if (iter == m_mapActorInfo.end())
//	{
//		ERR_MSG(L"맵에 해당 키가 없다");
//		return;
//	}
//	iter->second->tInfo.vPos = { m_fPosX ,m_fPosY,0.f };
//	iter->second->tInfo.fAngle = m_fRotZ;
//	iter->second->tInfo.vSize = { m_fScaleX,m_fScaleY ,0.f };
//	m_pView->Invalidate(FALSE);
//	UpdateData(FALSE);
//}

void COptionView::OnLbnSelchangeListOptionPrefab()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void COptionView::OnEnKillfocusEditPosx()
{

}


void COptionView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
	m_pmapPrefab = &(pForm->m_tObjectTool.m_mapObject);

	CHierarchyView*	pHierarchyView = dynamic_cast<CHierarchyView*>(pMain->m_tRightSplitter.GetPane(0, 0));
	m_pmapActorInfo = &(pHierarchyView->m_mapActorInfo);

	for (auto& rPair : *m_pmapPrefab)
	{
		CString cstrTemp = rPair.second->cstrName;
		if (m_ComboBox_PrefabList.FindStringExact(-1, cstrTemp.GetString()) == -1)
			m_ComboBox_PrefabList.AddString(cstrTemp);
	}
}


void COptionView::OnCbnSelchangeComboOptionPrefab()
{

}
