#include "stdafx.h"
#include "Player.h"
#include "Prefab_Manager.h"
#include "Bullet.h"
#include "CollisionMgr.h"
#include "Knife.h"
#include "VKnife.h"
#include "VS_Clock.h"
#include "VS_Meling.h"
#include "VS_Patchuri.h"
#include "VS_Reimu.h"
#include "VS_Flandre.h"
#include "VS_Alice.h"
#include "SceneChanger.h"
#include "Item.h"

CPlayer::CPlayer()
	:m_bAttacking(false)
	, m_bCrouch(false)
	, m_bPrevIsFliped(false)
	, m_bDodge(false)
	, m_bInvincible(false)
	, m_fHitTimeLength(0.3f)
	, m_fHitCumulateTime(0.f)
	, m_bHit(false)
	, m_fInvincibleTimeLength(1.5f)
	, m_fJumpMaxTime(0.35f)
	, m_fMaxKnifeTime(0.5f)
	, m_fKnifeRemainedTime(0.f)
	, m_bFlyable(true)
	, m_fCurMp(100)
	, m_uiCurChi(50)
	, m_fFlyingSpeed(70.f)
	, m_uiLife(3)
{
}


CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject()
{
	const ANIMATION* pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + m_pObjectInfo->wstrIdleAnimImage_StateKey);
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.1");
		return E_FAIL;
	}
	m_vecAnimation.resize(STATE::STATE_END);
	m_vecAnimation[IDLE] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"S_Walking");
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.2");
		return E_FAIL;
	}
	m_vecAnimation[S_WALKING] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"Walking");
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.3");
		return E_FAIL;
	}
	m_vecAnimation[WALKING] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"Stoping");
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.4");
		return E_FAIL;
	}
	m_vecAnimation[STOPING] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"Crouch");
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.5");
		return E_FAIL;
	}
	m_vecAnimation[CROUCH] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"StandUp");
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.6");
		return E_FAIL;
	}
	m_vecAnimation[STANDUP] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"Turning");
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.7");
		return E_FAIL;
	}
	m_vecAnimation[TURNING] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"U_Jumping");
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.8");
		return E_FAIL;
	}
	m_vecAnimation[U_JUMPING] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"Jumping");
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.9");
		return E_FAIL;
	}
	m_vecAnimation[JUMPING] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"Falling");
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.10");
		return E_FAIL;
	}
	m_vecAnimation[FALLING] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"Attack");
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.11");
		return E_FAIL;
	}
	m_vecAnimation[ATTACK] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"Jump_Attack");
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.12");
		return E_FAIL;
	}
	m_vecAnimation[JUMP_ATTACK] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"Bounce");
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.13");
		return E_FAIL;
	}
	m_vecAnimation[BOUNCE] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"Hit");
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.14");
		return E_FAIL;
	}
	m_vecAnimation[HIT] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"F_Landing");
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.15");
		return E_FAIL;
	}
	m_vecAnimation[F_LANDING] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"L_Flying");
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.16");
		return E_FAIL;
	}
	m_vecAnimation[L_FLYING] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"R_Flying");
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.17");
		return E_FAIL;
	}
	m_vecAnimation[R_FLYING] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"Dodge");
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.18");
		return E_FAIL;
	}
	m_vecAnimation[DODGE] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"Dodge_Attack");
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.19");
		return E_FAIL;
	}
	m_vecAnimation[DODGE_ATTACK] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"D_Attack");
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.20");
		return E_FAIL;
	}
	m_vecAnimation[D_ATTACK] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"Jump_D_Attack");
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.21");
		return E_FAIL;
	}
	m_vecAnimation[JUMP_D_ATTACK] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"S_Knife");
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.22");
		return E_FAIL;
	}
	m_vecAnimation[S_KNIFE] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"E_Knife");
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.23");
		return E_FAIL;
	}
	m_vecAnimation[E_KNIFE] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"DS_Knife");
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.24");
		return E_FAIL;
	}
	m_vecAnimation[DS_KNIFE] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"DE_Knife");
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.25");
		return E_FAIL;
	}
	m_vecAnimation[DE_KNIFE] = pAnim;
	m_eCurState = IDLE;
	m_vecAnimation.emplace_back(pAnim);

	m_vecBodyCollision.resize(1);
	m_vecBodyCollision[0].eId = COLLISION::C_RECT;
	m_vecBodyCollision.resize(1);
	m_vecBodyTileCollision.resize(1);
	m_vecAttackCollision.resize(10);
	for (UINT i = 1; i < m_vecAttackCollision.size(); ++i)
	{
		m_vecAttackCollision[i].eId = COLLISION::C_SPHERE;
	}
	m_pBulletInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Sakuya_Knife");

	m_bJumping = false;
	m_bFalling = false;

	for (int i = 0; i < 3; ++i)
	{
		if (!m_pVSkill[i])
		{
			switch (CGame_Manager::Get_Instance()->m_uiSelectedSubWeapon[i])
			{
			case CVSkill::KNIFE:
				m_pVSkill[i] = static_cast<CVSkill*>(CVKnife::Create());
				break;
			case CVSkill::CLOCK:
				m_pVSkill[i] = static_cast<CVSkill*>(CVS_Clock::Create());
				break;
			case CVSkill::MEILING:
				m_pVSkill[i] = static_cast<CVSkill*>(CVS_Meling::Create());
				break;
			case CVSkill::ALICE:
				m_pVSkill[i] = static_cast<CVSkill*>(CVS_Alice::Create());
				break;
			case CVSkill::PATCHURI:
				m_pVSkill[i] = static_cast<CVSkill*>(CVS_Patchuri::Create());
				break;
			case CVSkill::REIMU:
				m_pVSkill[i] = static_cast<CVSkill*>(CVS_Reimu::Create());
				break;
			case CVSkill::FLANDRE:
				m_pVSkill[i] = static_cast<CVSkill*>(CVS_Flandre::Create());
				break;
			}
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::VSKILL, m_pVSkill[i]);
		}

		INFO tInfo;
		tInfo.vPos = { 40.f, 0.f, 0.f };
		tInfo.fAngle = 0.f + (45.f * i);
		tInfo.vSize = { 0.3f, 0.3f, 1.f };
		m_pVSkill[i]->Set_Info(tInfo);
	}
	

	return S_OK;
}

int CPlayer::Update_GameObject()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	UpdateMoveWithPressKey();
	UpdateJump();

	m_tInfo.vPos += m_tInfo.vDir;
	
	Offset();

	return OBJ_NOEVENT;
}

void CPlayer::Late_Update_GameObject()
{
	if (m_fCurHp <= 0.f)
		m_bDead = true;

	UpdateState();
	UpdateAnimation();
	float fMapSizeX = (float)CScroll_Manager::GetMapSizeX();
	float fMapSizeY = (float)CScroll_Manager::GetMapSizeY();
	if (m_tInfo.vPos.y > fMapSizeY)
	{
		m_tInfo.vPos = { 200.f, 300.f, 0.f };
		m_tInfo.vDir.y = 0;
		m_bDead = true;
	}

	if (m_tInfo.vPos.x < 0)
		m_tInfo.vPos.x = 0;
	if (m_tInfo.vPos.x > fMapSizeX)
		m_tInfo.vPos.x = (float)(fMapSizeX);

	UpdateBodyCollision();
	UpdateAttackCollision();
}

void CPlayer::Render_GameObject()
{
	if (!m_bVisible)
		return;
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_vecAnimation[0]->wstrObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, (m_bFliped ? -1.0f : 1.0f), 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_tInfo.fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);
	matWorld = matScale * matRotZ * matTrans;
	const RECT& rect = m_vecAnimation[0]->vecRect[0];
	float 	fCenterX = float(((rect.right - rect.left)*0.5f));
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
	const RECT& tRenderRect = m_vecAnimation[m_eCurState]->vecRect[m_uiAnimationFrame];
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &tRenderRect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	RenderCollision();
	
}

void CPlayer::Release_GameObject()
{
}

void CPlayer::OnBlocked(CGameObject * pHitObject, DIRECTION::ID _eId)
{
	m_fCurHp -= pHitObject->Get_Damage();
	m_eCurState = HIT;
	m_bBlockable = false;
	m_bHit = true;
	m_uiAnimationFrame = 0;
	m_bJumping = false;
	m_bFalling = true;
	m_bOnGround = false;
	m_bFlying = false;
	m_fJumpRamainedTime = 0;
}

void CPlayer::Set_OnGround(bool _b)
{
	if (m_bFlying)
		return;
	m_bOnGround = _b;
	if (_b)
	{
		m_bFlyable = true;
	}
}

CGameObject * CPlayer::Create(const OBJECTINFO* _pPrefab, const INFO& _tInfo )
{
	CGameObject* pInstance = new CPlayer;
	pInstance->Set_Prefab(_pPrefab);
	pInstance->Set_Info(_tInfo);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

void CPlayer::OnBlockedTile(CGameObject * pHitObject, DIRECTION::ID _eId)
{
	if (m_bFlying)
		return;
	if (!m_bJumping && _eId == DIRECTION::S)
	{
		//m_bFlyable = true;
		//m_fJumpRamainedTime = 0.f;
		m_bFalling = false;
		m_bJumping = false;
		m_tInfo.vDir.y = 0;
		m_bFlyable = true;
	}
}

void CPlayer::OnOverlaped(CGameObject* _pHitObject, _vec3 vHitPos)
{
	if (dynamic_cast<CItem*>(_pHitObject))
		return;
	CSceneChanger* _pChanger = dynamic_cast<CSceneChanger*>(_pHitObject);
	if (_pChanger)
	{
		_pChanger->Set_Change();
		return;
	}
	if (m_bHit)
		return;
	CGameObject::OnOverlaped(_pHitObject, vHitPos);
	m_eCurState = HIT;
	m_bBlockable = false;
	m_bOverlapable = false;
	m_bHit = true;
	m_bFalling = true;
	m_bFlying = false;
	m_uiAnimationFrame = 0;
	m_fJumpRamainedTime = 0;
}

void CPlayer::UpdateState()
{
	if (m_bFlying)
		m_tInfo.vDir.y = 0;
	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (!m_bBlockable)
	{
		m_fHitCumulateTime += fDeltaTime;

		if (!m_bHit && m_fHitCumulateTime > m_fInvincibleTimeLength)
		{
			m_fHitCumulateTime = 0;
			m_bBlockable = true;
			m_tInfo.vDir.y = 0;
			m_bOnGround = false;
			m_bVisible = true;
			m_bOverlapable = true;
		} 
		else if (m_fHitCumulateTime > m_fHitTimeLength)
		{
			m_bHit = false;
			if (m_bDead)
			{
				CScene_Manager::Get_Instance()->Reset();
				m_bDead = false;
				m_fCurHp = 100.f;
				m_fCurMp = 100.f;
				--m_uiLife;
				m_tInfo.vPos = CScene_Manager::Get_Instance()->Get_StartPos();
				D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();
				D3DXVECTOR3 vDiff = m_tInfo.vPos - D3DXVECTOR3{ float(CLIENTCX >> 1), float(CLIENTCY >> 1), 0.f };
				CScroll_Manager::Force_Set_Scroll(-vDiff);
			}
		}
		else
			return;

		if (!m_bBlockable && !m_bHit)
			m_bVisible = !m_bVisible;
	}
		
	CPlayer::STATE eState = IDLE;

	float fMoveSpeed = fDeltaTime * m_pObjectInfo->fMoveSpeed;
	if (!m_bAttacking && !m_bKnifeAttacking)
	{

		if (m_bJumping && m_eCurState != FALLING)
		{
			if (m_eCurState != FALLING)
			{
				if (m_eCurState == U_JUMPING)
					eState = U_JUMPING;
				else if (m_eCurState == JUMPING)
					eState = JUMPING;
				else if (abs(m_tInfo.vDir.x) < (fMoveSpeed * 0.5f))
					eState = U_JUMPING;
				else
					eState = JUMPING;
			}
		}
		else
		{
			if (m_bFalling || m_eCurState == FALLING && !m_bOnGround)
				eState = FALLING;
			else
			{
				if (!m_bCrouch && m_eCurState == CROUCH)
					eState = STANDUP;
				else if(m_bCrouch)
					eState = CROUCH;
				else
				{
					if (m_tInfo.vDir.x != 0)
					{
						if (m_bFliped != m_bPrevIsFliped)
							eState = TURNING;
						else if (m_eCurState == S_WALKING || m_eCurState == IDLE)
							eState = S_WALKING;
						else if (m_bStoping || m_eCurState == STOPING)
							eState = STOPING;
						else if (m_eCurState == WALKING) // 마지막에서 두번째
							eState = WALKING;
						else
							eState = IDLE;
					}
				}
			}
		}
	}
	else if (m_bAttacking)
	{
		if (!m_bOnGround)
		{
			if(m_bCrouch)
				eState = JUMP_D_ATTACK;
			else
				eState = JUMP_ATTACK;
		}
		else
		{
			if (m_bFalling)
				eState = JUMP_ATTACK;
			else
			{
				if (m_bCrouch)
					eState = D_ATTACK;
				else
				{
					eState = ATTACK;
				}
			}
		}
	}
	else if (m_bKnifeAttacking)
	{
		if (!m_bOnGround)
		{
			m_bKnifeAttacking = false;
		}			
		if (m_bJumping && m_eCurState != FALLING)
		{
			if (m_eCurState != FALLING)
			{
				if (m_eCurState == U_JUMPING)
					eState = U_JUMPING;
				else if (m_eCurState == JUMPING)
					eState = JUMPING;
				else if (abs(m_tInfo.vDir.x) < (fMoveSpeed * 0.5f))
					eState = U_JUMPING;
				else
					eState = JUMPING;
			}
		}
		else
		{
			if (m_bFalling || m_eCurState == FALLING && !m_bOnGround)
				eState = FALLING;
			else
			{
				if (!m_bCrouch && m_eCurState == CROUCH)
					eState = STANDUP;
				else if (m_bCrouch && m_eCurState != DE_KNIFE)
				{
					eState = DS_KNIFE;
				}
				else if (m_bCrouch && m_eCurState == DE_KNIFE)
				{
					eState = DE_KNIFE;
				}
				else if (m_eCurState != E_KNIFE)
				{
					eState = S_KNIFE;
				}
				else if (m_eCurState == E_KNIFE)
				{
					eState = E_KNIFE;
				}
			}
		}
	}

	m_bPrevIsFliped = m_bFliped;
	m_eCurState = eState;

	if (m_eCurState != m_ePrevState)
	{
		m_uiAnimationFrame = 0;

		m_ePrevState = m_eCurState;
	}

}

void CPlayer::UpdateAnimation()
{
	m_fAnimationCumulatedTime += CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_fAnimationCumulatedTime > m_vecAnimation[m_eCurState]->fPlay_Speed)
	{
		++m_uiAnimationFrame;
		if (m_uiAnimationFrame >= m_vecAnimation[m_eCurState]->vecRect.size())
		{
			if(!m_vecAnimation[m_eCurState]->bLoop)
			{
				UINT uiTempFrame = m_uiAnimationFrame;
				m_uiAnimationFrame = 0;
				switch (m_eCurState)
				{
				case CPlayer::S_WALKING:
					m_eCurState = WALKING;
					break;
				case CPlayer::STOPING:
					m_tInfo.vDir.x = 0;
					m_eCurState = IDLE;
					break;
				case CPlayer::CROUCH:
					m_uiAnimationFrame = m_vecAnimation[m_eCurState]->vecRect.size()-1;
					break;
				case CPlayer::STANDUP:
					m_eCurState = IDLE;
					break;
				case CPlayer::TURNING:
					m_eCurState = WALKING;
					break;
				case CPlayer::U_JUMPING:
					m_eCurState = FALLING;
				case CPlayer::JUMPING:
					m_eCurState = FALLING;
					break;
				case CPlayer::ATTACK:
					m_bAttacking = false;
					m_eCurState = IDLE;
					break;
				case CPlayer::JUMP_ATTACK:
					m_bAttacking = false;
					m_eCurState = FALLING;
					break;
				case CPlayer::HIT:
					m_eCurState = FALLING;
					m_bFalling = true;
					m_bOnGround = false;
					m_bHit = false;

					break;
				case CPlayer::F_LANDING:
					m_uiAnimationFrame = m_vecAnimation[m_eCurState]->vecRect.size() - 1;
					break;
				case CPlayer::L_FLYING:
					break;
				case CPlayer::R_FLYING:
					break;
				case CPlayer::DODGE:
					m_eCurState = IDLE;
					break;
				case CPlayer::DODGE_ATTACK:
					m_bAttacking = false;
					break;
				case CPlayer::D_ATTACK:
					m_bAttacking = false;
					m_ePrevState = CROUCH;
					m_eCurState = CROUCH;
					m_uiAnimationFrame = m_vecAnimation[CROUCH]->vecRect.size()-1;
					break;
				case CPlayer::JUMP_D_ATTACK:
					m_bAttacking = false;
					m_eCurState = FALLING;
					break;
				case CPlayer::S_KNIFE:
					m_uiAnimationFrame = m_vecAnimation[m_eCurState]->vecRect.size() - 1;
					if (m_fKnifeRemainedTime <= 0)
						m_eCurState = E_KNIFE;
					break;
				case CPlayer::E_KNIFE:
					m_bKnifeAttacking = false;
					m_eCurState = IDLE;
					break;
				case CPlayer::DS_KNIFE:
					m_uiAnimationFrame = m_vecAnimation[m_eCurState]->vecRect.size() - 1;
					if (m_fKnifeRemainedTime <= 0)
						m_eCurState = DE_KNIFE;
					break;
				case CPlayer::DE_KNIFE:
					m_bKnifeAttacking = false;
					m_eCurState = CROUCH;
					m_ePrevState = CROUCH;
					m_uiAnimationFrame = m_vecAnimation[CROUCH]->vecRect.size() - 1;
					break;
				case CPlayer::STATE_END:
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

void CPlayer::UpdateBodyCollision()
{
	float fSize = m_tInfo.vSize.x;
	float fReduceSizeX = 0.3f;
	float fReduceSizeUp = 0.6f;
	float fReduceSizeDown = 1.f;
	if (m_bCrouch && m_bOnGround)
		fReduceSizeUp = -0.2f;
	if (m_bJumping)
	{
		fReduceSizeDown = 0.7f + abs((m_vecAnimation[JUMPING]->vecRect.size()-1) * 0.5f - m_uiAnimationFrame) * 0.6f / (m_vecAnimation[JUMPING]->vecRect.size()+1);
	}
		
	RECT rect = m_vecAnimation[0]->vecRect[0];
	
	_vec2 v2Radius = { (float)((rect.right - rect.left) * 0.5f), (float)((rect.bottom - rect.top) * 0.5f) };
	v2Radius.x *= m_tInfo.vSize.x;
	v2Radius.y *= m_tInfo.vSize.y;
	m_vecBodyCollision[0].tFRect = 
	{
		(float)(m_tInfo.vPos.x - v2Radius.x * fReduceSizeX * m_tInfo.vSize.x),
		(float)(m_tInfo.vPos.y - v2Radius.y * fReduceSizeUp * m_tInfo.vSize.y),
		(float)(m_tInfo.vPos.x + v2Radius.x * fReduceSizeX * m_tInfo.vSize.x),
		(float)(m_tInfo.vPos.y + v2Radius.y * fReduceSizeDown * m_tInfo.vSize.y)
	};

	m_vecBodyTileCollision[0].tFRect =
	{
		(float)(m_tInfo.vPos.x) - (float)(v2Radius.x * fReduceSizeX ),
		(float)(m_tInfo.vPos.y) - (float)(v2Radius.y * fReduceSizeUp),
		(float)(m_tInfo.vPos.x) + (float)(v2Radius.x * fReduceSizeX ),
		(float)(m_tInfo.vPos.y) + (float)(v2Radius.y)
	};
}

void CPlayer::UpdateAttackCollision()
{
	RECT rect = m_vecAnimation[0]->vecRect[0];

	_vec2 v2Radius = { (float)((rect.right - rect.left) * 0.5f), (float)((rect.bottom - rect.top) * 0.5f) };
	v2Radius.x *= m_tInfo.vSize.x;
	v2Radius.y *= m_tInfo.vSize.y;
	if (m_bOnGround && m_bAttacking && m_bCrouch && m_uiAnimationFrame > 4 && m_uiAnimationFrame < 8)
	{
		m_vecAttackCollision[0].tFRect = FRECT();
		for (UINT i = 1; i < m_vecAttackCollision.size(); ++i)
		{
			m_vecAttackCollision[i].tFRect.left = (float)(m_tInfo.vPos.x + v2Radius.x * 0.25f * (i - 0.25f));
			m_vecAttackCollision[i].tFRect.top = (float)(m_tInfo.vPos.y - v2Radius.y * (0.4f - (i - 1) * 0.1f));
			m_vecAttackCollision[i].tFRect.right = (float)(m_tInfo.vPos.x + v2Radius.x * 0.25f * i);
			m_vecAttackCollision[i].tFRect.bottom = (float)(m_tInfo.vPos.y - v2Radius.y * (0.4f - i) * 0.1f);
			if (m_bFliped)
			{
				float fTemp = m_vecAttackCollision[i].tFRect.left;
				m_vecAttackCollision[i].tFRect.left = m_tInfo.vPos.x - (m_vecAttackCollision[i].tFRect.right - m_tInfo.vPos.x);
				m_vecAttackCollision[i].tFRect.right = m_tInfo.vPos.x - (fTemp - m_tInfo.vPos.x);
			}
		}

	}
	else if (m_bAttacking && m_bCrouch && m_uiAnimationFrame > 4 && m_uiAnimationFrame < 8)
	{
		m_vecAttackCollision[0].tFRect = FRECT();
		for (UINT i = 1; i < m_vecAttackCollision.size(); ++i)
		{
			m_vecAttackCollision[i].tFRect.left = (float)(m_tInfo.vPos.x + v2Radius.x * 0.25f * (i - 0.25f));
			m_vecAttackCollision[i].tFRect.top = (float)(m_tInfo.vPos.y - v2Radius.y * (0.5f - (i - 1) * 0.1f));
			m_vecAttackCollision[i].tFRect.right = (float)(m_tInfo.vPos.x + v2Radius.x * 0.25f * i);
			m_vecAttackCollision[i].tFRect.bottom = (float)(m_tInfo.vPos.y - v2Radius.y * (0.5f - i) * 0.1f);
			if (m_bFliped)
			{
				float fTemp = m_vecAttackCollision[i].tFRect.left;
				m_vecAttackCollision[i].tFRect.left = m_tInfo.vPos.x - (m_vecAttackCollision[i].tFRect.right - m_tInfo.vPos.x);
				m_vecAttackCollision[i].tFRect.right = m_tInfo.vPos.x - (fTemp - m_tInfo.vPos.x);
			}
		}
	}
	else if (!m_bCrouch && m_bAttacking && m_uiAnimationFrame > 2 && m_uiAnimationFrame < 12)
	{
		m_vecAttackCollision[0].tFRect =
		{
			(float)(m_tInfo.vPos.x) ,
			(float)(m_tInfo.vPos.y - v2Radius.y * 0.3f) ,
			(float)(m_tInfo.vPos.x) + v2Radius.x * 2.5f,
			(float)(m_tInfo.vPos.y - v2Radius.y * 0.1f)
		};
		if (m_bFliped)
		{
			float fTemp = m_vecAttackCollision[0].tFRect.left;
			m_vecAttackCollision[0].tFRect.left = m_tInfo.vPos.x - (m_vecAttackCollision[0].tFRect.right - m_tInfo.vPos.x);
			m_vecAttackCollision[0].tFRect.right = m_tInfo.vPos.x - (fTemp - m_tInfo.vPos.x);
		}
		for (UINT i = 1; i < m_vecAttackCollision.size(); ++i)
		{
			m_vecAttackCollision[i].tFRect = FRECT();
		}
	}
	else
	{
		for (UINT i = 0; i < m_vecAttackCollision.size(); ++i)
		{
			m_vecAttackCollision[i].tFRect = FRECT();
		}
	}
}

void CPlayer::UpdateMoveWithPressKey()
{
	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_bFlying)
	{
		m_fCurMp -= fDeltaTime * 25.f;
		if (m_fCurMp < 0)
		{
			m_fCurMp = 0;
			m_bFlying = false;
			m_bFalling = true;
		}
			
	}
	else
	{
		m_fCurMp += fDeltaTime * 50.f;
		if (m_fCurMp >= 100.f)
			m_fCurMp = 100.f;
	}

	if (CKey_Manager::Get_Instance()->Key_Down(KEY_V))
	{
		CVSkill* pTemp = m_pVSkill[NEXT];
		m_pVSkill[NEXT] = m_pVSkill[CUR];
		m_pVSkill[CUR] = m_pVSkill[PREV];
		m_pVSkill[PREV] = pTemp;

		m_pVSkill[NEXT]->Set_Angle(90.f);
		m_pVSkill[CUR]->Set_Angle(45.f);
		m_pVSkill[PREV]->Set_Angle(0.f);
	}

	
	if (m_fKnifeRemainedTime > 0)
		m_fKnifeRemainedTime -= fDeltaTime;
	else
		m_fKnifeRemainedTime = 0.f;
	float fMoveSpeed = fDeltaTime * m_pObjectInfo->fMoveSpeed;

	if (m_bHit)
	{
		m_tInfo.vDir.x = m_bFliped ? fMoveSpeed : -fMoveSpeed;
		fMoveSpeed *= 0.5f;
		m_tInfo.vDir.y = -fMoveSpeed;
		return;
	}		
	if (m_bFlying)
		fMoveSpeed = fDeltaTime * m_fFlyingSpeed;

	float fTargetDirX{ 0.f };

	if (!(m_bAttacking && m_bOnGround && m_bKnifeAttacking))
	{
		if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_LEFT))
		{
			m_bFliped = true;
			fTargetDirX = -fMoveSpeed;
		}
		else if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_RIGHT))
		{
			m_bFliped = false;
			fTargetDirX = +fMoveSpeed;
		}
		else
		{
			fTargetDirX = 0;
		}

		if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_DOWN))
		{
			m_bCrouch = true;
			if (m_bFlying)
			{
				m_tInfo.vDir.y = fDeltaTime * 30.f;
			}
			else
			{
				
				if (m_bOnGround)
				{
					fTargetDirX = 0;
				}
			}
		}
		else
		{
			m_bCrouch = false;
		}

	}
	if (m_bFlying && CKey_Manager::Get_Instance()->Key_Pressing(KEY_UP))
	{
		m_tInfo.vDir.y = -fDeltaTime * m_fFlyingSpeed;
	}
	if (m_bFlying && CKey_Manager::Get_Instance()->Key_Pressing(KEY_DOWN))
	{
		m_bCrouch = true;
		m_tInfo.vDir.y = fDeltaTime * m_fFlyingSpeed;
	}
	if (!m_bAttacking && !m_bKnifeAttacking)
	{
		if (CKey_Manager::Get_Instance()->Key_Down(KEY_Z))
		{
			m_bAttacking = true;
			if (m_bOnGround)
			{
				m_tInfo.vDir.x = 0;
				fTargetDirX = 0;
			}
			if (m_bFlying)
			{
				m_bFalling = true;
				m_bFlying = false;
			}
		}
		if (CKey_Manager::Get_Instance()->Key_Down(KEY_C))
		{
			
			m_bKnifeAttacking = true;
			if (m_bOnGround)
			{
				m_tInfo.vDir.x = 0;
				fTargetDirX = 0;
				m_fKnifeRemainedTime = m_fMaxKnifeTime;

				if (dynamic_cast<CVKnife*>(m_pVSkill[CUR]))
				{
					INFO tBullet;
					tBullet.vPos = m_tInfo.vPos;
					tBullet.vPos.y -= 10.f;
					tBullet.fAngle = !m_bFliped ? 0.f : 180.f;
					CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)m_pBulletInfo->eObjId, CKnife::Create(m_pBulletInfo, tBullet.vPos, tBullet.fAngle));
					tBullet.vPos.y += 20.f;
					CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)m_pBulletInfo->eObjId, CKnife::Create(m_pBulletInfo, tBullet.vPos, tBullet.fAngle));
				}
				else
				{
					INFO tBullet;
					tBullet.vPos = m_tInfo.vPos;
					tBullet.fAngle = !m_bFliped ? 0.f : 180.f;
					m_pVSkill[CUR]->Use(tBullet);
				}
			}
			else
			{
				m_bKnifeAttacking = true;
				m_fKnifeRemainedTime = 0.001f;

				INFO tBullet;
				tBullet.vPos = m_tInfo.vPos;
				tBullet.fAngle = !m_bFliped ? 0.f : 180.f;
				m_pVSkill[CUR]->Use(tBullet);
			}
			if (dynamic_cast<CVKnife*>(m_pVSkill[CUR]))
			{
				CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)m_pBulletInfo->eObjId,CKnife::Create(m_pBulletInfo, m_tInfo.vPos, !m_bFliped ? 0.f : 180.f));
			}
		}
	}



	if (CKey_Manager::Get_Instance()->Key_Down(KEY_F1))
	{
		m_bRenderCollision = !m_bRenderCollision;
	}

	bool bXKeyDown = CKey_Manager::Get_Instance()->Key_Down(KEY_X);
	if (!m_bFalling && m_bOnGround && bXKeyDown)
	{
		m_bJumping = true;
		m_bFalling = false;
		m_bOnGround = false;
		m_fJumpRamainedTime = m_fJumpMaxTime;
	}
	else if (m_bFlyable && bXKeyDown)
	{
		m_bFlying = true;
		m_bFlyable = false;
		m_fJumpRamainedTime = 0;
	}
	else if (m_bFlying && bXKeyDown)
	{
		m_bFlying = false;
		m_bFalling = true;
	}
	

	bool bXKeyUp= CKey_Manager::Get_Instance()->Key_Up(KEY_X);
	if (m_bJumping && bXKeyUp)
	{
		m_bJumping = false;
		m_bFalling = true;
		m_bOnGround = false;
		m_fJumpRamainedTime = 0.f;
	}
	else if (m_bJumping)
	{
		m_bJumping = true;
		m_bFalling = false;
		m_bOnGround = false;
	}
	

	//if (!m_bFalling && !m_bJumping && CKey_Manager::Get_Instance()->Key_Pressing(KEY_X))
	//{
	//	m_bJumping = true;
	//	m_bFalling = false;
	//	m_bOnGround = false;
	//	m_fJumpRamainedTime = m_fJumpMaxTime;
	//}
	//else if (!m_bFalling && m_bJumping && !CKey_Manager::Get_Instance()->Key_Pressing(KEY_X))
	//{
	//	m_bJumping = false;
	//	m_bFalling = true;
	//	m_bOnGround = false;
	//	m_fJumpRamainedTime = 0.f;
	//
	//}
	//else if (!m_bFlying && m_bFalling && m_bFlyable && CKey_Manager::Get_Instance()->Key_Down(KEY_X))
	//{
	//	m_bFalling = false;
	//	m_bFlying = true;
	//	m_bFlyable = false;
	//}
	//else if (m_bFlying && CKey_Manager::Get_Instance()->Key_Down(KEY_X))
	//{
	//	m_bFalling = true;
	//	m_bFlying = false;
	//}
	//else 
	//{
	//	if (m_fJumpRamainedTime > 0.f)
	//		m_fJumpRamainedTime -= fDeltaTime;
	//	if (m_fJumpRamainedTime <= 0)
	//	{
	//		m_bJumping = false;
	//		m_bFalling = true;
	//		m_bOnGround = false;
	//		m_fJumpRamainedTime = 0.f;
	//	}
	//}

	
	
	m_tInfo.vDir.x = CMyMath::FInterpTo(m_tInfo.vDir.x, fTargetDirX, fDeltaTime, 5.f);
	
	if (abs(abs(m_tInfo.vDir.x) - fTargetDirX) < 1.f)
		m_tInfo.vDir.x = fTargetDirX;

	if (fTargetDirX == 0 && abs(m_tInfo.vDir.x) > fMoveSpeed * 0.5f)
		m_bStoping = true;
	else
		m_bStoping = false;

	if (!m_bJumping && !m_bOnGround)
	{
		UpdateGravity();
		m_bFalling = true;
	}

}

void CPlayer::Offset()
{
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();
	D3DXVECTOR3 vDiff = m_tInfo.vPos - D3DXVECTOR3{ float(CLIENTCX>>1), float(CLIENTCY>>1), 0.f};
	CScroll_Manager::Set_Scroll(-vDiff);
	
}

void CPlayer::UpdateJump()
{
	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_bJumping)
	{
		m_tInfo.vDir.y = -300.f * fDeltaTime;
		if (m_fJumpRamainedTime > 0.f)
			m_fJumpRamainedTime -= fDeltaTime;
		if (m_fJumpRamainedTime <= 0)
		{
			m_bJumping = false;
			m_bFalling = true;
			m_bOnGround = false;
			m_fJumpRamainedTime = 0.f;
		}
	}

}

void CPlayer::InitUpdate_GameObject()
{
	UpdateBodyCollision();
}
