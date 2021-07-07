#include "stdafx.h"
#include "Player.h"
#include "Prefab_Manager.h"
#include "Bullet.h"

CPlayer::CPlayer()
	:m_ePrevState(STATE::STATE_END)
	,m_eCurState(STATE::STATE_END)
	, m_fAnimationCumulatedTime(0.f)
	,m_bFalling(false)
	,m_bJumping(false)
	, m_bAttacking(false)
	, m_bCrouch(false)
	, m_bPrevIsFliped(false)
{
}


CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject()
{
	m_tInfo = m_pActorInfo->tInfo;
	
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
	m_eCurState = IDLE;
	m_vecAnimation.emplace_back(pAnim);
	
	m_bJumping = false;
	m_bFalling = false;
	return S_OK;
}

int CPlayer::Update_GameObject()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	UpdateMoveWithPressKey();

	UpdateState();
	UpdateAnimation();	

	m_tInfo.vPos += m_tInfo.vDir;

	Offset();

	return OBJ_NOEVENT;
}

void CPlayer::Late_Update_GameObject()
{
	if (m_tInfo.vPos.y > WINCY - 100)
	{
		m_tInfo.vPos.y = WINCY - 100;
		m_bFalling = false;
		m_bJumping = false;
		m_eCurState = IDLE;
		m_tInfo.vDir.y = 0;
	}
}

void CPlayer::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_vecAnimation[0]->wstrObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, (m_bIsFliped ? -1.0f : 1.0f), 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_tInfo.fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);
	matWorld = matScale * matRotZ * matTrans;
	const RECT& rect = m_vecAnimation[0]->vecRect[0];
	float 	fCenterX = float(((rect.right - rect.left)*0.5f));
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
	const RECT& tRenderRect = m_vecAnimation[m_eCurState]->vecRect[m_uiAnimationFrame];
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &tRenderRect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	//TCHAR szBuff[32];
	//swprintf_s(szBuff, L"Y : %f", m_tInfo.vDir.y);
	//D3DXMATRIX mat;
	//D3DXMatrixTranslation(&mat, 100.f, 100.f, 0.f);

	//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&mat);
	//CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), szBuff, lstrlen(szBuff), nullptr, 0, D3DCOLOR_ARGB(255, 255, 0, 0));


}

void CPlayer::Release_GameObject()
{
}

CGameObject * CPlayer::Create(const ACTORINFO* _pActorInfo, const OBJECTINFO* _pPrefab)
{
	CGameObject* pInstance = new CPlayer;
	pInstance->Set_Prefab(_pPrefab);
	pInstance->Set_ActorInfo(_pActorInfo);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

void CPlayer::UpdateState()
{
	CPlayer::STATE eState = IDLE;
	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	float fMoveSpeed = fDeltaTime * m_pObjectInfo->fMoveSpeed;
	if (!m_bAttacking)
	{
		if (m_tInfo.vDir.y <= -14.0f && m_bJumping)
		{
			m_bJumping = false;
			m_bFalling = true;
			eState = CPlayer::FALLING;
		}
		else
		{
			if (m_bJumping)
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
				if (m_bFalling)
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
							if (m_bIsFliped != m_bPrevIsFliped)
								eState = TURNING;
							else if (m_eCurState == S_WALKING || m_eCurState == IDLE)
								eState = S_WALKING;
							else if (m_bStoping)
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
	}
	else
	{
		if (m_bJumping)
		{
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

	m_bPrevIsFliped = m_bIsFliped;
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
					//m_bFalling = true;
					m_eCurState = FALLING;
					break;
				case CPlayer::ATTACK:
					m_bAttacking = false;
					m_eCurState = IDLE;
					break;
				case CPlayer::JUMP_ATTACK:
					break;
				case CPlayer::HIT:
					break;
				case CPlayer::F_LANDING:
					break;
				case CPlayer::L_FLYING:
					break;
				case CPlayer::R_FLYING:
					break;
				case CPlayer::DODGE:
					break;
				case CPlayer::DODGE_ATTACK:
					break;
				case CPlayer::D_ATTACK:
					break;
				case CPlayer::JUMP_D_ATTACK:
					break;
				case CPlayer::KNIFE:
					break;
				case CPlayer::D_KNIFE:
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

void CPlayer::UpdateMoveWithPressKey()
{
	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	float fMoveSpeed = fDeltaTime * m_pObjectInfo->fMoveSpeed;
	float fTargetDirX{ 0.f };
	if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_LEFT))
	{
		m_bIsFliped = true;
		fTargetDirX = -fMoveSpeed;
	}
	else if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_RIGHT))
	{
		m_bIsFliped = false;
		fTargetDirX = +fMoveSpeed;
	}
	else
	{
		fTargetDirX = 0;
	}

	if (!m_bFalling && !m_bJumping && CKey_Manager::Get_Instance()->Key_Pressing(KEY_X) && m_tInfo.vDir.y > -12.f)
	{
		m_bJumping = true;
		m_bFalling = false;
	}
	else if (!m_bFalling && m_bJumping && (m_tInfo.vDir.y < -12.f || !CKey_Manager::Get_Instance()->Key_Pressing(KEY_X)))
	{
		m_tInfo.vDir.y = -7.f;
		m_bJumping = false;
		m_bFalling = true;
	}

	if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_DOWN))
	{
		m_bCrouch = true;
	}
	else
	{
		m_bCrouch = false;
	}

	if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_C))
	{
		m_bAttacking = true;
	}

	if (m_bJumping)
	{
		m_tInfo.vDir.y -= (12.f - m_tInfo.vDir.y) * 0.05f;

	}
	
	m_tInfo.vDir.x = CMyMath::FInterpTo(m_tInfo.vDir.x, fTargetDirX, fDeltaTime, 8.f);

	if (fTargetDirX == 0 && m_tInfo.vDir != 0)
		m_bStoping = true;
	else
		m_bStoping = false;

	if (m_bFalling)
	{
		m_tInfo.vDir.y += 0.98f;
	}


}

void CPlayer::Offset()
{
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();
	D3DXVECTOR3 vDiff = m_tInfo.vPos - D3DXVECTOR3{ float(WINCX>>1), float(WINCY>>1), 0.f};
	CScroll_Manager::Set_Scroll(-vDiff);
}
