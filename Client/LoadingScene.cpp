#include "stdafx.h"
#include "LoadingScene.h"


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

	InitializeCriticalSection(&m_tCritical_Section);
	return S_OK;
}

void CLoadingScene::Update_Scene()
{
}

void CLoadingScene::InitUpdate_Scene()
{
}

void CLoadingScene::Render_Scene()
{
}

void CLoadingScene::Release_Scene()
{
	DeleteCriticalSection(&m_tCritical_Section);
}

void CLoadingScene::Set_LoadingStage(const CScene_Manager::ID & _eId)
{
	m_eLoadingStage = _eId;
}
