// Mapping.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "Mapping.h"
#include "MainFrm.h"
#include "ObjectTool.h"
#include "Form.h"
#include "HierarchyView.h"
#include "MFCToolView.h"


// CMapping

IMPLEMENT_DYNCREATE(CMapping, CFormView)

CMapping::CMapping()
	: CFormView(IDD_MAPPING)
	, m_uiWidth(CLIENTCX)
	, m_uiHeight(CLIENTCY)
	, m_uiStageFirst(1)
	, m_uiStageSecond(1)
	, m_pSelectedPrefab(nullptr)
{

}

CMapping::~CMapping()
{
}

void CMapping::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_M_WIDTH, m_uiWidth);
	DDX_Text(pDX, IDC_EDIT_M_HEIGHT, m_uiHeight);
	DDX_Text(pDX, IDC_EDIT_M_FIRST, m_uiStageFirst);
	DDX_Text(pDX, IDC_EDIT_M_SECOND, m_uiStageSecond);
	DDX_Control(pDX, IDC_PIC_M_PREFAB, m_Pic_Prefab);
	DDX_Control(pDX, IDC_LIST_M_PREFAB, m_Listbox_Prefab);
	LoadPrefab();
}

void CMapping::LoadPrefab()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm* pForm = dynamic_cast<CForm*>(pMain->m_tLeftSplitter.GetPane(1, 0));
	m_Listbox_Prefab.ResetContent();
	auto& map = pForm->m_tObjectTool.m_mapObject;
	for (auto& rPair : map)
	{
		m_Listbox_Prefab.AddString(rPair.second->cstrName);
	}
}

BEGIN_MESSAGE_MAP(CMapping, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_M_APPLY, &CMapping::OnBnClickedButtonMApply)
	ON_BN_CLICKED(IDC_BUTTON_M_SAVE, &CMapping::OnBnClickedButtonMSave)
	ON_BN_CLICKED(IDC_BUTTON_M_LOAD, &CMapping::OnBnClickedButtonMLoad)
	ON_LBN_SELCHANGE(IDC_LIST_M_PREFAB, &CMapping::OnLbnSelchangeListMPrefab)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMapping 진단입니다.

#ifdef _DEBUG
void CMapping::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMapping::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMapping 메시지 처리기입니다.


void CMapping::OnBnClickedButtonMApply()
{
	UpdateData(TRUE);
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CHierarchyView* pHView = dynamic_cast<CHierarchyView*>(pMain->m_tRightSplitter.GetPane(0, 0));
	CMFCToolView* pView = dynamic_cast<CMFCToolView*>(pMain->m_tMainSplitter.GetPane(0, 1));
	pView->SetScrollSizes(MM_TEXT, CSize(m_uiWidth, m_uiHeight));
}


void CMapping::OnBnClickedButtonMSave()
{
	UpdateData(TRUE);
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CHierarchyView* pHView = dynamic_cast<CHierarchyView*>(pMain->m_tRightSplitter.GetPane(0, 0));
	pHView->SaveTreeItems(m_uiStageFirst, m_uiStageSecond);
}


void CMapping::OnBnClickedButtonMLoad()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CHierarchyView* pHView = dynamic_cast<CHierarchyView*>(pMain->m_tRightSplitter.GetPane(0, 0));
	pHView->SaveTreeItems(m_uiStageFirst, m_uiStageSecond);
	UpdateData(TRUE);
	pHView->LoadTreeItems(m_uiStageFirst, m_uiStageSecond);

	UpdateData(FALSE);
}


void CMapping::OnLbnSelchangeListMPrefab()
{
	int iPrefabSelected = m_Listbox_Prefab.GetCurSel();

	if (iPrefabSelected == LB_ERR)
		return;

	CString cstrObjectName;
	m_Listbox_Prefab.GetText(iPrefabSelected, cstrObjectName);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm* pForm = dynamic_cast<CForm*>(pMain->m_tLeftSplitter.GetPane(1, 0));
	auto* pMapPrefab = &pForm->m_tObjectTool.m_mapObject;
	auto *pMapAnimation = &pForm->m_tAnimationTool.m_mapAnima;
	auto& iter_find = pMapPrefab->find(cstrObjectName);
	if (iter_find == pMapPrefab->end())
	{
		ERR_MSG(L"CMapping::OnLbnSelchangeListMPrefab");
		return;
	}
	m_pSelectedPrefab = iter_find->second;
	CString cstrKey;
	if (iter_find->second->bIsSingle)
	{
		cstrKey = iter_find->second->cstrObjectImage_ObjectKey;
		CTexture_Manager::Get_Instance()->DrawPic(cstrKey, iter_find->second->tRect,0,m_Pic_Prefab);
	}		
	else
	{
		auto& iter_find_Anim = pMapAnimation->find(iter_find->second->cstrIdleAnimImage_ObjectKey + iter_find->second->cstrIdleAnimImage_StateKey);
		cstrKey = iter_find->second->cstrIdleAnimImage_ObjectKey;
		if (iter_find_Anim == pMapAnimation->end())
		{
			ERR_MSG(L"CMapping::OnLbnSelchangeListMPrefab");
			return;
		}
		
		CTexture_Manager::Get_Instance()->DrawPic(cstrKey, iter_find_Anim->second->vecRect[0], 0, m_Pic_Prefab);
	}

}


void CMapping::OnDestroy()
{
	UpdateData(TRUE);
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CHierarchyView* pHView = dynamic_cast<CHierarchyView*>(pMain->m_tRightSplitter.GetPane(0, 0));
	pHView->SaveTreeItems(m_uiStageFirst, m_uiStageSecond);
	CFormView::OnDestroy();
}


void CMapping::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	LoadPrefab();
}
