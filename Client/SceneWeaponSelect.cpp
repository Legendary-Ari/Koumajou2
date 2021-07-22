#include "stdafx.h"
#include "SceneWeaponSelect.h"
#include "WeaponSelectUi.h"
#include "BackGround.h"
#include "BGSelectCastle.h"
#include "WeaponSelectCursor.h"
#include "WeaponSelectedUi.h"

CSceneWeaponSelect::CSceneWeaponSelect()
{
}


CSceneWeaponSelect::~CSceneWeaponSelect()
{
	Release_Scene();
}

HRESULT CSceneWeaponSelect::Ready_Scene()
{
	{
		const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"SelectBG");
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, CBackGround::Create(pObjectInfo));
	}
	{
		const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"SelectBG2");
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, CBGSelectCastle::Create(pObjectInfo));
	}
	{
		const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"WeaponSelected");
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, CWeaponSelectedUi::Create(pObjectInfo));
	}
	{
		const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"WeaponSelectBar");
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, CWeaponSelectUi::Create(pObjectInfo));
	}
	
	{
		const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"WeaponSelectRing");
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, CWeaponSelectCursor::Create(pObjectInfo));
	}
	
	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"A Night.mp3");

	return S_OK;
}

void CSceneWeaponSelect::InitUpdate_Scene()
{
	CGameObject_Manager::Get_Instance()->InitUpdate_GameObject_Manager();
}

void CSceneWeaponSelect::Update_Scene()
{
	CGameObject_Manager::Get_Instance()->Update_GameObject_Manager();
}

void CSceneWeaponSelect::Render_Scene()
{
	CGameObject_Manager::Get_Instance()->Render_GameObject_Manager();
}

void CSceneWeaponSelect::Release_Scene()
{
	CSoundMgr::Get_Instance()->StopAll();
	CGameObject_Manager::Get_Instance()->Release_StageObject();
}

CScene * CSceneWeaponSelect::Create()
{
	CScene* pInstance = new CSceneWeaponSelect;
	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Delete(pInstance);
		return pInstance;
	}
	return pInstance;
}
