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

void COptionView::OnHirerachyTreeCtrlSelectChanged(CString  _cstrActorName)
{
	UpdatePrefabComboBox();
	if (_cstrActorName.IsEmpty())
	{
		m_ComboBox_PrefabList.SetCurSel(-1);
		return;
	}
	auto& iter_Actor_find = m_pmapActorInfo->find(_cstrActorName);
	if (iter_Actor_find == m_pmapActorInfo->end())
	{
		ERR_MSG(L"트리항목의 해당하는 맵을 찾지 못했습니다. COptionView::OnHirerachyTreeCtrlSelectChanged");
		return;
	}
	ACTORINFO * pActorInfo = iter_Actor_find->second;
	int iComboIdx = m_ComboBox_PrefabList.FindString(-1, pActorInfo->wstrPrefabName);
	m_ComboBox_PrefabList.SetCurSel(iComboIdx);

	m_fPosX = pActorInfo->tInfo.vPos.x;
	m_fPosY = pActorInfo->tInfo.vPos.y;
	m_fScaleX = pActorInfo->tInfo.vSize.x;
	m_fScaleY = pActorInfo->tInfo.vSize.y;
	m_fAngle = pActorInfo->tInfo.fAngle;
	UpdateData(FALSE);
	OnCbnSelchangeComboOptionPrefab();
}

void COptionView::UpdatePrefabComboBox()
{
	for (auto& rPair : *m_pmapPrefab)
	{
		CString cstrTemp = rPair.second->cstrName;
		if (m_ComboBox_PrefabList.FindStringExact(-1, cstrTemp.GetString()) == -1)
			m_ComboBox_PrefabList.AddString(cstrTemp);
	}
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
	DDX_Control(pDX, IDC_PIC_OPTION_OBJ, m_PicturePrefab);
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

	UpdatePrefabComboBox();
}


void COptionView::OnCbnSelchangeComboOptionPrefab()
{
	int iComboIdx = m_ComboBox_PrefabList.GetCurSel();
	if (iComboIdx == CB_ERR)
	{
		CGraphic_Device::Get_Instance()->Render_Begin();
		CGraphic_Device::Get_Instance()->Render_End(m_PicturePrefab.m_hWnd);
	}
	CString cstrPrafabName;
	m_ComboBox_PrefabList.GetLBText(iComboIdx, cstrPrafabName);

	auto& iter_find = m_pmapPrefab->find(cstrPrafabName);
	if (iter_find == m_pmapPrefab->end())
	{
		ERR_MSG(L"그림없습니다.");
		return;
	}
		

	CGraphic_Device::Get_Instance()->Render_Begin();
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(iter_find->second->cstrObjectImage_ObjectKey.GetString());
	if (nullptr == pTexInfo)
		return;
	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, WINCX / pTexInfo->tImageInfo.Width, WINCY / pTexInfo->tImageInfo.Height, 0.f);
	D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f);
	matWorld = matScale * matTrans;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	CGraphic_Device::Get_Instance()->Render_End(m_PicturePrefab.m_hWnd);
}
