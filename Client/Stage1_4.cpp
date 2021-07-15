#include "stdafx.h"
#include "Stage1_4.h"
#include "Prefab_Manager.h"
#include "Player.h"
#include "StageUi.h"

CStage1_4::CStage1_4()
{
}


CStage1_4::~CStage1_4()
{
	Release_Scene();
}

HRESULT CStage1_4::Ready_Scene()
{
	CGameObject* pObject = nullptr;
	//pObject =	CTerrain::Create();
	//CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::BACKGROUND, pObject);
	//pObject = nullptr;

	CPrefab_Manager::Get_Instance()->SpawnObjectbyScene(CScene_Manager::STAGE_1_4);

	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::UI, CStageUi::Create());

	return S_OK;
}

void CStage1_4::InitUpdate_Scene()
{
	CGameObject_Manager::Get_Instance()->InitUpdate_GameObject_Manager();
}

void CStage1_4::Update_Scene()
{
	CGameObject_Manager::Get_Instance()->Update_GameObject_Manager(); 
}

void CStage1_4::Render_Scene()
{
	CGameObject_Manager::Get_Instance()->Render_GameObject_Manager();
}

void CStage1_4::Release_Scene()
{
	CGameObject_Manager::Get_Instance()->Release_StageObject();
}

CScene * CStage1_4::Create()
{
	CScene* pInstance = new CStage1_4;
	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Delete(pInstance);
		return pInstance;
	}
	return pInstance;
}
