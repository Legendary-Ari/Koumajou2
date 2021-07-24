#include "stdafx.h"
#include "BossRemilia.h"
#include "Remilia_Arm.h"
#include "Remilia_Eye.h"
#include "RemiliaBullet.h"
#include "Remilia_Lazor.h"
#include "BossSkillEffect.h"
#include "Knife.h"
#include "Effect.h"

CBossRemilia::CBossRemilia()
	:m_fCoolTimePattern3Bullet(0.8f)
	, m_fRemainPatternTime(0)
	, m_uiPattern3PosMount(12)
	, m_uiPattern3Idx (0)
	, m_uiMaxSpellIdx(8)
	, m_fHitMaxTime(0.1f)
{
	ZeroMemory(m_bDonePattern, sizeof(m_bDonePattern));
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
	m_vecPatternTime[P_IDLE] = 1.1f;
	m_vecPatternTime[P_PATTERN1] = 1.0f;
	m_vecPatternTime[P_MOVE1] = 0.5f;
	m_vecPatternTime[P_PATTERN2] = 1.0f;
	m_vecPatternTime[P_MOVE2] = 0.5f;
	m_vecPatternTime[P_PATTERN3] = 3.0f;
	m_vecPatternTime[P_MOVE3] = 1.0f;
	m_vecPatternTime[P_PATTERN4] = 6.0f;
	m_vecPatternTime[P_MOVE4_1] = 6.0f;
	m_vecPatternTime[P_PATTERN4_1] = 6.0f;
	m_vecPatternTime[P_SPELL1] = 3.0f;
	m_vecPatternTime[P_SPELL2] = 4.0f;
	m_vecPatternTime[P_SPELL3] = 4.0f;
	m_fRemainPatternTime = 2.0f;

	m_vPattern3Pos[0] = 130.f * _vec3{ cosf(D3DXToRadian(-90)), sinf(D3DXToRadian(-90)), 0.f };
	m_vPattern3Pos[1] = 130.f * _vec3{ cosf(D3DXToRadian(-90 - 120)), sinf(D3DXToRadian(-90 - 120)), 0.f };
	m_vPattern3Pos[2] = 130.f * _vec3{ cosf(D3DXToRadian(-90 + 120)), sinf(D3DXToRadian(-90 + 120)), 0.f };
	m_vPattern3Pos[3] = 130.f * _vec3{ cosf(D3DXToRadian(90)), sinf(D3DXToRadian(90)), 0.f };
	m_vPattern3Pos[4] = 130.f * _vec3{ cosf(D3DXToRadian(90 - 120)), sinf(D3DXToRadian(90 - 120)), 0.f };
	m_vPattern3Pos[5] = 130.f * _vec3{ cosf(D3DXToRadian(90 + 120)), sinf(D3DXToRadian(90 + 120)), 0.f };
	m_vPattern3Pos[6] = 300.f * _vec3{ cosf(D3DXToRadian(0)), sinf(D3DXToRadian(0)), 0.f };
	m_vPattern3Pos[7] = 300.f * _vec3{ cosf(D3DXToRadian(-60)), sinf(D3DXToRadian(-60)), 0.f };
	m_vPattern3Pos[8] = 300.f * _vec3{ cosf(D3DXToRadian(-120)), sinf(D3DXToRadian(-120)), 0.f };
	m_vPattern3Pos[9] = 300.f * _vec3{ cosf(D3DXToRadian(180)), sinf(D3DXToRadian(-180)), 0.f };
	m_vPattern3Pos[10] = 300.f * _vec3{ cosf(D3DXToRadian(-240)), sinf(D3DXToRadian(-240)), 0.f };
	m_vPattern3Pos[11] = 300.f * _vec3{ cosf(D3DXToRadian(-300)), sinf(D3DXToRadian(-300)), 0.f };

	m_vecPattern3LazorIdx.emplace_back(make_pair(0,1));
	m_vecPattern3LazorIdx.emplace_back(make_pair(1, 2));
	m_vecPattern3LazorIdx.emplace_back(make_pair(2, 0));
	m_vecPattern3LazorIdx.emplace_back(make_pair(3, 4));
	m_vecPattern3LazorIdx.emplace_back(make_pair(4, 5));
	m_vecPattern3LazorIdx.emplace_back(make_pair(5, 3));
	m_vecPattern3LazorIdx.emplace_back(make_pair(2, 6));
	m_vecPattern3LazorIdx.emplace_back(make_pair(4, 6));
	m_vecPattern3LazorIdx.emplace_back(make_pair(1, 9));
	m_vecPattern3LazorIdx.emplace_back(make_pair(5, 9));
	m_vecPattern3LazorIdx.emplace_back(make_pair(0, 7));
	m_vecPattern3LazorIdx.emplace_back(make_pair(4, 7));
	m_vecPattern3LazorIdx.emplace_back(make_pair(0, 8));
	m_vecPattern3LazorIdx.emplace_back(make_pair(5, 8));
	m_vecPattern3LazorIdx.emplace_back(make_pair(1, 10));
	m_vecPattern3LazorIdx.emplace_back(make_pair(2, 11));
	m_vecPattern3LazorIdx.emplace_back(make_pair(3, 10));
	m_vecPattern3LazorIdx.emplace_back(make_pair(3, 11));
	m_vecPattern3LazorIdx.emplace_back(make_pair(6, 7));
	m_vecPattern3LazorIdx.emplace_back(make_pair(7, 8));
	m_vecPattern3LazorIdx.emplace_back(make_pair(8, 9));
	m_vecPattern3LazorIdx.emplace_back(make_pair(9, 10));
	m_vecPattern3LazorIdx.emplace_back(make_pair(10, 11));
	m_vecPattern3LazorIdx.emplace_back(make_pair(11, 6));

	m_vecBodyCollision.resize(1);
	ZeroMemory(&m_tInfo, sizeof(INFO));
	m_vMoveTargetPos = m_tInfo.vPos = { float(CLIENTCX>>1), float(CLIENTCY >> 1),0.f };
	m_tInfo.vSize = { 1.0f,1.0f,0.f };
	m_eCurState = IDLE;
	m_eCurPattern = PATTERN::P_IDLE;

	m_pArm[LEFT] = static_cast<CRemilia_Arm*>(CRemilia_Arm::Create(true, this));
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::ENEMY, m_pArm[LEFT]);
	m_pArm[RIGHT] = static_cast<CRemilia_Arm*>(CRemilia_Arm::Create(false, this));
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::ENEMY, m_pArm[RIGHT]);

	for (int i = 0; i < 3; ++i)
	{
		m_pEye[i] = static_cast<CRemilia_Eye*>(CRemilia_Eye::Create(i, this));
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::ENEMY, m_pEye[i]);
	}

	m_fCurHp = m_pObjectInfo->fMaxHp;

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
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSS_VOICE);
			CSoundMgr::Get_Instance()->PlaySound(L"lem_A037.wav",CSoundMgr::BOSS_VOICE);
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
	m_tInfo.vPos = CMyMath::VecInterpTo(m_tInfo.vPos,m_vMoveTargetPos,fDeltaTime,1.f);

	return OBJ_NOEVENT;
}

void CBossRemilia::InitUpdate_GameObject()
{
}

void CBossRemilia::Late_Update_GameObject()
{
	if (m_bDead)
		return;
	UpdateBodyCollision();
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


	{
		const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Remilia_Mohojin");
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(pObjectInfo->wstrObjectImage_ObjectKey);
		if (nullptr == pTexInfo)
			return;
		D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

		D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
		D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_tInfo.fAngle));
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - 10.f + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);
		matWorld = matScale * matRotZ * matTrans;
		const RECT& rect = pObjectInfo->tRect;
		float 	fCenterX = float(((rect.right - rect.left)*0.5f));
		float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		D3DCOLOR tColor;
		tColor = D3DCOLOR_ARGB(150, 255, 255, 255);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, tColor);

	}
	{
		const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Remilia_Body");
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(pObjectInfo->wstrObjectImage_ObjectKey);
		if (nullptr == pTexInfo)
			return;
		D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

		D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, (m_bFliped ? -1.0f : 1.0f), 1.f, 0.f);
		D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_tInfo.fAngle));
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - 10.f + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);
		matWorld = matScale * matRotZ * matTrans;
		const RECT& rect = pObjectInfo->tRect;
		float 	fCenterX = float(((rect.right - rect.left)*0.5f));
		float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		D3DCOLOR tColor;
		tColor = D3DCOLOR_ARGB(255, 255, 255, 255);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, tColor);

	}



	RenderCollision();
}

void CBossRemilia::Release_GameObject()
{
}

void CBossRemilia::OnOverlaped(CGameObject * _pHitObject, _vec3 vHitPos)
{
	if (!m_bHit || dynamic_cast<CKnife*>(_pHitObject))
	{
		CGameObject::OnOverlaped(_pHitObject, vHitPos);
		m_bHit = true;
		m_fHitRemainTime = m_fHitMaxTime;
		const ANIMATION* pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"Common_EffectsLight");
		INFO tInfo;
		ZeroMemory(&tInfo, sizeof(INFO));
		tInfo.vPos = vHitPos;
		tInfo.vDir = { 0.0f,0.0f,0.f };
		tInfo.vSize = { 0.5f,0.5f,0.f };
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::EFFECT), CEffect::Create(pAnim, tInfo));
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ENEMY_HIT);
		CSoundMgr::Get_Instance()->PlaySound(L"101.wav", CSoundMgr::ENEMY_HIT);
	}
}

void CBossRemilia::Set_ArmNullptr(bool bIsLeft)
{
	m_pArm[bIsLeft] = nullptr;
}

void CBossRemilia::Set_EyeNullptr(int idx)
{
	m_pEye[idx] = nullptr;
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
			m_vMoveTargetPos = { float(CLIENTCX >> 1) , float(CLIENTCY >> 1) , 0.f };
			break;
		case CBossRemilia::P_PATTERN1:
			m_eCurState = STATE::ATTACK;
			m_vMoveTargetPos.x = rand() % (CLIENTCX >> 1) + (CLIENTCX >> 2);
			m_vMoveTargetPos.y = rand() % (CLIENTCY >> 1) + (CLIENTCY >> 2);
			Pattern1_Init();	
			break;
		case CBossRemilia::P_MOVE1:
			m_eCurState = STATE::MOVE;
			m_vMoveTargetPos.x = rand() % (CLIENTCX >> 1) + (CLIENTCX >> 2);
			m_vMoveTargetPos.y = rand() % (CLIENTCY >> 1) + (CLIENTCY >> 2);
			break;
		case CBossRemilia::P_PATTERN2:
			m_eCurState = STATE::ATTACK;
			Pattern2_Init();
			break;
		case CBossRemilia::P_MOVE2:
			m_vMoveTargetPos.x = rand() % (CLIENTCX >> 1) + (CLIENTCX >> 2);
			m_vMoveTargetPos.y = rand() % (CLIENTCY >> 1) + (CLIENTCY >> 2);
			m_eCurState = STATE::MOVE;
			break;
		case CBossRemilia::P_PATTERN3:
			m_eCurState = STATE::ATTACK;
			Pattern3_Init();
			break;
		case CBossRemilia::P_MOVE3:
			m_vMoveTargetPos.x = rand() % (CLIENTCX >> 1) + (CLIENTCX >> 2);
			m_vMoveTargetPos.y = rand() % (CLIENTCY >> 1) + (CLIENTCY >> 2);
			m_eCurState = STATE::MOVE;
			break;
		case CBossRemilia::P_PATTERN4:
			m_eCurState = STATE::ATTACK;
			Pattern4_Init();
			break;
		case CBossRemilia::P_MOVE4_1:
			m_vMoveTargetPos.x = rand() % (CLIENTCX >> 1) + (CLIENTCX >> 2);
			m_vMoveTargetPos.y = rand() % (CLIENTCY >> 1) + (CLIENTCY >> 2);
			break;
		case CBossRemilia::P_PATTERN4_1:
			m_vMoveTargetPos = { float(CLIENTCX >> 1) , float(CLIENTCY >> 1) , 0.f };
			m_eCurState = STATE::ATTACK;
			Pattern4_Init();
			break;
		case CBossRemilia::P_SPELL1:
			m_eCurState = STATE::SPELL;
			Spell1_Init();
			break;
		case CBossRemilia::P_SPELL2:
			m_eCurState = STATE::GUNGNIL;
			Spell2_Init();
			break;
		case CBossRemilia::P_SPELL3:
			m_eCurState = STATE::GUNGNIL;
			Spell3_Init();
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

			break;
		}
		case CBossRemilia::P_PATTERN2:
			Pattern2();
			break;
		case CBossRemilia::P_MOVE2:
		{
			Pattern2();
			break;
		}
		case CBossRemilia::P_PATTERN3:

			Pattern2();
			Pattern3();
			break;
		case CBossRemilia::P_MOVE3:
		{

			break;
		}
		case CBossRemilia::P_PATTERN4:
			Pattern4();
			break;
		case CBossRemilia::P_MOVE4_1:
			break;
		case CBossRemilia::P_PATTERN4_1:
			Pattern4();
			break;
		case CBossRemilia::P_SPELL1:
			Spell1();
			break;
		case CBossRemilia::P_SPELL2:
			Spell2();
			break;
		case CBossRemilia::P_SPELL3:
			Spell3();
			break;
		default:
			break;
		}

		_vec3 vDiff = (m_vMoveTargetPos - m_tInfo.vPos);
		if (D3DXVec3Length(&vDiff) > 10.f)
		{
			D3DXVec3Normalize(&m_tInfo.vDir, &vDiff);
		}
		else
		{
			ZeroMemory(&m_tInfo.vDir, sizeof(_vec3));
		}
		
	}
}

void CBossRemilia::UpdateDie()
{
	m_tInfo.vPos += m_vDieDir;
	m_fRemainDieTime -= CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_fRemainDieTime <= 0)
	{
		m_bDestroyed = true;
	}
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
					m_eCurState = STATE::IDLE;
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
	float fSizeX = m_tInfo.vSize.x;
	float fSizeY = m_tInfo.vSize.y;
	float fReduceSizeLeft = 0.5f;
	float fReduceSizeRight = 0.5f;
	float fReduceSizeUp = 0.7f;
	float fReduceSizeDown = 0.7f;

	RECT rect = m_vecAnimation[0]->vecRect[0];

	_vec2 v2Radius = { (float)((rect.right - rect.left) * 0.5f), (float)((rect.bottom - rect.top) * 0.5f) };
	v2Radius.x *= fSizeX;
	v2Radius.y *= fSizeY;
	m_vecBodyCollision[0].tFRect =
	{
		(float)(m_tInfo.vPos.x - v2Radius.x * m_tInfo.vSize.x * fReduceSizeLeft),
		(float)(m_tInfo.vPos.y - v2Radius.y * m_tInfo.vSize.y * fReduceSizeUp),
		(float)(m_tInfo.vPos.x + v2Radius.x * m_tInfo.vSize.x * fReduceSizeRight),
		(float)(m_tInfo.vPos.y + v2Radius.y * m_tInfo.vSize.y * fReduceSizeDown)
	};
}


void CBossRemilia::Pattern1()
{
	if (m_fRemainPatternTime < 0.1f && !m_bDonePattern[B_PATTERN1])
	{
		for (int i = 0; i<12; ++i)
		{
			if (i % 3 == 0)
				continue;
			{
				for (int j = 0; j < 12; ++j)
				{
					const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Remilia_SmallBullet");
					INFO tInfo;
					tInfo.vPos = m_tInfo.vPos;
					tInfo.vDir = { cosf(D3DXToRadian(-30 * i + abs(j - 6) * 2.f)),sinf(D3DXToRadian(-30 * i + abs(j - 6) * 2.f)),0.f };
					tInfo.vSize = { 1.f, 1.f, 0.f };
					tInfo.fAngle = 0;
					CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, CRemiliaBullet::Create(pObjectInfo, tInfo, 130.f + j * 1.f));
				}
			}
		}

		m_bDonePattern[B_PATTERN1] = true;
	}

}

void CBossRemilia::Pattern1_Init()
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSS_VOICE);
	switch (rand() % 5)
	{
	case 0:
		CSoundMgr::Get_Instance()->PlaySound(L"lem_A029.wav", CSoundMgr::BOSS_VOICE);
		break;
	case 1:
		CSoundMgr::Get_Instance()->PlaySound(L"lem_A031.wav", CSoundMgr::BOSS_VOICE);
		break;
	default:
		break;
	}
	m_bDonePattern[B_PATTERN1] = false;
	for (int i = 0; i<12; ++i)
	{
		if (i % 3 == 0)
			continue;
		const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Remilia_BigBullet");
		INFO tInfo;
		tInfo.vPos = m_tInfo.vPos;
		tInfo.vDir = { cosf(D3DXToRadian(-30 * i)),sinf(D3DXToRadian(-30 * i)),0.f };
		tInfo.vSize = { 0.75f, 0.75f, 0.75f };
		tInfo.fAngle = 0;
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, CRemiliaBullet::Create(pObjectInfo, tInfo, 150.f));
	}
}

void CBossRemilia::Pattern2_Init()
{
	m_fAtkCoolTime = m_fAtkRadio[R_PATTERN2];
	m_uiPattern2Mount = 0;
}

void CBossRemilia::Pattern2()
{
	m_fAtkCoolTime -= CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_fAtkCoolTime <= 0)
	{
		m_fAtkCoolTime = m_fAtkRadio[R_PATTERN2];
		for (int i = 0; i < 3; ++i)
		{
			++m_uiPattern2Mount;
			const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Remilia_SmallBullet");
			INFO tInfo;
			tInfo.fAngle = -120 * i + m_uiPattern2Mount * -1.3f;
			tInfo.vPos = m_tInfo.vPos + 50.f * _vec3{ cosf(D3DXToRadian(tInfo.fAngle)),sinf(D3DXToRadian(tInfo.fAngle)),0.f };
			tInfo.vDir = { cosf(D3DXToRadian(tInfo.fAngle + 135.f)),sinf(D3DXToRadian(tInfo.fAngle + 135.f)),0.f };
			tInfo.vSize = { 1.0f,1.0f,0.0f };
			
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, CRemiliaBullet::Create(pObjectInfo, tInfo, 150.f));
		}
	}
}

void CBossRemilia::Pattern3_Init()
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSS_VOICE);
	switch (rand() % 5)
	{
	case 0:
		CSoundMgr::Get_Instance()->PlaySound(L"lem_A029.wav", CSoundMgr::BOSS_VOICE);
		break;
	case 1:
		CSoundMgr::Get_Instance()->PlaySound(L"lem_A031.wav", CSoundMgr::BOSS_VOICE);
		break;
	default:
		break;
	}
	m_uiPattern3Idx = 0;
	m_fAtkCoolTime2 = 0;
}

void CBossRemilia::Pattern3()
{
	m_fAtkCoolTime2 -= CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_fAtkCoolTime2 <= 0 && m_uiPattern3Idx < m_vecPattern3LazorIdx.size())
	{
		m_fAtkCoolTime2 = m_fAtkRadio[R_PATTERN3];
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::ENEMY_BULLET), 
			CRemilia_Lazor::Create(
				m_tInfo.vPos + m_vPattern3Pos[m_vecPattern3LazorIdx[m_uiPattern3Idx].first],
				m_tInfo.vPos + m_vPattern3Pos[m_vecPattern3LazorIdx[m_uiPattern3Idx].second]
				));
		++m_uiPattern3Idx;
	}

}

void CBossRemilia::Pattern4_Init()
{
	switch (rand() % 5)
	{
	case 0:
		CSoundMgr::Get_Instance()->PlaySound(L"lem_A029.wav", CSoundMgr::BOSS_VOICE);
		break;
	case 1:
		CSoundMgr::Get_Instance()->PlaySound(L"lem_A031.wav", CSoundMgr::BOSS_VOICE);
		break;
	default:
		break;
	}
	m_fAtkCoolTime = m_fAtkRadio[R_PATTERN4];
	m_fAtkCoolTime = 0;

}

void CBossRemilia::Pattern4()
{
	m_fAtkCoolTime -= CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_fAtkCoolTime <= 0)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSS_BULLET1);
		CSoundMgr::Get_Instance()->PlaySound(L"130.wav", CSoundMgr::BOSS_BULLET1);
		for (int i = 0; i < 2; ++i)
		{
			if(m_pArm[i])
				m_pArm[i]->Shoot();
		}
		m_fAtkCoolTime = m_fAtkRadio[R_PATTERN4];
	}

}

void CBossRemilia::Spell1_Init()
{
	const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"BossSkill");
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, CBossSkillEffect::Create(pObjectInfo, m_vBulletCenter));
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSS_VOICE);
	switch (rand() % 2)
	{
	case 0:
		CSoundMgr::Get_Instance()->PlaySound(L"lem_A025.wav", CSoundMgr::BOSS_VOICE);
		break;
	case 1:
		CSoundMgr::Get_Instance()->PlaySound(L"lem_A027_B.wav", CSoundMgr::BOSS_VOICE);
		break;
	}
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSS_BULLET2);
	CSoundMgr::Get_Instance()->PlaySound(L"063.wav", CSoundMgr::BOSS_BULLET2);
	
}

void CBossRemilia::Spell1()
{
}

void CBossRemilia::Spell2_Init()
{
	m_fAtkCoolTime = 0.3f;
	m_uiSpellIdx = 0;

	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::ENEMY_BULLET),
		CRemilia_Lazor::Create(
			m_tInfo.vPos + _vec3{ float(WINCX),0.f,0.f },
			m_tInfo.vPos + _vec3{ float(-WINCX),0.f,0.f }
		));
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSS_VOICE);
	switch (rand() % 3)
	{
	case 0:
		CSoundMgr::Get_Instance()->PlaySound(L"lem_A033_C.wav", CSoundMgr::BOSS_VOICE);
		break;
	case 1:
		CSoundMgr::Get_Instance()->PlaySound(L"lem_A035.wav", CSoundMgr::BOSS_VOICE);
		break;
	case 2:
		CSoundMgr::Get_Instance()->PlaySound(L"lem_A036.wav", CSoundMgr::BOSS_VOICE);
		break;
	}
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSS_BULLET2);
	CSoundMgr::Get_Instance()->PlaySound(L"063.wav", CSoundMgr::BOSS_BULLET2);
}

void CBossRemilia::Spell2()
{
	m_fAtkCoolTime -= CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_fAtkCoolTime <= 0 && m_uiSpellIdx <= m_uiMaxSpellIdx)
	{
		m_fAtkCoolTime = m_fAtkRadio[R_SPELL];
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::ENEMY_BULLET),
			CRemilia_Lazor::Create(
				m_tInfo.vPos + _vec3{ m_uiSpellIdx * 30.f, 0.f, 0.f },
				_vec3{ m_tInfo.vPos.x + CLIENTCX * (float((signed int)m_uiMaxSpellIdx - (signed int)m_uiSpellIdx * 2) / float(m_uiMaxSpellIdx)), float(-CLIENTCY), 0.f }
			));																						 
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::ENEMY_BULLET),		 
			CRemilia_Lazor::Create(																	 
				m_tInfo.vPos + _vec3{ m_uiSpellIdx * 30.f, 0.f, 0.f },
				_vec3{ m_tInfo.vPos.x + CLIENTCX * (float((signed int)m_uiMaxSpellIdx - (signed int)m_uiSpellIdx * 2) / float(m_uiMaxSpellIdx)), float(CLIENTCY * 1.5f), 0.f }
		));																							 
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::ENEMY_BULLET),		
			CRemilia_Lazor::Create(																	 
				m_tInfo.vPos + _vec3{ m_uiSpellIdx * -30.f, 0.f, 0.f },								 
				_vec3{ m_tInfo.vPos.x + CLIENTCX * -(float((signed int)m_uiMaxSpellIdx - (signed int)m_uiSpellIdx * 2) / float(m_uiMaxSpellIdx)), float(-CLIENTCY), 0.f }
		));																							 
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::ENEMY_BULLET),		 
			CRemilia_Lazor::Create(																	 
				m_tInfo.vPos + _vec3{ m_uiSpellIdx * -30.f, 0.f, 0.f },								 
				_vec3{ m_tInfo.vPos.x + CLIENTCX * -(float((signed int)m_uiMaxSpellIdx - (signed int)m_uiSpellIdx * 2) / float(m_uiMaxSpellIdx)), float(CLIENTCY * 1.5f), 0.f }
		));
		++m_uiSpellIdx;
	}
}
void CBossRemilia::Spell3_Init()
{
	m_fAtkCoolTime = 0.3f;
	m_uiSpellIdx = 0;

	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::ENEMY_BULLET),
		CRemilia_Lazor::Create(
			m_tInfo.vPos + _vec3{ 0.f,float(WINCY),0.f },
			m_tInfo.vPos + _vec3{ 0.f,float(-WINCY),0.f }
	));

}
void CBossRemilia::Spell3()
{
	m_fAtkCoolTime -= CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_fAtkCoolTime <= 0 && m_uiSpellIdx <= m_uiMaxSpellIdx)
	{
		m_fAtkCoolTime = m_fAtkRadio[R_SPELL];
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::ENEMY_BULLET),
			CRemilia_Lazor::Create(
				m_tInfo.vPos + _vec3{ 0.f, m_uiSpellIdx * 60.f, 0.f },
				_vec3{ float(-CLIENTCX), m_tInfo.vPos.y + CLIENTCY * (float((signed int)m_uiMaxSpellIdx - (signed int)m_uiSpellIdx * 4) / float(m_uiMaxSpellIdx)),  0.f }
		));
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::ENEMY_BULLET),
			CRemilia_Lazor::Create(
				m_tInfo.vPos + _vec3{ 0.f, m_uiSpellIdx * 60.f, 0.f },
				_vec3{ float(CLIENTCX * 1.5f),m_tInfo.vPos.y + CLIENTCY * (float((signed int)m_uiMaxSpellIdx - (signed int)m_uiSpellIdx * 4) / float(m_uiMaxSpellIdx)), 0.f }
		));
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::ENEMY_BULLET),
			CRemilia_Lazor::Create(
				m_tInfo.vPos + _vec3{ 0.f, m_uiSpellIdx * -60.f, 0.f },
				_vec3{ float(-CLIENTCX), m_tInfo.vPos.y + CLIENTCY * -(float((signed int)m_uiMaxSpellIdx - (signed int)m_uiSpellIdx * 4) / float(m_uiMaxSpellIdx)), 0.f }
		));
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::ENEMY_BULLET),
			CRemilia_Lazor::Create(
				m_tInfo.vPos + _vec3{ 0.f, m_uiSpellIdx * -60.f, 0.f },
				_vec3{ float(CLIENTCX * 1.5f), m_tInfo.vPos.y + CLIENTCY * -(float((signed int)m_uiMaxSpellIdx - (signed int)m_uiSpellIdx * 4) / float(m_uiMaxSpellIdx)),  0.f }
		));
		++m_uiSpellIdx;
	}
}
