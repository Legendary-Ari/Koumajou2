#include "stdafx.h"
#include "Toad.h"
CToad::CToad()
{
}


CToad::~CToad()
{
}

CGameObject * CToad::Create(const ACTORINFO * _pActorInfo, const OBJECTINFO * _pObjectInfo)
{
	CGameObject* pInstance = new CToad;
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

HRESULT CToad::Ready_GameObject()
{
	if (FAILED(CEnemy::Ready_GameObject()))
		return E_FAIL;
	m_vecAnimation.resize(STATE_END);

	const ANIMATION* pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + m_pObjectInfo->wstrIdleAnimImage_StateKey);
	if (!pAnim)
	{
		ERR_MSG(L"Toad의 이미지를 찾지 못했습니다.");
		return E_FAIL;
	}
	m_vecAnimation[IDLE] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"Jump");
	if (!pAnim)
	{
		ERR_MSG(L"Toad의 이미지를 찾지 못했습니다.1");
		return E_FAIL;
	}
	m_vecAnimation[JUMP] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"Attack");
	if (!pAnim)
	{
		ERR_MSG(L"Toad의 이미지를 찾지 못했습니다.2");
		return E_FAIL;
	}
	m_vecAnimation[ATTACK] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"Die");
	if (!pAnim)
	{
		ERR_MSG(L"Toad의 이미지를 찾지 못했습니다.3");
		return E_FAIL;
	}
	m_vecAnimation[DIE] = pAnim;


	m_eCurState = IDLE;
	m_fJumpPower = -7.f;

	m_vecBodyCollision.resize(1);
	m_vecBodyCollision[0].eId = COLLISION::C_RECT;
	m_vecTileCollision = m_vecBodyCollision;
	m_vecAttackCollision.resize(1);
	m_vecAttackCollision[0].eId = COLLISION::C_RECT;
	return S_OK;
}

int CToad::Update_GameObject()
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
			m_eCurState = CToad::DIE;
			m_uiAnimationFrame = 0;
			ZeroMemory( &m_vecBodyCollision[0].tFRect, sizeof(FRECT));
			m_bDieInit = false;
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
		else
		{
			return OBJ_NOEVENT;
		}
	}

	UpdatePattern();
	UpdateGravity();
	UpdateJump();
	m_tInfo.vPos += m_tInfo.vDir;

	return OBJ_NOEVENT;
}

void CToad::Late_Update_GameObject()
{
	UpdateState();
	UpdateAnimation();
	UpdateTileCollision();
	UpdateBodyCollision();
	UpdateAttackCollision();
}

void CToad::Render_GameObject()
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

void CToad::UpdateBodyCollision()
{
	if (m_bDead)
		return;
	float fSize = m_tInfo.vSize.x;
	float fReduceSizeLeft = 0.25f;
	float fReduceSizeRight = 0.25f;
	float fReduceSizeUp = 0.f;
	float fReduceSizeDown = 1.f;

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

void CToad::UpdateTileCollision()
{
	float fSize = m_tInfo.vSize.x;
	float fReduceSizeLeft = 0.25f;
	float fReduceSizeRight = 0.25f;
	float fReduceSizeUp = 0.f;
	float fReduceSizeDown = 1.f;

	RECT rect = m_vecAnimation[0]->vecRect[0];

	_vec2 v2Radius = { (float)((rect.right - rect.left) * 0.5f), (float)((rect.bottom - rect.top) * 0.5f) };
	v2Radius *= fSize;
	m_vecTileCollision[0].tFRect =
	{
		(float)(m_tInfo.vPos.x - v2Radius.x * m_tInfo.vSize.x * fReduceSizeLeft),
		(float)(m_tInfo.vPos.y - v2Radius.y * m_tInfo.vSize.y * fReduceSizeUp),
		(float)(m_tInfo.vPos.x + v2Radius.x * m_tInfo.vSize.x * fReduceSizeRight),
		(float)(m_tInfo.vPos.y + v2Radius.y * m_tInfo.vSize.y * fReduceSizeDown)
	};
}

void CToad::UpdateAttackCollision()
{
	if (m_eCurState != ATTACK)
	{
		ZeroMemory(&m_vecAttackCollision[0].tFRect, sizeof(FRECT));
		return;
	}
		
	RECT rect = m_vecAnimation[0]->vecRect[0];

	_vec2 v2Radius = { (float)((rect.right - rect.left) * 0.5f), (float)((rect.bottom - rect.top) * 0.5f) };
	v2Radius.x *= m_tInfo.vSize.x;
	v2Radius.y *= m_tInfo.vSize.y;

	m_vecAttackCollision[0].tFRect =
	{
		(float)(m_tInfo.vPos.x + v2Radius.x * 0.2f) ,
		(float)(m_tInfo.vPos.y - v2Radius.y * -0.4f) ,
		(float)(m_tInfo.vPos.x) + v2Radius.x * 0.2f * m_uiAnimationFrame,
		(float)(m_tInfo.vPos.y - v2Radius.y * -0.5f)
	};
	if (m_bFliped)
	{
		float fTemp = m_vecAttackCollision[0].tFRect.left;
		m_vecAttackCollision[0].tFRect.left = m_tInfo.vPos.x - (m_vecAttackCollision[0].tFRect.right - m_tInfo.vPos.x);
		m_vecAttackCollision[0].tFRect.right = m_tInfo.vPos.x - (fTemp - m_tInfo.vPos.x);
	}
}

void CToad::UpdatePattern()
{
	UpdateAttackCoolDown();
	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	const CGameObject* pPlayer = CGameObject_Manager::Get_Instance()->Get_Player();
	_vec3 vDiff = pPlayer->Get_Info().vPos - m_tInfo.vPos;
	_vec3 vDiffNomalized;
	D3DXVec3Normalize(&vDiffNomalized, &vDiff);
	if ( D3DXVec3Length(&vDiff) > 100.f)
	{
		if (m_fAttackCoolDownRemainTime == 0.f)
		{
			if (m_ePrevState != JUMP && m_ePrevState != ATTACK)
			{
				m_eCurState = JUMP;
				m_fAttackCoolDownRemainTime = m_fMaxAttackCoolDownTime;
				if (m_tInfo.vPos.x > CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info().vPos.x)
					m_bFliped = true;
				else
					m_bFliped = false;
				m_tInfo.vDir.y = m_fJumpPower;
				m_bJumping = true;
				m_bOnGround = false;
			}
		}
	}
	else
	{
		if (m_fAttackCoolDownRemainTime == 0.f && !m_bJumping)
		{
			m_eCurState = ATTACK;
			if (m_tInfo.vPos.x > CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info().vPos.x && m_eCurState)
				m_bFliped = true;
			else
				m_bFliped = false;
		}
	}
	//--------------------------------------


	
}

void CToad::UpdateState()
{
	if (m_bOnGround && m_eCurState != ATTACK && !m_bDead)
		m_eCurState = IDLE;



	if (m_ePrevState != m_eCurState)
	{
		m_uiAnimationFrame = 0;
		switch (m_eCurState)
		{
		case IDLE:
			m_tInfo.vDir.x = 0;
			m_tInfo.vDir.y = 0;
			break;
		case JUMP:
			break;
		case ATTACK:
			m_tInfo.vDir.x = 0;
			break;
		case DIE:
			break;
		default:
			break;
		}
		m_ePrevState = m_eCurState;
	}
}

void CToad::UpdateAnimation()
{
	m_fAnimationCumulatedTime += CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_fAnimationCumulatedTime > m_vecAnimation[m_eCurState]->fPlay_Speed)
	{
		++m_uiAnimationFrame;
		if (m_eCurState == JUMP && m_bJumping)
		{
			if (m_uiAnimationFrame > 1)
				m_uiAnimationFrame = 1;
		}
		if (m_uiAnimationFrame >= m_vecAnimation[m_eCurState]->vecRect.size())
		{
			if (!m_vecAnimation[m_eCurState]->bLoop)
			{
				UINT uiTempFrame = m_uiAnimationFrame;
				m_uiAnimationFrame = 0;
				switch (m_eCurState)
				{
				case CToad::IDLE:
					m_tInfo.vDir.x = 0;
					m_tInfo.vDir.y = 0;
					m_uiAnimationFrame = 0;
					break;
				case CToad::ATTACK:
					m_eCurState = IDLE;
					m_fAttackCoolDownRemainTime = m_fMaxAttackCoolDownTime;
					m_uiAnimationFrame = 0;
					break;
				case CToad::JUMP:
					m_uiAnimationFrame = m_vecAnimation[m_eCurState]->vecRect.size() - 1;
					break;
				case CToad::DIE:
					m_bDestroyed = true;
					m_uiAnimationFrame = m_vecAnimation[m_eCurState]->vecRect.size() - 1;
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

void CToad::UpdateJump()
{
	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_bJumping || m_bFalling)
	{
		if (m_bFliped)
		{
			m_tInfo.vDir.x = -m_pObjectInfo->fMoveSpeed * fDeltaTime;
		}
		else
		{
			m_tInfo.vDir.x = m_pObjectInfo->fMoveSpeed * fDeltaTime;
		}
	}
	if (m_tInfo.vDir.y > 0)
	{
		m_bJumping = false;
		m_bFalling = true;
	}
}

void CToad::UpdateDie()
{
	
}
