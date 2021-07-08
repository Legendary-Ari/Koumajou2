#include "stdafx.h"
#include "GameObject.h"


CGameObject::CGameObject()
	:m_pObjectInfo(nullptr)
	, m_uiAnimationFrame(0)
	, m_tInfo({})
	, m_bIsFliped(false)
	, m_bRenderCollision(true)
{
	
}


CGameObject::~CGameObject()
{
}

void CGameObject::Set_Pos(const D3DXVECTOR3 & vPos)
{
	m_tInfo.vPos = vPos; 
}

void CGameObject::OnOverlaped(CGameObject * _pHitObject)
{
}

void CGameObject::OnBlocked(CGameObject* pHitObject,DIRECTION::ID _eId)
{
}

void CGameObject::OnBlockedTile(CGameObject * pHitObject, DIRECTION::ID _eId)
{
}

void CGameObject::Set_Prefab(const OBJECTINFO * _pPrefab)
{
	m_pObjectInfo = _pPrefab;
}

void CGameObject::Set_ActorInfo(const ACTORINFO * _pPlacement)
{
	m_pActorInfo = _pPlacement;
}

void CGameObject::Add_PosX(float _fPosX)
{
	m_tInfo.vPos.x += _fPosX;
}

void CGameObject::Add_PosY(float _fPosY)
{
	m_tInfo.vPos.y += _fPosY;
}

void CGameObject::UpdateState()
{
}

void CGameObject::UpdateAnimation()
{
	
}

void CGameObject::UpdateBodyCollision()
{
}

void CGameObject::UpdateAttackCollision()
{
}

void CGameObject::RenderCollision()
{
	if (!m_bRenderCollision)
		return;
	CGraphic_Device::Get_Instance()->Get_Sprite()->End();
	for (auto& tCollision : m_vecBodyCollision)
	{
		D3DXVECTOR2* v2LinePos;
		DWORD dwSize = 0;
		if (tCollision.eId == COLLISION::C_RECT)
		{
			dwSize = 5;
			v2LinePos = new _vec2[dwSize];
			v2LinePos[0] = { (float)(tCollision.tRect.left), (float)(tCollision.tRect.top) };
			v2LinePos[1] = { (float)(tCollision.tRect.right),(float)( tCollision.tRect.top) };
			v2LinePos[2] = { (float)(tCollision.tRect.right),(float)( tCollision.tRect.bottom) };
			v2LinePos[3] = { (float)(tCollision.tRect.left), (float)(tCollision.tRect.bottom) };
			v2LinePos[4] = v2LinePos[0];
		}
		else
		{
			_vec2 v2Center = { (tCollision.tRect.right + tCollision.tRect.left) * 0.5f, (tCollision.tRect.top + tCollision.tRect.bottom) * 0.5f };
			float fRadius = (tCollision.tRect.right - tCollision.tRect.left) * 0.5f;
			dwSize = 9;
			v2LinePos = new _vec2[dwSize];
			for (int i = 0; i < int(dwSize-1); ++i)
			{
				v2LinePos[i] = v2Center + fRadius * _vec2{cosf(D3DXToRadian(- i * 45.f)),sinf(D3DXToRadian(-i * 45.f))};
			}
			v2LinePos[8] = v2LinePos[0];
			
		}

		for (DWORD i = 0; i < dwSize; ++i)
		{
			_vec3 vScroll = CScroll_Manager::Get_Scroll();
			v2LinePos[i] += {vScroll.x,vScroll.y};
		}
			
		CGraphic_Device::Get_Instance()->Get_Line()->Draw(v2LinePos, dwSize, D3DCOLOR_ARGB(255, 100, 255, 255));
	}
	for (auto& tCollision : m_vecAttackCollision)
	{

		D3DXVECTOR2* v2LinePos;
		DWORD dwSize = 0;
		if (tCollision.eId == COLLISION::C_RECT)
		{
			dwSize = 5;
			v2LinePos = new _vec2[dwSize];
			v2LinePos[0] = {(float)( tCollision.tRect.left), (float)(tCollision.tRect.top) };
			v2LinePos[1] = {(float)( tCollision.tRect.right),(float)( tCollision.tRect.top) };
			v2LinePos[2] = {(float)( tCollision.tRect.right),(float)( tCollision.tRect.bottom) };
			v2LinePos[3] = {(float)( tCollision.tRect.left), (float)(tCollision.tRect.bottom) };
			v2LinePos[4] = v2LinePos[0];
		}
		else
		{
			_vec2 v2Center = { (tCollision.tRect.right + tCollision.tRect.left) * 0.5f, (tCollision.tRect.top + tCollision.tRect.bottom) * 0.5f };
			float fRadius = (tCollision.tRect.right - tCollision.tRect.left) * 0.5f;
			dwSize = 9;
			v2LinePos = new _vec2[dwSize];
			for (int i = 0; i < int(dwSize-1); ++i)
			{
				v2LinePos[i] = v2Center + fRadius * _vec2{ cosf(D3DXToRadian(-i * 45.f)),sinf(D3DXToRadian(-i * 45.f)) };
			}
			v2LinePos[8] = v2LinePos[0];

		}
		for (DWORD i = 0; i < dwSize; ++i)
		{
			_vec3 vScroll = CScroll_Manager::Get_Scroll();
			v2LinePos[i] += {vScroll.x, vScroll.y};
		}
		CGraphic_Device::Get_Instance()->Get_Line()->Draw(v2LinePos, dwSize, D3DCOLOR_ARGB(255, 255, 100, 100));
		
	}
	CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);
}

