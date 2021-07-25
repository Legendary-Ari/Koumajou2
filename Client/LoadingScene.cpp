#include "stdafx.h"
#include "LoadingScene.h"
#include "Fade.h"

CLoadingScene::CLoadingScene()
	:m_bLoaded(false)
{
}


CLoadingScene::~CLoadingScene()
{
	Release_Scene();
}

CScene * CLoadingScene::Create(const CScene_Manager::ID & _eId)
{
	CScene* pInstance = new CLoadingScene;
	static_cast<CLoadingScene*>(pInstance)->Set_LoadingStage(_eId);
	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Delete(pInstance);
		return pInstance;
	}
	return pInstance;
}

HRESULT CLoadingScene::Ready_Scene()
{
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::BACKGROUND, CFade::Create(true));
	InitializeCriticalSection(&m_tCritical_Section);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadActor, this, 0, nullptr);
	return S_OK;
}

void CLoadingScene::Update_Scene()
{
	if (m_bLoaded)
	{
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
		DeleteCriticalSection(&m_tCritical_Section);
		CScene_Manager::Get_Instance()->Change_Scene_Manager(m_eLoadingStage);
		return;
	}
}

void CLoadingScene::InitUpdate_Scene()
{
}

void CLoadingScene::Render_Scene()
{
	const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"SelectBG");
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(pObjectInfo->wstrObjectImage_ObjectKey);
	if (nullptr == pTexInfo)
		return;
	RECT rect = pObjectInfo->tRect;
	float 	fCenterX = float(((rect.right - rect.left) * 0.5f));
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));

	_matrix matTrans;
	D3DXMatrixTranslation(&matTrans, float(CLIENTCX >> 1), float(CLIENTCY >> 1), 0.f);

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &_vec3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CLoadingScene::Release_Scene()
{
}

void CLoadingScene::Set_LoadingStage(const CScene_Manager::ID & _eId)
{
	m_eLoadingStage = _eId;
}

unsigned CLoadingScene::LoadActor(LPVOID pVoid)
{
	CLoadingScene* pScene = static_cast<CLoadingScene*>(pVoid);
	EnterCriticalSection(&pScene->m_tCritical_Section);
	CPrefab_Manager::Get_Instance()->SpawnObjectbyScene(pScene->m_eLoadingStage);
	pScene->m_bLoaded = true;
	LeaveCriticalSection(&pScene->m_tCritical_Section);
	return 0;
}
