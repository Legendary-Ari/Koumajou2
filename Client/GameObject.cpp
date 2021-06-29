#include "stdafx.h"
#include "GameObject.h"


CGameObject::CGameObject()
	:m_pObjectInfo(nullptr)
	//:m_tInfo({})
{
	ZeroMemory(&m_tInfo, sizeof(INFO)); 
}


CGameObject::~CGameObject()
{
}

void CGameObject::Set_Pos(const D3DXVECTOR3 & vPos)
{
	m_tInfo.vPos = vPos; 
}

void CGameObject::Set_Prefab(const OBJECTINFO * _pPrefab)
{
	m_pObjectInfo = _pPrefab;
}

void CGameObject::Set_Placement(const ACTORINFO * _pPlacement)
{
	m_pPlacement = _pPlacement;
}
