
// MFCToolView.cpp : CMFCToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "MFCTool.h"
#endif

#include "MFCToolDoc.h"
#include "MFCToolView.h"
#include "Single_Texture.h"
#include "MainFrm.h"
#include "Form.h"
#include "UiTool.h"
#include "HierarchyView.h"
#include "Mapping.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
HWND g_hWND; 

// CMFCToolView

IMPLEMENT_DYNCREATE(CMFCToolView, CScrollView)

BEGIN_MESSAGE_MAP(CMFCToolView, CScrollView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CMFCToolView ����/�Ҹ�

CMFCToolView::CMFCToolView()
	:m_pTerrain(nullptr)
	, m_pSelectedActor(nullptr)
	, m_iPivotMoveing(-1)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CMFCToolView::~CMFCToolView()
{

	//Safe_Delete(m_pTerrain);
	CTexture_Manager::Destroy_Instance(); 
	CGraphic_Device::Destroy_Instance(); 
}

BOOL CMFCToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CMFCToolView �׸���

void CMFCToolView::OnDraw(CDC* /*pDC*/)
{
	CMFCToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// �׸��� �׸��� ���� �Ϸ��� ���� 
	//1. ����۸� ���� 
	CGraphic_Device::Get_Instance()->Render_Begin(); 
	//m_pTerrain->Render_Terrain(); 
	//CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	//CMiniview* pView = dynamic_cast<CMiniview*>(pMain->m_tSecondSplitter.GetPane(0, 0));
	//CForm* pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
	//pForm->m_tUiTool.SetView(this);
	//pForm->m_tUiTool.Render_UI();

	for (int i = 0; i < RENDERID::END; ++i)
	{
		m_listRender[i].clear();
	}

	for (auto& rPair : m_pHierarchyView->m_mapActorInfo) // Render������ �ٲٱ�
	{
		if (rPair.second->bIsFolder)
			continue;
		auto& iter = m_pmapPrefab->find(rPair.second->wstrPrefabName);
		if (iter == m_pmapPrefab->end())
		{
			ERR_MSG(L"CMFCToolView::OnDraw �ʿ� �ش� Ű�� �����ϴ�");
			return;
		}
		ACTORINFO* pActorInfo = rPair.second;
		OBJECTINFO* pObjectInfo = iter->second;
		m_listRender[pObjectInfo->eRenderId].emplace_back(pActorInfo);
	}
	

	m_mapCollisionToPrefab.clear();
	for (int i = 0; i < RENDERID::END; ++i)
	{
		for (auto& pActorInfo : m_listRender[i])
		{
			auto& iter = m_pmapPrefab->find(pActorInfo->wstrPrefabName);
			if (iter == m_pmapPrefab->end())
			{
				ERR_MSG(L"CMFCToolView::OnDraw �ʿ� �ش� Ű�� �����ϴ�");
				return;
			}

			OBJECTINFO* pObjectInfo = iter->second;

			const TEXINFO* pTexInfo = nullptr;
			if (pObjectInfo->bIsSingle)
			{
				pTexInfo = CTexture_Manager::Get_Instance()
					->Get_TexInfo(pObjectInfo->cstrObjectImage_ObjectKey.GetString());
			}
			else
			{
				pTexInfo = CTexture_Manager::Get_Instance()
					->Get_TexInfo(pObjectInfo->cstrIdleAnimImage_ObjectKey.GetString());
			}
			
			if (!pTexInfo)
			{
				ERR_MSG(L"CMFCToolView::OnDraw Texture�� �ش� Ű�� �����ϴ�");
				return;
			}
			float fCenterX;
			float fCenterY;

			RECT rect{};

			if (pObjectInfo->bIsSingle)
				rect = pObjectInfo->tRect;
			else
			{
				auto& iter_Anim_find = m_pmapAnimation->find(pObjectInfo->cstrIdleAnimImage_ObjectKey + pObjectInfo->cstrIdleAnimImage_StateKey);
				if (iter_Anim_find == m_pmapAnimation->end())
				{
					ERR_MSG(L"Multi�� �����Ǿ������� �ִϸ��̼��� ã�� ���߽��ϴ�. MFCToolView::OnDraw");
					return;
				}
				rect = iter_Anim_find->second->vecRect[0];
			}

			if (pObjectInfo->bIsSingle && rect.bottom == 0)
			{
				rect = {};
				fCenterX = float((pTexInfo->tImageInfo.Width*0.5f));
				fCenterY = float((pTexInfo->tImageInfo.Height*0.5f));
			}
			else
			{
				fCenterX = float(((rect.right - rect.left)*0.5f));
				fCenterY = float(((rect.bottom - rect.top) * 0.5f));
			}

			D3DXMATRIX matScale, matTrans, matRotZ, matWorld;

			//�ʱ�ȭ
			D3DXMatrixIdentity(&matScale);
			D3DXMatrixIdentity(&matTrans);
			D3DXMatrixIdentity(&matRotZ);
			D3DXMatrixScaling(&matScale, pActorInfo->tInfo.vSize.x, pActorInfo->tInfo.vSize.y, 0.f);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-pActorInfo->tInfo.fAngle));
			if (pObjectInfo->eRenderId == RENDERID::BACKGROUND || pObjectInfo->eRenderId == RENDERID::UI)
				D3DXMatrixTranslation(&matTrans, pActorInfo->tInfo.vPos.x, pActorInfo->tInfo.vPos.y, 0.f);
			else
				D3DXMatrixTranslation(&matTrans, pActorInfo->tInfo.vPos.x - GetScrollPos(SB_HORZ), pActorInfo->tInfo.vPos.y - GetScrollPos(SB_VERT), 0.f);
			matWorld = matScale *matRotZ* matTrans;

			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
			CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, (rect.bottom == 0) ? nullptr : &rect, &D3DXVECTOR3{ fCenterX,fCenterY,0.f }, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			if (i == RENDERID::BACKGROUND)
				continue;
			CGraphic_Device::Get_Instance()->Get_Sprite()->End();
			const DWORD dwSize = 5;
			_vec2 v2LinePos[dwSize];
			if (pObjectInfo->bIsSingle)
			{
				if (pObjectInfo->tRect.bottom > 0)
				{
					v2LinePos[0] = { pActorInfo->tInfo.vPos.x - (rect.right - rect.left)*0.5f, pActorInfo->tInfo.vPos.y - (rect.bottom - rect.top)*0.5f };
					v2LinePos[1] = { pActorInfo->tInfo.vPos.x + (rect.right - rect.left)*0.5f, pActorInfo->tInfo.vPos.y - (rect.bottom - rect.top)*0.5f };
					v2LinePos[2] = { pActorInfo->tInfo.vPos.x + (rect.right - rect.left)*0.5f, pActorInfo->tInfo.vPos.y + (rect.bottom + rect.top)*0.5f };
					v2LinePos[3] = { pActorInfo->tInfo.vPos.x - (rect.right - rect.left)*0.5f, pActorInfo->tInfo.vPos.y + (rect.bottom + rect.top)*0.5f };

				}
				else
				{
					v2LinePos[0] = { pActorInfo->tInfo.vPos.x - pTexInfo->tImageInfo.Width*0.5f, pActorInfo->tInfo.vPos.y - pTexInfo->tImageInfo.Height*0.5f };
					v2LinePos[1] = { pActorInfo->tInfo.vPos.x + pTexInfo->tImageInfo.Width*0.5f, pActorInfo->tInfo.vPos.y - pTexInfo->tImageInfo.Height*0.5f };
					v2LinePos[2] = { pActorInfo->tInfo.vPos.x + pTexInfo->tImageInfo.Width*0.5f, pActorInfo->tInfo.vPos.y + pTexInfo->tImageInfo.Height*0.5f };
					v2LinePos[3] = { pActorInfo->tInfo.vPos.x - pTexInfo->tImageInfo.Width*0.5f, pActorInfo->tInfo.vPos.y + pTexInfo->tImageInfo.Height*0.5f };
				}				
			}
			else
			{

				v2LinePos[0] = { pActorInfo->tInfo.vPos.x - (rect.right - rect.left)*0.5f, pActorInfo->tInfo.vPos.y - (rect.bottom - rect.top)*0.5f };
				v2LinePos[1] = { pActorInfo->tInfo.vPos.x + (rect.right - rect.left)*0.5f, pActorInfo->tInfo.vPos.y - (rect.bottom - rect.top)*0.5f };
				v2LinePos[2] = { pActorInfo->tInfo.vPos.x + (rect.right - rect.left)*0.5f, pActorInfo->tInfo.vPos.y + (rect.bottom + rect.top)*0.5f };
				v2LinePos[3] = { pActorInfo->tInfo.vPos.x - (rect.right - rect.left)*0.5f, pActorInfo->tInfo.vPos.y + (rect.bottom + rect.top)*0.5f };
			}
			v2LinePos[0].x -= GetScrollPos(SB_HORZ);
			v2LinePos[1].x -= GetScrollPos(SB_HORZ);
			v2LinePos[2].x -= GetScrollPos(SB_HORZ);
			v2LinePos[3].x -= GetScrollPos(SB_HORZ);
			v2LinePos[0].y -= GetScrollPos(SB_VERT);
			v2LinePos[1].y -= GetScrollPos(SB_VERT);
			v2LinePos[2].y -= GetScrollPos(SB_VERT);
			v2LinePos[3].y -= GetScrollPos(SB_VERT);
			v2LinePos[4] = v2LinePos[0];

			m_mapCollisionToPrefab.emplace(RECT{ (LONG)v2LinePos[0].x, (LONG)v2LinePos[0].y,(LONG)v2LinePos[2].x, (LONG)v2LinePos[2].y }, PREFABSTRUCT{ pActorInfo,pObjectInfo,nullptr });
			CGraphic_Device::Get_Instance()->Get_Line()->Draw(v2LinePos, dwSize, D3DCOLOR_ARGB(255, 100, 255, 255));
			CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);

		}
	}
	Pivot.Render();
	CGraphic_Device::Get_Instance()->Render_End(GetSafeHwnd());
}

// CMFCToolView �μ�

BOOL CMFCToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMFCToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMFCToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CMFCToolView ����

#ifdef _DEBUG
void CMFCToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMFCToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMFCToolDoc* CMFCToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCToolDoc)));
	return (CMFCToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCToolView �޽��� ó����


void CMFCToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	
	//GetScrollPos()
	g_hWND = m_hWnd; 
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd()); 
	CForm* pForm = dynamic_cast<CForm*>(pMain->m_tLeftSplitter.GetPane(1, 0));
	m_pmapAnimation = &pForm->m_tAnimationTool.m_mapAnima;
	RECT rcMain = {}; 
	pMain->GetWindowRect(&rcMain);

	RECT rcView{}; 
	GetClientRect(&rcView);  

// 	rcMain.right = rcMain.right - rcMain.left; 
// 	rcMain.bottom = rcMain.bottom - rcMain.top; 
// 	rcMain.left = 0; 
// 	rcMain.right = 0; 
	SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);

	int iGapX = rcMain.right - rcView.right; 
	int iGapY = rcMain.bottom - rcView.bottom; 

	pMain->SetWindowPos(nullptr, 0, 0, CLIENTCX + iGapX, CLIENTCY + iGapY, SWP_NOMOVE);

	m_pHierarchyView = dynamic_cast<CHierarchyView*>(pMain->m_tRightSplitter.GetPane(0, 0));
	m_pmapPrefab = &(pForm->m_tObjectTool.m_mapObject);
	
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	//if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::MULTI_TEX, L"../Texture/Stage/Player/Attack/AKIHA_AKI01_00%d.png", L"Player", L"Attack", 6)))
	//	return; 
	//if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::MULTI_TEX, L"../Texture/Stage/Player/Dash/AKIHA_AKI13_00%d.png", L"Player", L"Dash", 11)))
	//	return;
	//if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::MULTI_TEX, L"../Texture/Stage/Terrain/Tile/Tile%d.png", L"Terrain", L"Tile", 38)))
	//	return;


	//if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::MULTI_TEX, L"../Texture/Stage/Effect/Mega_Explosion/sp_megaexplosion_01_%d.png", L"Effect", L"Mega_Explosion", 24)))
	//	return;

	//m_pTerrain = new CTerrain; 
	//if (FAILED(m_pTerrain->Ready_Terrain()))
	//	return; 
	//m_pTerrain->Set_View(this); 


}


void CMFCToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
// 	TCHAR szBuf[54] = L""; 
// 	swprintf_s(szBuf, L"X : %d , Y : %d", point.x, point.y); 
// 	ERR_MSG(szBuf); 
	D3DXVECTOR3 vMouse{ float(point.x) + GetScrollPos(SB_HORZ), float(point.y) + GetScrollPos(SB_VERT), 0.f }; 
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pForm = dynamic_cast<CForm*>(pMain->m_tLeftSplitter.GetPane(1, 0));
	CMapping* pMapping = dynamic_cast<CMapping*>(pMain->m_tLeftSplitter.GetPane(0, 0));
	CHierarchyView* pHierarchyView = dynamic_cast<CHierarchyView*>(pMain->m_tRightSplitter.GetPane(0, 0));
	//m_pTerrain->TilePicking_Terrain(vMouse, 2, 0);

	//pForm->m_tUiTool.PickingPos(vMouse);
	////���⼭ �浹 üũ 
	//pForm->m_tUiTool.Collision_Down(vMouse);
	CPoint pt = point;
	//ScreenToClient(&pt);
	pt.x += GetScrollPos(SB_HORZ);
	pt.y += GetScrollPos(SB_VERT);
	m_iPivotMoveing = Pivot.IsPtInRect(pt);

	if (m_iPivotMoveing == -1)
	{
		bool bActorSelected = false;
		for (auto& rPair : m_mapCollisionToPrefab)
		{
			if (!PtInRect(&rPair.first, point))
				continue;
			CTreeCtrl& tree = m_pHierarchyView->GetTreeCtrl();
			HTREEITEM curItem = tree.GetRootItem();

			tree.Expand(curItem, TVE_EXPAND);
			HTREEITEM childItem = tree.GetNextItem(curItem, TVGN_CHILD);
			while (childItem != NULL)
			{
				childItem = tree.GetNextItem(childItem, TVGN_NEXT);
				tree.Expand(childItem, TVE_EXPAND);
			}
			stack<HTREEITEM> stackParentItem;
			while (curItem != NULL)
			{
				CString cstrItem = tree.GetItemText(curItem);
				if (cstrItem == rPair.second.pActorInfo->wstrActorName)
					break;
				if (tree.ItemHasChildren(curItem))
				{
					stackParentItem.emplace(curItem);
					curItem = tree.GetNextItem(curItem, TVGN_CHILD);
				}
				else
				{
					curItem = tree.GetNextItem(curItem, TVGN_NEXT);
					if (curItem == NULL)
					{
						curItem = tree.GetNextItem(stackParentItem.top(), TVGN_NEXT);
						stackParentItem.pop();
					}
				}
			}

			if (curItem == NULL)
			{
				ERR_MSG(L"CMFCToolView::OnLButtonDown ������ �޴µ� Hierarchy���� �����ϴ�.(?!)");
				return;
			}

			tree.Select(curItem, TVGN_CARET);
			m_pSelectedActor = rPair.second.pActorInfo;
			bActorSelected = true;
			Pivot.Set_Pos({ (LONG)m_pSelectedActor->tInfo.vPos.x,(LONG)m_pSelectedActor->tInfo.vPos.y });
			break;
		}
		if (!bActorSelected)
		{
			if(pMapping->m_pSelectedPrefab)
				pHierarchyView->CreateNewPickedItem(pMapping->m_pSelectedPrefab, pt);
		}
	}
	else
	{
		//Pivot.Set_Pos(pt);
	}




	Invalidate(FALSE); 
	CScrollView::OnLButtonDown(nFlags, point);
}


void CMFCToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	D3DXVECTOR3 vMouse{ float(point.x) + GetScrollPos(SB_HORZ), float(point.y) + GetScrollPos(SB_VERT), 0.f };
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pForm = dynamic_cast<CForm*>(pMain->m_tLeftSplitter.GetPane(1, 0));
	CPoint pt = point;
	//ScreenToClient(&pt);
	pt.x += GetScrollPos(SB_HORZ);
	pt.y += GetScrollPos(SB_VERT);
//	pForm->m_tUiTool.Collision_Move(vMouse);
	CPoint tDiff;
	switch (m_iPivotMoveing)
	{
	case CPivot::X:
		tDiff = pt - Pivot.Get_Pos();
		m_pSelectedActor->tInfo.vPos.x += tDiff.x;
		Pivot.Add_PosX(tDiff.x);
		break;
	case CPivot::Y:
		tDiff = pt - Pivot.Get_Pos();
		m_pSelectedActor->tInfo.vPos.y += tDiff.y;
		Pivot.Add_PosY(tDiff.y);
		break;
	case CPivot::XY:
		tDiff = pt - Pivot.Get_Pos();
		m_pSelectedActor->tInfo.vPos.x += tDiff.x;
		m_pSelectedActor->tInfo.vPos.y += tDiff.y;
		Pivot.Add_PosX(tDiff.x);
		Pivot.Add_PosY(tDiff.y);
		break;
	default:
		break;
	}
	Invalidate(FALSE);
	CScrollView::OnMouseMove(nFlags, point);
}


void CMFCToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	D3DXVECTOR3 vMouse{ float(point.x) + GetScrollPos(SB_HORZ), float(point.y) + GetScrollPos(SB_VERT), 0.f };
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pForm = dynamic_cast<CForm*>(pMain->m_tLeftSplitter.GetPane(1, 0));

	//pForm->m_tUiTool.Collision_Up();
	CPoint pt = point;
	//ScreenToClient(&pt);
	pt.x += GetScrollPos(SB_HORZ);
	pt.y += GetScrollPos(SB_VERT);
	CPoint tDiff;
	switch (m_iPivotMoveing)
	{
	case CPivot::X:
		tDiff = pt - Pivot.Get_Pos();
		m_pSelectedActor->tInfo.vPos.x += tDiff.x;
		Pivot.Add_PosX(tDiff.x);
		break;
	case CPivot::Y:
		tDiff = pt - Pivot.Get_Pos();
		m_pSelectedActor->tInfo.vPos.y += tDiff.y;
		Pivot.Add_PosY(tDiff.y);
		break;
	case CPivot::XY:
		tDiff = pt - Pivot.Get_Pos();
		m_pSelectedActor->tInfo.vPos.x += tDiff.x;
		m_pSelectedActor->tInfo.vPos.y += tDiff.y;
		Pivot.Add_PosX(tDiff.x);
		Pivot.Add_PosY(tDiff.y);
		break;
	default:
		break;
	}
	m_iPivotMoveing = -1;
	Invalidate(FALSE);
	CScrollView::OnLButtonUp(nFlags, point);
}
