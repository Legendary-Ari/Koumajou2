#include "stdafx.h"
#include "BossRemilia.h"


CBossRemilia::CBossRemilia()
	:m_fCoolTimePattern3Bullet(0.8f)
	, m_fRemainPatternTime(0)
{
}


CBossRemilia::~CBossRemilia()
{
}

CGameObject * CBossRemilia::Create(const OBJECTINFO * _pObjectInfo)
{
	CGameObject* pInstance = new CBossRemilia;
	pInstance->Set_Prefab(_pObjectInfo);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CBossRemilia::Ready_GameObject()
{
	const ANIMATION* pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + m_pObjectInfo->wstrIdleAnimImage_StateKey);
	if (!pAnim)
	{
		ERR_MSG(L"CBossRemilia 이미지를 찾지 못했습니다.1");
		return E_FAIL;
	}
	m_vecAnimation.resize(STATE::STATE_END);
	m_vecAnimation[IDLE] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"B_Attack");
	if (!pAnim)
	{
		ERR_MSG(L"CBossRemilia 이미지를 찾지 못했습니다.2");
		return E_FAIL;
	}
	m_vecAnimation[ATTACK] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"Move");
	if (!pAnim)
	{
		ERR_MSG(L"CBossRemilia 이미지를 찾지 못했습니다.3");
		return E_FAIL;
	}
	m_vecAnimation[MOVE] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"Spell");
	if (!pAnim)
	{
		ERR_MSG(L"CBossRemilia 이미지를 찾지 못했습니다.4");
		return E_FAIL;
	}
	m_vecAnimation[SPELL] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"Gungnil");
	if (!pAnim)
	{
		ERR_MSG(L"CBossRemilia 이미지를 찾지 못했습니다.5");
		return E_FAIL;
	}
	m_vecAnimation[GUNGNIL] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"Die");
	if (!pAnim)
	{
		ERR_MSG(L"CBossRemilia 이미지를 찾지 못했습니다.6");
		return E_FAIL;
	}

	m_vecPatternTime.resize(PATTERN::P_PATTERN_END);
	m_fRemainPatternTime = m_vecPatternTime[P_IDLE] = 0.1f;
	m_vecPatternTime[P_PATTERN1] = 0.2f;
	m_vecPatternTime[P_MOVE1] = 0.1f;
	m_vecPatternTime[P_PATTERN2] = 0.1f;
	m_vecPatternTime[P_MOVE2] = 0.5f;
	m_vecPatternTime[P_PATTERN3] = 3.0f;
	m_vecPatternTime[P_MOVE3] = 3.0f;
	m_vecPatternTime[P_MOVE4] = 3.0f;
	m_vecPatternTime[P_PATTERN4] = 3.0f;
	m_vecPatternTime[P_SPELL1] = 3.0f;
	m_vecPatternTime[P_SPELL2] = 3.0f;

	ZeroMemory(&m_tInfo, sizeof(INFO));
	m_tInfo.vPos = { float(CLIENTCX>>1), float(CLIENTCY >> 1),0.f };
	m_tInfo.vSize = { 1.0f,1.0f,0.f };
	m_eCurState = IDLE;
	m_eCurPattern = PATTERN::P_IDLE;
	return S_OK;
}

int CBossRemilia::Update_GameObject()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;
	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_bDead)
	{
		if (m_bDieInit)
		{
			m_bDieInit = false;
			m_uiAnimationFrame = 0;
			ZeroMemory(&m_vecBodyCollision[0].tFRect, sizeof(FRECT));

			m_fRamainedDieTime = m_fMaxDieTime;
			m_vDieDir = { m_bFliped ? cosf(D3DXToRadian(-30.f)) : cosf(D3DXToRadian(30.f)), sinf(D3DXToRadian(-30)),0.f };
		}
		UpdateDie();
		return OBJ_NOEVENT;
	}
	if (m_bHit)
	{
		m_fHitRemainTime -= fDeltaTime;
		ZeroMemory(&m_vecBodyCollision[0].tFRect, sizeof(FRECT));
		if (m_fHitRemainTime < 0)
		{
			m_bHit = false;
			m_fHitRemainTime = 0;
		}

	}
	m_vBulletCenter = m_tInfo.vPos;
	m_vBulletCenter.y += 16.f;
	UpdatePattern();
	m_tInfo.vPos += m_tInfo.vDir * 50.f * fDeltaTime;
	return OBJ_NOEVENT;
}

void CBossRemilia::InitUpdate_GameObject()
{
}

void CBossRemilia::Late_Update_GameObject()
{
}

void CBossRemilia::Render_GameObject()
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
	const RECT& rect = m_vecAnimation[m_eCurState]->vecRect[0];
	float 	fCenterX = float(((rect.right - rect.left)*0.5f));
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
	const RECT& tRenderRect = m_vecAnimation[m_eCurState]->vecRect[m_uiAnimationFrame];
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	D3DCOLOR tColor;
	if (m_bHit)
		tColor = D3DCOLOR_ARGB(255, 150, 150, 150);
	else
		tColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &tRenderRect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, tColor);
	RenderCollision();
}

void CBossRemilia::Release_GameObject()
{
}

void CBossRemilia::OnOverlaped(CGameObject * _pHitObject, _vec3 vHitPos)
{
}

void CBossRemilia::UpdatePattern()
{
	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_fRemainPatternTime -= fDeltaTime;
	if (m_fRemainPatternTime <= 0)
	{
		m_eCurPattern = PATTERN(m_eCurPattern + 1);
		if (m_eCurPattern == PATTERN::P_PATTERN_END)
		{
			m_eCurPattern = PATTERN::P_IDLE;
		}
	}
	if (m_ePrevPattern != m_eCurPattern) // Init
	{
		m_fRemainPatternTime += m_vecPatternTime[m_eCurPattern];
		switch (m_eCurPattern)
		{
		case CBossRemilia::P_IDLE:
			m_eCurState = STATE::IDLE;
			break;
		case CBossRemilia::P_PATTERN1:
			m_eCurState = STATE::ATTACK;
			break;
		case CBossRemilia::P_MOVE1:
			m_eCurState = STATE::MOVE;
			break;
		case CBossRemilia::P_PATTERN2:
			m_eCurState = STATE::ATTACK;
			break;
		case CBossRemilia::P_MOVE2:
			m_eCurState = STATE::MOVE;
			break;
		case CBossRemilia::P_PATTERN3:
			m_eCurState = STATE::ATTACK;
			break;
		case CBossRemilia::P_MOVE3:
			m_eCurState = STATE::MOVE;
			break;
		case CBossRemilia::P_MOVE4:
			break;
		case CBossRemilia::P_PATTERN4:
			m_eCurState = STATE::ATTACK;
			break;
		case CBossRemilia::P_SPELL1:
			m_eCurState = STATE::SPELL;
			break;
		case CBossRemilia::P_SPELL2:
			m_eCurState = STATE::SPELL;
			break;
		default:
			break;
		}
		m_ePrevPattern = m_eCurPattern;
	}
	{ // Always
		switch (m_eCurPattern)
		{
		case CBossRemilia::P_IDLE:
			break;
		case CBossRemilia::P_PATTERN1:
			Pattern1();
			break;
		case CBossRemilia::P_MOVE1:
		{
			_vec3 vDiff = (m_vMoveTargetPos - m_tInfo.vPos);
			D3DXVec3Normalize(&m_tInfo.vDir, &vDiff);
			break;
		}
		case CBossRemilia::P_PATTERN2:
			Pattern2();
			break;
		case CBossRemilia::P_MOVE2:
		{
			_vec3 vDiff = (m_vMoveTargetPos - m_tInfo.vPos);
			D3DXVec3Normalize(&m_tInfo.vDir, &vDiff);
			break;
		}
		case CBossRemilia::P_PATTERN3:
			Pattern3();
			break;
		case CBossRemilia::P_MOVE3:
		{
			_vec3 vDiff = (m_vMoveTargetPos - m_tInfo.vPos);
			D3DXVec3Normalize(&m_tInfo.vDir, &vDiff);
			break;
		}
		case CBossRemilia::P_MOVE4:
		{
			_vec3 vDiff = (m_vMoveTargetPos - m_tInfo.vPos);
			D3DXVec3Normalize(&m_tInfo.vDir, &vDiff);
			break;
		}
		case CBossRemilia::P_PATTERN4:
			Pattern4();
			break;
		case CBossRemilia::P_SPELL1:
			Spell1();
			break;
		case CBossRemilia::P_SPELL2:
			Spell2();
			break;
		default:
			break;
		}
	}
}

void CBossRemilia::UpdateDie()
{
}

void CBossRemilia::UpdateState() // UpdateAnimation Playing
{
	switch (m_eCurState)
	{
	case STATE::IDLE:
		break;
	case STATE::ATTACK:
		break;
	case STATE::MOVE:
		break;
	case STATE::GUNGNIL:
		break;
	case STATE::SPELL:
		break;
	case STATE::DIE:
		break;
	default:
		break;
	}
}

void CBossRemilia::UpdateAnimation() // UpdateAnimation End
{
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
				case STATE::IDLE:
					break;
				case STATE::ATTACK:
					break;
				case STATE::MOVE:
					break;
				case STATE::GUNGNIL:
					break;
				case STATE::SPELL:
					break;
				case STATE::DIE:
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

void CBossRemilia::UpdateBodyCollision()
{
}

void CBossRemilia::Pattern1()
{
}

void CBossRemilia::Pattern2()
{
}

void CBossRemilia::Pattern3()
{
}

void CBossRemilia::Pattern4()
{
}

void CBossRemilia::Spell1()
{
}

void CBossRemilia::Spell2()
{
}
