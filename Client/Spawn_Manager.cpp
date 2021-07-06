#include "stdafx.h"
#include "Spawn_Manager.h"
#include "Player.h"
#include "Bullet.h"
#include "Effect.h"
#include "BackGround.h"
#include "Gui.h"
#include "GameObject_Manager.h"
#include "Enemy.h"

CSpawn_Manager::CSpawn_Manager()
{
}


CSpawn_Manager::~CSpawn_Manager()
{
}

HRESULT CSpawn_Manager::Spawn(const wstring _wstrObjName, const ACTORINFO * _pActorInfo, const OBJECTINFO * _pObjectInfo)
{
	wstring wstrPrefab = _pObjectInfo->wstrPrefabName;
	if (wstrPrefab == L"Ruler_Body")
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)_pObjectInfo->eObjId, CEnemy::Create(_pObjectInfo, _pActorInfo));
	}


	return S_OK;
}
