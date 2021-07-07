#include "stdafx.h"
#include "GameObject.h"


CGameObject::CGameObject()
	:m_pObjectInfo(nullptr)
	, m_uiAnimationFrame(0)
	, m_tInfo({})
{
	
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

void CGameObject::Set_ActorInfo(const ACTORINFO * _pPlacement)
{
	m_pActorInfo = _pPlacement;
}

void CGameObject::UpdateState()
{
}

void CGameObject::UpdateAnimation()
{
	
}
