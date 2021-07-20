#include "stdafx.h"
#include "Stage1_4.h"
#include "Prefab_Manager.h"
#include "Player.h"
#include "StageUi.h"
#include "BossAlice.h"
#include "BossHpUi.h"

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
	m_vStartPos = { 50.f,500.f,0.f };
	CPrefab_Manager::Get_Instance()->SpawnObjectbyScene(CScene_Manager::STAGE_1_4);

	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::UI, CStageUi::Create());

	const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"BossAlice");
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::BOSS, CBossAlice::Create(pObjectInfo));
	if (!CGameObject_Manager::Get_Instance()->Get_Player())
	{
		const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Player");
		const ACTORINFO* pActorInfo = CPrefab_Manager::Get_Instance()->Get_PlacementPrefab(L"Player");
		if (pActorInfo)
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, CPlayer::Create(pObjectInfo, pActorInfo->tInfo));
		else
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, CPlayer::Create(pObjectInfo, { {50.f,500.f,0.f},{0.f,0.f,0.f},{1.f,1.f,0.f},0.f }));
	}
	{
		const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"BossHpBar");
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::UI, CBossHpUi::Create(pObjectInfo));
	}

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
	CGameObject_Manager::Get_Instance()->Release_GameObject_Manager();
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
