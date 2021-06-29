#include "stdafx.h"
#include "Spawn_Manager.h"
#include "Player.h"
#include "Bullet.h"
#include "Effect.h"
#include "BackGround.h"
#include "Gui.h"
#include "GameObject_Manager.h"

CSpawn_Manager::CSpawn_Manager()
{
}


CSpawn_Manager::~CSpawn_Manager()
{
}

HRESULT CSpawn_Manager::Spawn(const wstring _wstrObjName, const ACTORINFO * _pPlacement, const OBJECTINFO * _pObjectInfo)
{
	CGameObject* pObject = nullptr;
	if (_wstrObjName == L"Player")
	{
		pObject = CPlayer::Create(_pObjectInfo, _pPlacement);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)_pObjectInfo->eObjId, pObject);
		pObject = nullptr;
	}
	else if (_wstrObjName == L"Monster")
	{

	}
	else if (!_pObjectInfo)
	{
		switch ((RENDERID::ID)_pPlacement->eRenderID)
		{
		case RENDERID::BACKGROUND:
			pObject = CBackGround::Create(_pPlacement);
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::BACKGROUND), pObject);
			pObject = nullptr;
			break;
		case RENDERID::UI:
			pObject = CGui::Create(_pPlacement);
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::UI), pObject);
			pObject = nullptr;
			break;
		default:
			break;
		}
	}

	return S_OK;
}
