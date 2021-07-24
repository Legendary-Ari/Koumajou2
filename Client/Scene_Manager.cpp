#include "stdafx.h"
#include "Scene_Manager.h"
#include "SceneWeaponSelect.h"
#include "LoadingScene.h"
#include "Stage1_1.h"
#include "Stage1_4.h"
#include "Stage2_4.h"
IMPLEMENT_SINGLETON(CScene_Manager)
CScene_Manager::CScene_Manager()
	:m_eCurScene(END)
	, m_eNextScene(END)
	, m_pScene(nullptr)
{
}


CScene_Manager::~CScene_Manager()
{
	Release_Scene_Manager(); 
}

HRESULT CScene_Manager::Change_Scene_Manager(const ID eID, const ID eNextId)
{
	m_eNextScene = eID; 
	if (m_eCurScene != m_eNextScene)
	{

		Safe_Delete(m_pScene); 
		switch (m_eNextScene)
		{
		case CScene_Manager::SCENE_LOADING:
			m_pScene = CLoadingScene::Create(eNextId);
			break;
		case CScene_Manager::MENU:
			break;
		case CScene_Manager::SELECT:
			m_pScene = CSceneWeaponSelect::Create();
			break;
		case CScene_Manager::STAGE_1_1:
			if (m_ePrevScene != m_eNextScene)
			{
			}
			m_pScene = CStage1_1::Create();
			break;
		case CScene_Manager::STAGE_1_4:
			if (m_ePrevScene != m_eNextScene)
			{
				CSoundMgr::Get_Instance()->StopAll();
				CSoundMgr::Get_Instance()->PlayBGM(L"Stage 1-4.mp3");
			}
			m_pScene = CStage1_4::Create();
			break;
		//case CScene_Manager::STAGE_2_1:
		//	m_pScene = CStage2_1::Create();
		//	break;
		case CScene_Manager::STAGE_2_4:
			if (m_ePrevScene != m_eNextScene)
			{
				CSoundMgr::Get_Instance()->StopAll();
				CSoundMgr::Get_Instance()->PlayBGM(L"Stage 2-4.mp3");
			}
			m_pScene = CStage2_4::Create();
			break;
		default:
			m_pScene = CStage1_1::Create();
			break;
		}
		m_pScene->InitUpdate_Scene();
		m_ePrevScene = m_eCurScene;
		m_eCurScene = m_eNextScene; 
	}
	return S_OK;
}

void CScene_Manager::Update_Scene_Manager()
{
	m_pScene->Update_Scene(); 
	CScroll_Manager::UpdateScroll();
}

void CScene_Manager::InitUpdate_Scene_Manager()
{
	m_pScene->InitUpdate_Scene();
}

void CScene_Manager::Render_Scene_Manager()
{
	m_pScene->Render_Scene(); 
}

void CScene_Manager::Release_Scene_Manager()
{
	Safe_Delete(m_pScene); 
}

const _vec3 & CScene_Manager::Get_StartPos() const
{
	return m_pScene->Get_StartPos();
}

HRESULT CScene_Manager::Reset()
{
	//Safe_Delete(m_pScene);
	//m_pScene = CLoadingScene::Create(m_eCurScene);
	//m_pScene->InitUpdate_Scene();
	//m_eNextScene = m_eCurScene = CScene_Manager::SCENE_LOADING;
	Change_Scene_Manager(CScene_Manager::SCENE_LOADING, m_eCurScene);

	return S_OK;
}
