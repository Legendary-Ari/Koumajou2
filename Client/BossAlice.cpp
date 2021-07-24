#include "stdafx.h"
#include "BossAlice.h"
#include "Knife.h"
#include "Effect.h"
#include "AliceBBlue.h"
#include "BAliceDoll.h"
#include "BossSkillEffect.h"
#include "SceneChanger.h"

CBossAlice::CBossAlice()
	: m_fHitMaxTime(0.2f)
	, m_ePatternIndex(P_IDLE)
	, m_fRemainDieTime(2.f)
	, m_fBlueDistance(40.f)
	, m_uiMaxBlueBullet(5)
	, m_bPaternInit(true)
	, m_fGreenDistance(20.f)
	, m_uiRemainSummon(0)
	, m_fCoolDownSummon(0.8f)
{
}


CBossAlice::~CBossAlice()
{
	Release_GameObject();

}

CGameObject * CBossAlice::Create(const OBJECTINFO * _pObjectInfo)
{
	CGameObject* pInstance = new CBossAlice;
	pInstance->Set_Prefab(_pObjectInfo);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CBossAlice::Ready_GameObject()
{
	const ANIMATION* pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + m_pObjectInfo->wstrIdleAnimImage_StateKey);
	if (!pAnim)
	{
		ERR_MSG(L"CBossAlice 이미지를 찾지 못했습니다.1");
		return E_FAIL;
	}
	m_vecAnimation.resize(STATE::STATE_END);
	m_vecAnimation[IDLE] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"Attack");
	if (!pAnim)
	{
		ERR_MSG(L"CBossAlice 이미지를 찾지 못했습니다.2");
		return E_FAIL;
	}
	m_vecAnimation[ATTACK] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"Move");
	if (!pAnim)
	{
		ERR_MSG(L"CBossAlice 이미지를 찾지 못했습니다.3");
		return E_FAIL;
	}
	m_vecAnimation[MOVE] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"Summon");
	if (!pAnim)
	{
		ERR_MSG(L"CBossAlice 이미지를 찾지 못했습니다.4");
		return E_FAIL;
	}
	m_vecAnimation[SUMMON] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"Die");
	if (!pAnim)
	{
		ERR_MSG(L"CBossAlice 이미지를 찾지 못했습니다.5");
		return E_FAIL;
	}
	m_vecAnimation[DIE] = pAnim;
	m_tInfo.vPos = {650.f, 400.f,0.f};
	m_tInfo.vSize = { 1.f, 1.f, 1.f };
	m_eCurState = IDLE;
	m_ePatternIndex = P_IDLE;
	m_fActionRemainedTime = m_fPatternTime[P_IDLE];
	m_vecBodyCollision.resize(1);

	m_uiBlueRemainBullet[0] = m_uiMaxBlueBullet;
	m_uiBlueRemainBullet[1] = m_uiMaxBlueBullet;
	m_fRemainBulletTime[0] = 0.5f;
	m_fRemainBulletTime[1] = 0.5f;

	return S_OK;
}

int CBossAlice::Update_GameObject()
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

void CBossAlice::InitUpdate_GameObject()
{

}

void CBossAlice::Late_Update_GameObject()
{
	UpdateState();
	if (m_bDead)
	{
		m_eCurState = DIE;
		m_uiAnimationFrame = 0;
	}
	UpdateAnimation();
	UpdateBodyCollision();
}

void CBossAlice::Render_GameObject()
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
	if (m_bHit )
		tColor = D3DCOLOR_ARGB(255, 150, 150, 150);
	else
		tColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &tRenderRect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, tColor);
	RenderCollision();
}

void CBossAlice::Release_GameObject()
{
	{
		_vec3 SrcPos = { 1300.f, 500.f, 0.f };
		_vec3 DstPos = { 20.f, 270.f, 0.f };
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::COLLISION, CSceneChanger::Create(SrcPos, DstPos, CScene_Manager::STAGE_2_4));
	}
}

void CBossAlice::OnOverlaped(CGameObject * _pHitObject, _vec3 vHitPos)
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

bool CBossAlice::Get_DollGoUp() const
{
	return m_ePatternIndex == PATTERN::P_PATTERN2;
}

const bool & CBossAlice::Get_Fliped() const
{
	return m_bFliped;
}

void CBossAlice::UpdatePattern()
{

	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_fActionRemainedTime -= fDeltaTime;
	if (m_fActionRemainedTime <= 0)
	{
		m_ePatternIndex = CBossAlice::PATTERN(m_ePatternIndex + 1);
		m_bPaternInit = true;

		m_vTarget = CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info().vPos;
		m_vTarget.x += rand() % 200 - 100;
		m_vTarget.y -= rand() % 50 + 50;
		

		if (m_ePatternIndex == P_PATTERN_END)
			m_ePatternIndex = P_PATTERN1;
		if (m_ePatternIndex == P_IDLE2)
		{
			m_uiBlueRemainBullet[0] = m_uiMaxBlueBullet;
			m_uiBlueRemainBullet[1] = m_uiMaxBlueBullet;
		}
		m_fActionRemainedTime = m_fPatternTime[m_ePatternIndex];
	}
	_vec3 vDiff;
	vDiff = m_vTarget - m_tInfo.vPos;
	switch (m_ePatternIndex)
	{
	case CBossAlice::P_IDLE:
		if (D3DXVec3Length(&vDiff) > 10.f)
		{
			D3DXVec3Normalize(&m_tInfo.vDir, &vDiff);
		}
		//else
		//{
		//	ZeroMemory(&m_tInfo.vDir, sizeof(_vec3));
		//}
		break;
	case CBossAlice::P_PATTERN1:
		ZeroMemory(&m_tInfo.vDir, sizeof(_vec3));
		Pattern1();
		break;
	case CBossAlice::P_IDLE2:
		if (D3DXVec3Length(&vDiff) > 10.f)
		{
			D3DXVec3Normalize(&m_tInfo.vDir, &vDiff);
		}
		//else
		//{
		//	ZeroMemory(&m_tInfo.vDir, sizeof(_vec3));
		//}
		break;
	case CBossAlice::P_PATTERN2:
		ZeroMemory(&m_tInfo.vDir, sizeof(_vec3));
		Pattern2();
		
		break;
	case CBossAlice::P_IDLE3:
		if (D3DXVec3Length(&vDiff) > 10.f)
		{
			D3DXVec3Normalize(&m_tInfo.vDir, &vDiff);
		}
		//else
		//{
		//	ZeroMemory(&m_tInfo.vDir, sizeof(_vec3));
		//}
		break;
	case CBossAlice::P_SUMMON:
		ZeroMemory(&m_tInfo.vDir, sizeof(_vec3));
		if(m_fActionRemainedTime < 1.f)
		m_bSummon = true;
		if (m_bPaternInit)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSS_VOICE);
			CSoundMgr::Get_Instance()->PlaySound(L"ali_A021.wav", CSoundMgr::BOSS_VOICE);
			m_uiRemainSummon = 3;
			m_fCoolDownSummon = 0.3f;
			const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"BossSkill");
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, CBossSkillEffect::Create(pObjectInfo,m_vBulletCenter));
		}
		break;
	case CBossAlice::P_MOVE1:
		if (D3DXVec3Length(&vDiff) > 10.f)
		{
			D3DXVec3Normalize(&m_tInfo.vDir, &vDiff);
		}
		//else
		//{
		//	ZeroMemory(&m_tInfo.vDir, sizeof(_vec3));
		//}
		break;
	case CBossAlice::P_MOVE2:
		if (D3DXVec3Length(&vDiff) > 10.f)
		{
			D3DXVec3Normalize(&m_tInfo.vDir, &vDiff);
		}
		//else
		//{
		//	ZeroMemory(&m_tInfo.vDir, sizeof(_vec3));
		//}
		break;
	default:
		break;
	}
	m_bPaternInit = false;



	Summon();



	
}

void CBossAlice::UpdateDie()
{
	m_tInfo.vPos += m_vDieDir;
	m_fRemainDieTime -= CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_fRemainDieTime <= 0)
	{
		m_bDestroyed = true;
	}
}

void CBossAlice::UpdateState()
{

	switch (m_ePatternIndex)
	{
	case CBossAlice::P_IDLE:
		m_eCurState = MOVE;
		break;
	case CBossAlice::P_PATTERN1:
		m_eCurState = ATTACK;
		break;
	case CBossAlice::P_IDLE2:
		m_eCurState = MOVE;
		break;
	case CBossAlice::P_PATTERN2:
		m_eCurState = ATTACK;
		break;
	case CBossAlice::P_IDLE3:
		m_eCurState = IDLE;
		break;
	case CBossAlice::P_SUMMON:
		m_eCurState = SUMMON;
		break;
	case CBossAlice::P_MOVE1:
		
		m_eCurState = MOVE;
		break;
	case CBossAlice::P_MOVE2:
		m_eCurState = MOVE;
		break;
	default:
		break;
	}
	if (m_eCurState == MOVE)
	{
		if (!(D3DXVec3Length(&m_tInfo.vDir) > 0))
			m_eCurState = IDLE;

	}
	if (m_eCurState == MOVE || m_eCurState == IDLE && !m_bDead)
	{
		if (m_tInfo.vPos.x > CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info().vPos.x)
			m_bFliped = true;
		else
			m_bFliped = false;
	}
}

void CBossAlice::UpdateAnimation()
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
				case IDLE:
					break;
				case ATTACK:
					m_uiAnimationFrame = m_vecAnimation[ATTACK]->vecRect.size()-1;
					break;
				case MOVE:
					m_uiAnimationFrame = m_vecAnimation[MOVE]->vecRect.size() - 1;
					break;
				case SUMMON:
					m_uiAnimationFrame = m_vecAnimation[SUMMON]->vecRect.size() - 1;
					break;
				case DIE:
					m_bDestroyed = true;
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

void CBossAlice::UpdateBodyCollision()
{
	float fSizeX = m_tInfo.vSize.x;
	float fSizeY = m_tInfo.vSize.y;
	float fReduceSizeLeft = 0.5f;
	float fReduceSizeRight = 0.5f;
	float fReduceSizeUp = 0.5f;
	float fReduceSizeDown = 0.5f;

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

void CBossAlice::Pattern1()
{
	m_fRemainBulletTime[BLUE_ABOVE] -= CTime_Manager::Get_Instance()->Get_DeltaTime();
	if(m_fRemainBulletTime[BLUE_ABOVE] < 0 && m_uiBlueRemainBullet[BLUE_ABOVE] > 0)
	{
		INFO tInfo;
		_vec3 vPos = m_vBulletCenter;
		_vec3 vDis;
		ZeroMemory(&vDis, sizeof(_vec3));
		float fAngle = 140 - (40 * (5 - m_uiBlueRemainBullet[BLUE_ABOVE]--));
		if (!m_bFliped)
			fAngle += 180;
		vDis.x = cosf(D3DXToRadian(-fAngle));
		vDis.y = sinf(D3DXToRadian(-fAngle));
		vPos += vDis * m_fBlueDistance;
		tInfo.vPos = vPos;
		tInfo.vDir = { cosf(D3DXToRadian(-(fAngle + 90.f))), sinf(D3DXToRadian(-(fAngle + 90.f))), 0.f };
		tInfo.vSize = { 1.f,1.f,1.f };
		tInfo.fAngle = fAngle + 90.f;
		CreateBlueBullet(tInfo);

		m_fRemainBulletTime[BLUE_ABOVE] = 0.2f;
	}

	m_fRemainBulletTime[BLUE_BELOW] -= CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_fRemainBulletTime[BLUE_BELOW] < 0 && m_uiBlueRemainBullet[BLUE_BELOW] > 0 && m_uiBlueRemainBullet[BLUE_ABOVE] < 2)
	{
		INFO tInfo;
		_vec3 vPos = m_vBulletCenter;
		_vec3 vDis;
		ZeroMemory(&vDis, sizeof(_vec3));	
		float fAngle = (float)220 + (40 * (5 - (int)m_uiBlueRemainBullet[BLUE_BELOW]--));
		if (!m_bFliped)
			fAngle += 180;
		vDis.x = cosf(D3DXToRadian(-fAngle));
		vDis.y = sinf(D3DXToRadian(-fAngle));
		vPos += vDis * m_fBlueDistance;
		tInfo.vPos = vPos;
		tInfo.vDir = { cosf(D3DXToRadian(-(fAngle - 90.f))), sinf(D3DXToRadian(-(fAngle - 90.f))), 0.f };
		tInfo.vSize = { 1.f,1.f,1.f };
		tInfo.fAngle = fAngle - 90.f;
		CreateBlueBullet(tInfo);

		m_fRemainBulletTime[BLUE_BELOW] = 0.2f;
	}
}

void CBossAlice::Pattern2()
{
	if (!m_bPaternInit)
		return;
	INFO tInfo;
	_vec3 vPos = m_vBulletCenter;
	_vec3 vDis;
	ZeroMemory(&vDis, sizeof(_vec3));
	for (int i = 0; i < 8; ++i)
	{
		vPos = m_vBulletCenter;
		float fAngle = (45.f * i);
		vDis.x = cosf(D3DXToRadian(-fAngle));
		vDis.y = sinf(D3DXToRadian(-fAngle));
		vPos += vDis * m_fBlueDistance;
		tInfo.vPos = vPos;
		fAngle += !m_bFliped ? 45.f : -45.f;
		tInfo.vDir = { cosf(D3DXToRadian(-fAngle)), sinf(D3DXToRadian(-fAngle)), 0.f };
		tInfo.vSize = { 1.5f,1.5f,1.5f };
		tInfo.fAngle = fAngle;
		CreateGreenBullet(tInfo);
	}
	for (int j = 0; j < 8; ++j)
	{
		vPos = m_vBulletCenter;
		float fAngle = (45.f * j - 22.5f);
		vDis.x = cosf(D3DXToRadian(-fAngle));
		vDis.y = sinf(D3DXToRadian(-fAngle));
		vPos += vDis * m_fGreenDistance;
		tInfo.vPos = vPos;
		fAngle += (!m_bFliped ? 45.f : -45.f);
		tInfo.vDir = { cosf(D3DXToRadian(-fAngle)), sinf(D3DXToRadian(-fAngle)), 0.f };
		tInfo.vSize = { 1.5f,1.5f,1.5f };
		tInfo.fAngle = fAngle;
		CreateGreenBullet(tInfo);
	}

}

void CBossAlice::Summon()
{
	m_fCoolDownSummon -= CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (!m_bSummon)
		return;
	if (!(m_uiRemainSummon > 0 && m_fCoolDownSummon <= 0))
		return;
	--m_uiRemainSummon;
	m_fCoolDownSummon = 0.6f;
	{
		const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"BlueDoll");
		INFO tInfo;
		ZeroMemory(&tInfo, sizeof(INFO));
		tInfo.vPos = m_tInfo.vPos;
		tInfo.vPos.y += 16.f;
		tInfo.vPos.x += m_fBlueDistance * (m_bFliped ? -1.f : 1.f);
		tInfo.vSize = { 1.f,1.f,0.f };
		tInfo.fAngle = 0.f;
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, CBAliceDoll::Create(pObjectInfo, tInfo, this));

	}
	{
		const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"BlueDoll");
		INFO tInfo;
		ZeroMemory(&tInfo, sizeof(INFO));
		tInfo.vPos = m_tInfo.vPos;
		tInfo.vPos.y += 16.f;
		tInfo.vPos.x += m_fBlueDistance * (m_bFliped ? -1.f : 1.f);
		tInfo.vSize = { 1.f,1.f,0.f };
		tInfo.fAngle = 180.f;
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, CBAliceDoll::Create(pObjectInfo, tInfo, this));

	}
}

void CBossAlice::CreateBlueBullet(const INFO & _tInfo)
{
	const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Alice_B_Blue");

	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, CAliceBBlue::Create(pObjectInfo, _tInfo));
}

void CBossAlice::CreateGreenBullet(const INFO & _tInfo)
{
	const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Alice_B_Green");

	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, CAliceBBlue::Create(pObjectInfo, _tInfo));

}

