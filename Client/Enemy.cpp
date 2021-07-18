#include "stdafx.h"
#include "Enemy.h"
#include "Effect.h"
#include "Knife.h"

CEnemy::CEnemy()
	:m_bActived(false)
	,m_bHit(false)
	,m_fHitMaxTime(0.2f)
	, m_fHitCumulatedTime(0.f)
	, m_fActionCoolDownRemainTime(0.f)
	, m_fMaxAttackCoolDownTime(3.f)
	, m_bTimeStop(false)
{
}


CEnemy::~CEnemy()
{
}
// m_tInfo ³Ö±â
HRESULT CEnemy::Ready_GameObject()
{
	m_tInfo = m_pActorInfo->tInfo;
	return S_OK;
}

int CEnemy::Update_GameObject()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	if (m_bHit)
	{
		m_fHitCumulatedTime += CTime_Manager::Get_Instance()->Get_DeltaTime();

		if (m_fHitCumulatedTime >= m_fHitMaxTime)
		{
			m_fHitCumulatedTime = 0;
			m_bHit = false;
		}
		else
		{
			return OBJ_NOEVENT;
		}		

	}

	return OBJ_NOEVENT;
}

void CEnemy::InitUpdate_GameObject()
{
	if (m_tInfo.vPos.x < CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info().vPos.x)
		m_bFliped = false;
	else
		m_bFliped = true;
}

void CEnemy::Late_Update_GameObject()
{
}

void CEnemy::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_pObjectInfo->wstrObjectImage_ObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_tInfo.fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);
	matWorld = matScale * matRotZ * matTrans;
	const RECT& rect = m_pObjectInfo->tRect;
	float 	fCenterX = float(((rect.right - rect.left)*0.5f));
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CEnemy::Release_GameObject()
{
}

void CEnemy::Set_Hit(bool _bHit)
{
	m_bHit = _bHit;
}

void CEnemy::Set_Die()
{
	m_bDead = true;
	m_bDieInit = true;
}

void CEnemy::OnOverlaped(CGameObject* _pHitObject, _vec3 vHitPos)
{
	if (!m_bHit || dynamic_cast<CKnife*>(_pHitObject))
	{
		CGameObject::OnOverlaped(_pHitObject, vHitPos);
		m_bHit = true;
		const ANIMATION* pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"SakuyaMelee");
		INFO tInfo;
		ZeroMemory(&tInfo, sizeof(INFO));
		tInfo.vPos = vHitPos;
		if(vHitPos.x < m_tInfo.vPos.x)
			tInfo.vDir = { 3.0f,0.0f,0.f };
		else
			tInfo.vDir = { -3.0f,0.0f,0.f };
		tInfo.vSize = { 5.0f,3.0f,0.f };
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::EFFECT), CEffect::Create(pAnim, tInfo));
	}

}

void CEnemy::Set_Active(bool _bActive)
{
	m_bActived = _bActive;
}

void CEnemy::Set_Fliped(bool _bFliped)
{
	m_bFliped = _bFliped;
}

void CEnemy::Set_TimeStop(bool _bStop)
{
	m_bTimeStop = _bStop;
}

void CEnemy::UpdateGravity()
{
	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (!m_bFlying && !m_bOnGround)
	{
		m_tInfo.vDir.y += m_fGravityAccelerlation * fDeltaTime;
	}
}

bool CEnemy::UpdateActive()
{
	if (m_bTimeStop)
	{
		if (m_bHit)
		{
			m_fHitCumulatedTime += CTime_Manager::Get_Instance()->Get_DeltaTime();
			ZeroMemory(&m_vecBodyCollision[0].tFRect, sizeof(FRECT));
			if (m_fHitCumulatedTime >= m_fHitMaxTime)
			{
				m_bHit = false;
				m_fHitCumulatedTime = 0;
			}

		}
		return false;
	}

	if (m_bActived)
		return m_bActived;
	_vec3 vDiff = CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info().vPos - m_tInfo.vPos;
	if (D3DXVec3Length(&vDiff) < 500.f)
		m_bActived = true;
	return m_bActived;
}

void CEnemy::UpdatePattern()
{
}

void CEnemy::UpdateActionCoolDown()
{
	if (m_fActionCoolDownRemainTime <= 0.f)
		return;
	m_fActionCoolDownRemainTime -= CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_fActionCoolDownRemainTime <= 0.f)
		m_fActionCoolDownRemainTime = 0.f;
}

void CEnemy::UpdateAnimation()
{
	m_fAnimationCumulatedTime += CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_fAnimationCumulatedTime >= m_vecAnimation[m_eCurState]->fPlay_Speed)
	{
		++m_uiAnimationFrame;
		if (m_uiAnimationFrame >= m_vecAnimation[m_eCurState]->vecRect.size())
			m_uiAnimationFrame = 0;
		m_fAnimationCumulatedTime = 0.f;
	}
	
}

void CEnemy::RenderDieEffect(_vec3 _vPos)
{
	if (m_bTimeStop)
		return;
	CGameObject::RenderDieEffect(_vPos);
}
