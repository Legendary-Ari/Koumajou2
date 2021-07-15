#include "stdafx.h"
#include "Owl.h"


COwl::COwl()
	:m_fMaxDieTime(0.5f)
	, m_fCumulatedDieTime(0.f)
	, m_fDieRotateSpeed(100.f)
{
}


COwl::~COwl()
{
}

CGameObject * COwl::Create(const ACTORINFO * _pActorInfo, const OBJECTINFO * _pObjectInfo)
{
	CGameObject* pInstance = new COwl;
	pInstance->Set_Prefab(_pObjectInfo);
	pInstance->Set_ActorInfo(_pActorInfo);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT COwl::Ready_GameObject()
{
	if (FAILED(CEnemy::Ready_GameObject()))
		return E_FAIL;
	m_vecAnimation.resize(STATE_END);

	const ANIMATION* pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + m_pObjectInfo->wstrIdleAnimImage_StateKey);
	if (!pAnim)
	{
		ERR_MSG(L"COwl의 이미지를 찾지 못했습니다.");
		return E_FAIL;
	}
	m_vecAnimation[IDLE] = pAnim;
	m_eCurState = IDLE;
	m_vTargetDir = m_tInfo.vDir;
	m_vTargetPos = m_tInfo.vPos;
	m_bFlying = true;
	m_bFalling = false;
	m_bJumping = false;
	m_bOnGround = false;

	m_vecBodyCollision.resize(1);
	m_vecBodyCollision[0].eId = COLLISION::C_RECT;
	return S_OK;
}

int COwl::Update_GameObject()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;
	if (!UpdateActive())
		return OBJ_NOEVENT;
	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_bDead)
	{
		if (m_bDieInit)
		{
			float fAngle = D3DXToRadian(rand());
			m_fDieRandomSign = cosf(fAngle);
			m_tInfo.vDir.y = -5.f;
			m_bDieInit = false;
			m_vecBodyCollision[0].tFRect = FRECT{};
		}
		UpdateDie();
		m_tInfo.vPos += m_tInfo.vDir;
		return OBJ_NOEVENT;
	}
	if (m_bHit)
	{
		m_fHitCumulatedTime += fDeltaTime;
		m_vecBodyCollision[0].tFRect = FRECT{};
		if (m_fHitCumulatedTime >= m_fHitMaxTime)
		{
			m_bHit = false;
			m_fHitCumulatedTime = 0;
		}
		else
		{
			return OBJ_NOEVENT;
		}
	}

	UpdatePattern();
	m_tInfo.vPos += m_tInfo.vDir * m_pObjectInfo->fMoveSpeed * fDeltaTime;
	UpdateAnimation();
	UpdateBodyCollision();
	UpdateAttackCollision();

	return OBJ_NOEVENT;
}

void COwl::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_vecAnimation[0]->wstrObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();
	if (m_tInfo.vDir.x < 0)
		m_bFliped = true;
	else
		m_bFliped = false;
	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, (m_bFliped ? -1.0f : 1.0f), 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_tInfo.fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);
	matWorld = matScale * matRotZ * matTrans;
	const RECT& rect = m_vecAnimation[IDLE]->vecRect[0];
	float 	fCenterX = float(((rect.right - rect.left) * 0.5f));
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
	const RECT& tRenderRect = m_vecAnimation[IDLE]->vecRect[m_uiAnimationFrame];
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	D3DCOLOR tColor;
	if (m_bHit)
		tColor = D3DCOLOR_ARGB(255, 100, 100, 100);
	else
		tColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &tRenderRect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, tColor);
	RenderCollision();
	if (m_bDead)
	{
		RenderDieEffect(m_tInfo.vPos);
	}
}

void COwl::UpdatePattern()
{
	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	const CGameObject* pPlayer = CGameObject_Manager::Get_Instance()->Get_Player();
	_vec3 vDiff = pPlayer->Get_Info().vPos - m_tInfo.vPos;
	_vec3 vDiffNomalized;
	D3DXVec3Normalize(&vDiffNomalized, &vDiff);
	_vec3 vDiffTarget = m_vTargetPos - m_tInfo.vPos;
	if (D3DXVec3Length( &vDiffTarget) < 30.f || D3DXVec3Length(&vDiff) > 200.f)
	{
		_vec3 vRandom = { float(rand() % 100) - 50.f , float(rand() % 100) - 50.f , 0.f};
		m_vTargetPos = pPlayer->Get_Info().vPos + vRandom;
	}
	
	_vec3 vDiffTargetNormalized;
	D3DXVec3Normalize(&vDiffNomalized, &vDiffTarget);
	m_tInfo.vDir = vDiffNomalized;

}

void COwl::UpdateBodyCollision()
{
	float fSize = m_tInfo.vSize.x;
	float fReduceSizeLeft = 1.0f;
	float fReduceSizeRight = 1.0f;
	float fReduceSizeUp = 0.5f;
	float fReduceSizeDown = 0.75f;

	RECT rect = m_vecAnimation[0]->vecRect[0];

	_vec2 v2Radius = { (float)((rect.right - rect.left) * 0.5f), (float)((rect.bottom - rect.top) * 0.5f) };
	v2Radius *= fSize;
	m_vecBodyCollision[0].tFRect =
	{
		(float)(m_tInfo.vPos.x - v2Radius.x * m_tInfo.vSize.x * fReduceSizeLeft),
		(float)(m_tInfo.vPos.y - v2Radius.y * m_tInfo.vSize.y * fReduceSizeUp),
		(float)(m_tInfo.vPos.x + v2Radius.x * m_tInfo.vSize.x * fReduceSizeRight),
		(float)(m_tInfo.vPos.y + v2Radius.y * m_tInfo.vSize.y * fReduceSizeDown)
	};
}

void COwl::UpdateDie()
{
	m_tInfo.vDir.y += 27.8f * CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_tInfo.fAngle += m_fDieRandomSign * m_fDieRotateSpeed * CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_fCumulatedDieTime += CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_fCumulatedDieTime > m_fMaxDieTime)
	{
		m_bDestroyed = true;
	}

}
