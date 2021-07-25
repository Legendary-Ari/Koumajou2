#include "stdafx.h"
#include "LogoScene.h"
#include "Fade.h"

CLogoScene::CLogoScene()
	:m_idx(0)
{
}


CLogoScene::~CLogoScene()
{
}

CScene * CLogoScene::Create()
{
	CScene* pInstance = new CLogoScene;
	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Delete(pInstance);
		return pInstance;
	}
	return pInstance;
}

HRESULT CLogoScene::Ready_Scene()
{
	CGameObject_Manager::Get_Instance()->Release_GameObject_Manager();
	CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::SINGLE_TEX, { { 0,0,0,0 } }, L"../Image/Logo.png", L"Logo");
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::BACKGROUND, CFade::Create(true));
	return S_OK;
}

void CLogoScene::Update_Scene()
{
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_Z))
	{
		switch (m_idx)
		{
			//1-1
		case 0:
			CScene_Manager::g_eStartScene = CScene_Manager::STAGE_1_1;
			break;
			//1-4
		case 1:
			CScene_Manager::g_eStartScene = CScene_Manager::STAGE_1_4;
			break;
			//2-4
		case 2:
			CScene_Manager::g_eStartScene = CScene_Manager::STAGE_2_4;
			break;
		default:
			break;
		}
		CScene_Manager::Get_Instance()->Change_Scene_Manager(CScene_Manager::SELECT);
		return;
	}
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_LEFT))
	{
		--m_idx;
		if (m_idx < 0)
			m_idx = 2;
	}
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_RIGHT))
	{
		++m_idx;
		if (m_idx > 2)
			m_idx = 0;
	}
}

void CLogoScene::InitUpdate_Scene()
{
}

void CLogoScene::Render_Scene()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"Logo");
	if (nullptr == pTexInfo)
		return;

	D3DXMATRIX matScale, matTrans, matWorld;

	D3DXMatrixScaling(&matScale, 0.75f, 0.75f, 0.0f);
	D3DXMatrixTranslation(&matTrans, float(CLIENTCX >> 1), float(CLIENTCY >> 1), 0.f);

	matWorld = matScale * matTrans;
	float 	fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float 	fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	CGameObject_Manager::Get_Instance()->Render_GameObject_Manager();

	{
		const OBJECTINFO* pObjectinfo = nullptr;
		switch (m_idx)
		{
			//1-1
		case 0:
			pObjectinfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"GameStart");
			break;
			//1-4
		case 1:
			pObjectinfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Stage1");
			break;
			//2-4
		case 2:
			pObjectinfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Stage2");
			break;
		default:
			break;
		}
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(pObjectinfo->wstrObjectImage_ObjectKey);
		if (nullptr == pTexInfo)
			return;

		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixTranslation(&matTrans, float(CLIENTCX >> 1), float((CLIENTCY >> 2) * 3.f), 0.f);
		matWorld = matTrans;
		RECT rect = pObjectinfo->tRect;
		float 	fCenterX = float((rect.right - rect.left) * 0.5f);
		float 	fCenterY = float((rect.bottom - rect.top) * 0.5f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		CGameObject_Manager::Get_Instance()->Render_GameObject_Manager();
	}

}

void CLogoScene::Release_Scene()
{
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::BACKGROUND, CFade::Create(false));
}
