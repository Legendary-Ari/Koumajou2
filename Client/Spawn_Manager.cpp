#include "stdafx.h"
#include "Spawn_Manager.h"
#include "Player.h"
#include "Bullet.h"
#include "Effect.h"
#include "BackGround.h"
#include "Gui.h"
#include "GameObject_Manager.h"
#include "Enemy.h"
#include "Ruler_Body.h"
#include "BackCollision.h"
#include "Toad.h"
#include "Skeleton.h"
#include "Rose.h"
#include "Owl.h"
#include "AluraUne.h"
#include "Pillor.h"

CSpawn_Manager::CSpawn_Manager()
{
}


CSpawn_Manager::~CSpawn_Manager()
{
}

HRESULT CSpawn_Manager::Spawn(const wstring _wstrObjName, const ACTORINFO * _pActorInfo, const OBJECTINFO * _pObjectInfo)
{
	wstring wstrPrefab = _pObjectInfo->wstrPrefabName;
	
	if (wstrPrefab == L"BG_Stage1-1")
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)_pObjectInfo->eObjId, CBackGround::Create(_pObjectInfo));
	}
	else if (wstrPrefab == L"bg05")
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)_pObjectInfo->eObjId, CBackGround::Create(_pObjectInfo));
	}
	else if (wstrPrefab == L"InvisibleBlock")
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)_pObjectInfo->eObjId, CBackCollision::Create(_pActorInfo, _pObjectInfo));
	}
	else if (wstrPrefab == L"s01g03")
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)_pObjectInfo->eObjId, CBackCollision::Create(_pActorInfo, _pObjectInfo));
	}
	else if (wstrPrefab == L"s01g03b")
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)_pObjectInfo->eObjId, CBackCollision::Create(_pActorInfo, _pObjectInfo));
	}
	else if (wstrPrefab == L"Alura_Une")
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)_pObjectInfo->eObjId, CAluraUne::Create(_pActorInfo, _pObjectInfo));
	}
	else if (wstrPrefab == L"s02o03")
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)_pObjectInfo->eObjId, CPillor::Create(_pActorInfo, _pObjectInfo));
	}
	else if (wstrPrefab == L"Toad")
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)_pObjectInfo->eObjId, CToad::Create(_pActorInfo, _pObjectInfo));
	}
	else if (wstrPrefab == L"Rose")
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)_pObjectInfo->eObjId, CRose::Create(_pActorInfo, _pObjectInfo));
	}
	else if (wstrPrefab == L"Skeleton")
	{
		//CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)_pObjectInfo->eObjId, CSkeleton::Create(_pActorInfo, _pObjectInfo));
	}
	else if (wstrPrefab == L"Owl")
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)_pObjectInfo->eObjId, COwl::Create(_pActorInfo, _pObjectInfo));
	}
	else if (wstrPrefab == L"Alura_Une")
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)_pObjectInfo->eObjId, CAluraUne::Create(_pActorInfo, _pObjectInfo));
	}
	else if (wstrPrefab == L"Une")
	{
		//CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)_pObjectInfo->eObjId, CAluraUne::Create(_pActorInfo, _pObjectInfo));
	}




	return S_OK;
}

ACTORINFO CSpawn_Manager::CreateEmptyInfo(const INFO & _tInfo, wstring _wstrPrefabName)
{
	return ACTORINFO{ _tInfo, false, _wstrPrefabName, L"" };
}
