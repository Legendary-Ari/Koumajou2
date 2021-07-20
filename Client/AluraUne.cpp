#include "stdafx.h"
#include "AluraUne.h"
#include "AluraUneGEffrect.h"
#include "AluraUneProjectile.h"

CAluraUne::CAluraUne()
	:m_fMaxDieTime(1.f)
	, m_iRemainAtk(0)
	, m_iMaxAtkMount(3)
{
}


CAluraUne::~CAluraUne()
{
}

CGameObject * CAluraUne::Create(const ACTORINFO * _pActorInfo, const OBJECTINFO * _pObjectInfo)
{
	CGameObject* pInstance = new CAluraUne;
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

HRESULT CAluraUne::Ready_GameObject()
{
	if (FAILED(CEnemy::Ready_GameObject()))
		return E_FAIL;
	m_vecAnimation.resize(STATE_END);

	const ANIMATION* pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + m_pObjectInfo->wstrIdleAnimImage_StateKey);
	if (!pAnim)
	{
		ERR_MSG(L"CAluraUne의 이미지를 찾지 못했습니다.");
		return E_FAIL;
	}
	m_vecAnimation[IDLE] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"Attack");
	if (!pAnim)
	{
		ERR_MSG(L"CAluraUne의 이미지를 찾지 못했습니다.1");
		return E_FAIL;
	}
	m_vecAnimation[ATTACK] = pAnim;

	m_eCurState = IDLE;
	m_bFlying = true;
	m_fMaxCoolDownDieEffectTime = 0.1f;
	m_fAttackWakeDistance = 200.f;
	m_uiChi = 30;

	m_vecBodyCollision.resize(1);
	m_vecBodyCollision[0].eId = COLLISION::C_RECT;
	return S_OK;
}

int CAluraUne::Update_GameObject()
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
			m_uiAnimationFrame = 0;
			ZeroMemory(&m_vecBodyCollision[0].tFRect, sizeof(FRECT));
			m_bDieInit = false;
			m_fRamainedDieTime = m_fMaxDieTime;
		}
		UpdateDie();
		return OBJ_NOEVENT;
	}
	if (m_bHit)
	{
		m_fHitCumulatedTime += fDeltaTime;
		ZeroMemory(&m_vecBodyCollision[0].tFRect, sizeof(FRECT));
		if (m_fHitCumulatedTime >= m_fHitMaxTime)
		{
			m_bHit = false;
			m_fHitCumulatedTime = 0;
		}

	}

	UpdatePattern();

	return OBJ_NOEVENT;
}

void CAluraUne::Late_Update_GameObject()
{
	UpdateState();
	UpdateAnimation();
	UpdateBodyCollision();
}

void CAluraUne::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_vecAnimation[0]->wstrObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, (m_bFliped ? -1.0f : 1.0f), 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_tInfo.fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);
	matWorld = matScale * matRotZ * matTrans;
	const RECT& rect = m_vecAnimation[IDLE]->vecRect[0];
	float 	fCenterX = float(((rect.right - rect.left) * 0.5f));
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
	const RECT& tRenderRect = m_vecAnimation[m_eCurState]->vecRect[m_uiAnimationFrame];
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	D3DCOLOR tColor;
	if (m_bHit || m_bDead)
		tColor = D3DCOLOR_ARGB(255, 150, 150, 150);
	else
		tColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &tRenderRect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, tColor);
	RenderCollision();

	if (m_bDead)
	{
		_vec3 vRandom(m_tInfo.vPos);
		vRandom.x += rand() % 50 - 25.f;
		vRandom.y += rand() % 50 - 25.f;
		RenderDieEffect(vRandom);
	}
}

void CAluraUne::UpdateBodyCollision()
{
	if (m_bDead)
	{
		ZeroMemory(&m_vecBodyCollision[0], sizeof(m_vecBodyCollision[0]));
		return;
	}
	float fSize = m_tInfo.vSize.x;
	float fReduceSizeLeft = 0.50f;
	float fReduceSizeRight = 0.50f;
	float fReduceSizeUp = 1.0f;
	float fReduceSizeDown = 1.0f;

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

void CAluraUne::UpdatePattern()
{
	UpdateActionCoolDown();
	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	const CGameObject* pPlayer = CGameObject_Manager::Get_Instance()->Get_Player();
	_vec3 vDiff = pPlayer->Get_Info().vPos - m_tInfo.vPos;
	_vec3 vDiffNomalized;
	D3DXVec3Normalize(&vDiffNomalized, &vDiff);
	if (D3DXVec3Length(&vDiff) > m_fAttackWakeDistance)
	{
		if (m_fActionCoolDownRemainTime > 0.5f)
		{
			if (m_tInfo.vPos.x > CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info().vPos.x)
				m_bFliped = true;
			else
				m_bFliped = false;
		}
	}
	else
	{
		if (m_fActionCoolDownRemainTime <= 0.f)
		{
			m_eCurState = ATTACK;
			m_iRemainAtk = 3;
		}
	}

	m_fActionCoolDownRemainTime -= fDeltaTime;
	if (m_iRemainAtk > 0 && m_fActionCoolDownRemainTime <= 0.f)
	{
		m_fActionCoolDownRemainTime = m_pObjectInfo->fAtkRatio;
		_vec3 vPos = m_tInfo.vPos;
		_vec3 vAdd = (m_tInfo.vSize.x * _vec3{40.f,0.f,0.f });
		--m_iRemainAtk;
		for (int i = 0; i < m_iMaxAtkMount - m_iRemainAtk; ++i)
		{
			vPos += vAdd;
		}
		const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Alura_Une_Projectile");
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, CAluraUneProjectile::Create(pObjectInfo, vPos));
		vPos.y += 32.f;
		const ANIMATION* pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"Alura_UneE_GroundUp");
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::EFFECT,CAluraUneGEffrect::Create(pAnim, vPos));
		vPos = m_tInfo.vPos;
		vAdd = (m_tInfo.vSize.x * _vec3{ -40.f,0.f,0.f });
		for (int i = 0; i < m_iMaxAtkMount - m_iRemainAtk; ++i)
		{
			vPos += vAdd;
		}
		const OBJECTINFO* pObjectInfo2 = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Alura_Une_Projectile");
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, CAluraUneProjectile::Create(pObjectInfo2, vPos));
		vPos.y += 32.f;
		const ANIMATION* pAnim2 = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"Alura_UneE_GroundUp");
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::EFFECT, CAluraUneGEffrect::Create(pAnim2, vPos));
	}
}

void CAluraUne::UpdateState()
{
	if (m_bTimeStop)
		return;
	if (m_ePrevState != m_eCurState)
	{
		m_uiAnimationFrame = 0;
		switch (m_eCurState)
		{
		case IDLE:
			break;
		case ATTACK:

			break;
		default:
			break;
		}
		m_ePrevState = m_eCurState;
	}

	if (m_eCurState == IDLE)
	{
		if (m_tInfo.vPos.x > CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info().vPos.x)
			m_bFliped = true;
		else
			m_bFliped = false;
	}
}

void CAluraUne::UpdateAnimation()
{
	if (m_bTimeStop)
		return;
	m_fAnimationCumulatedTime += CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_fAnimationCumulatedTime > m_vecAnimation[m_eCurState]->fPlay_Speed)
	{
		++m_uiAnimationFrame;
		if (m_uiAnimationFrame >= m_vecAnimation[m_eCurState]->vecRect.size())
		{
			if (!m_vecAnimation[m_eCurState]->bLoop)
			{
				UINT uiTempFrame = m_uiAnimationFrame;
				m_uiAnimationFrame = 0;
				switch (m_eCurState)
				{
				case IDLE:
					m_uiAnimationFrame = 0;
					break;
				case ATTACK:
					m_eCurState = IDLE;
					m_fActionCoolDownRemainTime = m_pObjectInfo->fAtkRatio;
					m_uiAnimationFrame = 0;
					break;
					break;
				default:
					break;
				}
			}
			else // if loop
			{
				m_uiAnimationFrame = 0;
			}

		}

		m_fAnimationCumulatedTime -= m_vecAnimation[m_eCurState]->fPlay_Speed;
	}
}

void CAluraUne::UpdateDie()
{
	m_fRamainedDieTime -= CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_fRamainedDieTime < 0)
	{
		m_bDestroyed = true;
	}
}

