#include "stdafx.h"
#include "GameOverScene.h"
#include "Fade.h"

CGameOverScene::CGameOverScene()
	:m_fSceneTime(5.f)
{
}


CGameOverScene::~CGameOverScene()
{
}

CScene * CGameOverScene::Create()
{
	CScene* pInstance = new CGameOverScene;
	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Delete(pInstance);
		return pInstance;
	}
	return pInstance;
}

HRESULT CGameOverScene::Ready_Scene()
{
	CGameObject_Manager::Get_Instance()->Release_GameObject_Manager();
	CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::SINGLE_TEX, { {0,0,0,0} }, L"../Image/Game Over.png", L"Game Over");
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::BACKGROUND, CFade::Create(true));
	return S_OK;
}

void CGameOverScene::Update_Scene()
{
	CGameObject_Manager::Get_Instance()->Update_GameObject_Manager();
	m_fSceneTime -= CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_fSceneTime < 0)
	{
		CScene_Manager::Get_Instance()->Change_Scene_Manager(CScene_Manager::SELECT);
		return;
	}
}

void CGameOverScene::InitUpdate_Scene()
{
}

void CGameOverScene::Render_Scene()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"Game Over");
	if (nullptr == pTexInfo)
		return;

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixTranslation(&matTrans, float(CLIENTCX >> 1), float(CLIENTCY >> 1), 0.f);
	matWorld = matTrans;
	float 	fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float 	fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	CGameObject_Manager::Get_Instance()->Render_GameObject_Manager();
}

void CGameOverScene::Release_Scene()
{
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::BACKGROUND, CFade::Create(false));	
}
