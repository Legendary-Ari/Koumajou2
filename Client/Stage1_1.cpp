#include "stdafx.h"
#include "Stage1_1.h"
#include "Prefab_Manager.h"
#include "Player.h"
#include "StageUi.h"
#include "SceneChanger.h"

CStage1_1::CStage1_1()
{
}


CStage1_1::~CStage1_1()
{
	Release_Scene();
}

HRESULT CStage1_1::Ready_Scene()
{
	CGameObject* pObject = nullptr;
	//pObject =	CTerrain::Create();
	//CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::BACKGROUND, pObject);
	//pObject = nullptr;

	m_vStartPos = { 200.f, 300.f, 0.f };
	//CPrefab_Manager::Get_Instance()->SpawnObjectbyScene(CScene_Manager::STAGE_1_1);
	if (!CGameObject_Manager::Get_Instance()->Get_Player())
	{
		const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Player");
		const ACTORINFO* pActorInfo = CPrefab_Manager::Get_Instance()->Get_PlacementPrefab(L"Player");
		if (pActorInfo)
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, CPlayer::Create(pObjectInfo, pActorInfo->tInfo));

	}
	CGameObject_Manager::Get_Instance()->Get_Player()->Set_Pos(m_vStartPos);


	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::UI, CStageUi::Create());
	_vec3 SrcPos = { 4650.f, 400.f, 0.f };
	_vec3 DstPos = { 50.f, 500.f, 0.f };
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::COLLISION, CSceneChanger::Create(SrcPos,DstPos,CScene_Manager::STAGE_1_4));

	return S_OK;
}

void CStage1_1::InitUpdate_Scene()
{
	CGameObject_Manager::Get_Instance()->InitUpdate_GameObject_Manager();
}

void CStage1_1::Update_Scene()
{
	CGameObject_Manager::Get_Instance()->Update_GameObject_Manager(); 
}

void CStage1_1::Render_Scene()
{
	CGameObject_Manager::Get_Instance()->Render_GameObject_Manager();
}

void CStage1_1::Release_Scene()
{
	CGameObject_Manager::Get_Instance()->Release_StageObject();
}

CScene * CStage1_1::Create()
{
	CScene* pInstance = new CStage1_1; 
	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Delete(pInstance);
		return pInstance;
	}
	return pInstance;
}
