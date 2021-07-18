#include "stdafx.h"
#include "GameObject.h"
#include "Effect.h"

bool CGameObject::m_bRenderCollision = false;
CGameObject::CGameObject()
	:m_pActorInfo(nullptr)
	, m_pObjectInfo(nullptr)
	, m_uiAnimationFrame(0)
	, m_tInfo({})
	, m_bFliped(false)
	, m_bVisible(true)
	, m_bBlockable(true)
	, m_bOverlapable(true)
	, m_bDead(false)
	, m_eCurState(-1)
	, m_ePrevState(-1)
	, m_fAnimationCumulatedTime(0.f)
	, m_bDieInit(false)
	, m_bFalling(false)
	, m_bJumping(false)
	, m_fGravityAccelerlation(29.4f)
	, m_fMaxCoolDownDieEffectTime(0.15f)
	, m_fCoolDownDieEffectRemainTime(0.f)
{
	
}


CGameObject::~CGameObject()
{
}

void CGameObject::Set_Pos(const D3DXVECTOR3 & vPos)
{
	m_tInfo.vPos = vPos; 
}

void CGameObject::OnOverlaped(CGameObject * _pHitObject, _vec3 vHitPos)
{
	m_fCurHp -= _pHitObject->Get_Damage();
	if (m_fCurHp <= 0.f)
	{
		m_bDead = true;
		m_bDieInit = true;
	}

}

void CGameObject::OnBlocked(CGameObject* pHitObject,DIRECTION::ID _eId)
{
}

void CGameObject::OnBlockedTile(CGameObject * pHitObject, DIRECTION::ID _eId)
{
	if (!m_bOnGround && _eId == DIRECTION::S /*|| _eId == DIRECTION::SW || _eId == DIRECTION::SE*/)
	{
		m_bFalling = false;
		m_bJumping = false;
		m_bOnGround = true;
		m_tInfo.vDir.y = 0;
	}
}

void CGameObject::Set_OnGround(bool _b)
{
}

void CGameObject::Set_Prefab(const OBJECTINFO * _pPrefab)
{
	m_pObjectInfo = _pPrefab;
	m_fCurHp = _pPrefab->fMaxHp;
}

void CGameObject::Set_ActorInfo(const ACTORINFO * _pPlacement)
{
	m_pActorInfo = _pPlacement;
	m_tInfo = _pPlacement->tInfo;
}

void CGameObject::Set_Info(const INFO & tInfo)
{
	m_tInfo = tInfo;
}

void CGameObject::Set_Angle(float _fAngle)
{
	m_tInfo.fAngle = _fAngle;
}

void CGameObject::Add_PosX(float _fPosX)
{
	m_tInfo.vPos.x += _fPosX;
}

void CGameObject::Add_PosY(float _fPosY)
{
	m_tInfo.vPos.y += _fPosY;
}

const RENDERID::ID & CGameObject::Get_RenderId() const
{
	if(!m_pObjectInfo)
		return RENDERID::END;
	return (RENDERID::ID)m_pObjectInfo->eRenderId;
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

void CGameObject::UpdateTileCollision()
{
	
}

void CGameObject::UpdateDie()
{
}

void CGameObject::UpdateJump()
{
}

void CGameObject::UpdateGravity()
{
	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (!m_bJumping && !m_bFlying)
	{
		m_tInfo.vDir.y += m_fGravityAccelerlation * fDeltaTime;
	}
}

void CGameObject::RenderCollision()
{
#ifdef _DEBUG
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
			v2LinePos[0] = { (float)(tCollision.tFRect.left), (float)(tCollision.tFRect.top) };
			v2LinePos[1] = { (float)(tCollision.tFRect.right),(float)( tCollision.tFRect.top) };
			v2LinePos[2] = { (float)(tCollision.tFRect.right),(float)( tCollision.tFRect.bottom) };
			v2LinePos[3] = { (float)(tCollision.tFRect.left), (float)(tCollision.tFRect.bottom) };
			v2LinePos[4] = v2LinePos[0];
		}
		else
		{
			_vec2 v2Center = { (tCollision.tFRect.right + tCollision.tFRect.left) * 0.5f, (tCollision.tFRect.top + tCollision.tFRect.bottom) * 0.5f };
			float fRadius = (tCollision.tFRect.right - tCollision.tFRect.left) * 0.5f;
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
		Safe_Delete_Arr(v2LinePos);
	}
	for (auto& tCollision : m_vecAttackCollision)
	{

		D3DXVECTOR2* v2LinePos;
		DWORD dwSize = 0;
		if (tCollision.eId == COLLISION::C_RECT)
		{
			dwSize = 5;
			v2LinePos = new _vec2[dwSize];
			v2LinePos[0] = { (float)(tCollision.tFRect.left), (float)(tCollision.tFRect.top) };
			v2LinePos[1] = { (float)(tCollision.tFRect.right),(float)(tCollision.tFRect.top) };
			v2LinePos[2] = { (float)(tCollision.tFRect.right),(float)(tCollision.tFRect.bottom) };
			v2LinePos[3] = { (float)(tCollision.tFRect.left), (float)(tCollision.tFRect.bottom) };
			v2LinePos[4] = v2LinePos[0];
		}
		else
		{
			_vec2 v2Center = { (tCollision.tFRect.right + tCollision.tFRect.left) * 0.5f, (tCollision.tFRect.top + tCollision.tFRect.bottom) * 0.5f };
			float fRadius = (tCollision.tFRect.right - tCollision.tFRect.left) * 0.5f;
			dwSize = 9;
			v2LinePos = new _vec2[dwSize];
			for (int i = 0; i < int(dwSize - 1); ++i)
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
		Safe_Delete_Arr(v2LinePos);
	}
	for (auto& tCollision : m_vecTileCollision)
	{

		D3DXVECTOR2* v2LinePos;
		DWORD dwSize = 0;
		if (tCollision.eId == COLLISION::C_RECT)
		{
			dwSize = 5;
			v2LinePos = new _vec2[dwSize];
			v2LinePos[0] = { (float)(tCollision.tFRect.left), (float)(tCollision.tFRect.top) };
			v2LinePos[1] = { (float)(tCollision.tFRect.right),(float)(tCollision.tFRect.top) };
			v2LinePos[2] = { (float)(tCollision.tFRect.right),(float)(tCollision.tFRect.bottom) };
			v2LinePos[3] = { (float)(tCollision.tFRect.left), (float)(tCollision.tFRect.bottom) };
			v2LinePos[4] = v2LinePos[0];
		}
		else
		{
			_vec2 v2Center = { (tCollision.tFRect.right + tCollision.tFRect.left) * 0.5f, (tCollision.tFRect.top + tCollision.tFRect.bottom) * 0.5f };
			float fRadius = (tCollision.tFRect.right - tCollision.tFRect.left) * 0.5f;
			dwSize = 9;
			v2LinePos = new _vec2[dwSize];
			for (int i = 0; i < int(dwSize - 1); ++i)
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
		CGraphic_Device::Get_Instance()->Get_Line()->Draw(v2LinePos, dwSize, D3DCOLOR_ARGB(100, 100, 100, 100));
		Safe_Delete_Arr(v2LinePos);
	
		
	}
	CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);

	_vec3 vScroll = CScroll_Manager::Get_Scroll();

	DEBUG_STRING(L" HP : %f", m_fCurHp,m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y)
#endif //_DEBUG
}

void CGameObject::RenderDieEffect(_vec3 _vPos)
{
	m_fCoolDownDieEffectRemainTime -= CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_fCoolDownDieEffectRemainTime > 0)
		return;
	m_fCoolDownDieEffectRemainTime = m_fMaxCoolDownDieEffectTime;
	const ANIMATION* pAnim;

	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"Common_EffectsDie");

	INFO tInfo;
	ZeroMemory(&tInfo, sizeof(INFO));
	tInfo.vPos = _vPos;
	tInfo.vDir = { 0.0f,0.0f,0.f };
	tInfo.vSize = { 1.0f,1.0f,0.f };

	CGameObject* pEffect = CEffect::Create(pAnim, tInfo);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::EFFECT,pEffect);

	_vec3 vBlack = _vPos;
	vBlack.y += 10.f;

	ZeroMemory(&tInfo, sizeof(INFO));
	tInfo.vPos = vBlack;
	tInfo.vDir = { 0.0f,0.0f,0.f };
	tInfo.vSize = { 1.0f,1.0f,0.f };

	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"Common_EffectsDieBlack");
	pEffect = CEffect::Create(pAnim, tInfo);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::EFFECT, pEffect);
}

