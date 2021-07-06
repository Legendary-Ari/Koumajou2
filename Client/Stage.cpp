#include "stdafx.h"
#include "Stage.h"
#include "Terrain.h"
#include "Prefab_Manager.h"
#include "Player.h"

CStage::CStage()
{
}


CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene()
{
	CGameObject* pObject = nullptr;
	//pObject =	CTerrain::Create();
	//CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::BACKGROUND, pObject);
	//pObject = nullptr;

	CPrefab_Manager::Get_Instance()->SpawnObjectbyScene(CScene_Manager::STAGE_1_1);


	return S_OK;
}

void CStage::Update_Scene()
{
	CGameObject_Manager::Get_Instance()->Update_GameObject_Manager(); 
}

void CStage::Render_Scene()
{
	CGameObject_Manager::Get_Instance()->Render_GameObject_Manager();
}

void CStage::Release_Scene()
{
}

CScene * CStage::Create()
{
	CScene* pInstance = new CStage; 
	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Delete(pInstance);
		return pInstance;
	}
	return pInstance;
}
