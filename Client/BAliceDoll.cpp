#include "stdafx.h"
#include "BAliceDoll.h"
#include "BossAlice.h"
#include "AliceBBlue.h"

CBAliceDoll::CBAliceDoll()
	:m_fDistance(60.f)
	,m_fAtkRatio(float(rand() % 2 + 2))
	,m_fCoolTime(0.5f)
{
}


CBAliceDoll::~CBAliceDoll()
{
}

CGameObject * CBAliceDoll::Create(const OBJECTINFO * _tObjectInfo, const INFO& _tInfo, CBossAlice* _pAlice)
{
	CGameObject* pInstance = new CBAliceDoll;
	pInstance->Set_Prefab(_tObjectInfo);
	pInstance->Set_Info(_tInfo);
	static_cast<CBAliceDoll*>(pInstance)->Set_Alice(_pAlice);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CBAliceDoll::Ready_GameObject()
{
	m_vecAnimation.resize(STATE_END);

	const ANIMATION* pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + m_pObjectInfo->wstrIdleAnimImage_StateKey);
	if (!pAnim)
	{
		ERR_MSG(L"CBAliceDoll의 이미지를 찾지 못했습니다.");
		return E_FAIL;
	}
	m_vecAnimation[IDLE] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"BlueAttack");
	if (!pAnim)
	{
		ERR_MSG(L"CBAliceDoll의 이미지를 찾지 못했습니다.2");
		return E_FAIL;
	}
	m_vecAnimation[ATTACK] = pAnim;
	pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + L"BlueMove");
	if (!pAnim)
	{
		ERR_MSG(L"CBAliceDoll의 이미지를 찾지 못했습니다.3");
		return E_FAIL;
	}
	m_vecAnimation[MOVE] = pAnim;

	m_bFlying = true;
	m_tInfo.vSize = { 1.0f,1.0f,0.f };
	m_eCurState = IDLE;

	m_vecBodyCollision.resize(1);
	m_vecBodyCollision[0].eId = COLLISION::C_RECT;
	return S_OK;
}

int CBAliceDoll::Update_GameObject()
{
	if (m_bDestroyed || m_bDead)
		return OBJ_DESTROYED;
	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();



	UpdatePattern();
	return OBJ_NOEVENT;
}

void CBAliceDoll::InitUpdate_GameObject()
{
}

void CBAliceDoll::Late_Update_GameObject()
{
	UpdateState();
	UpdateAnimation();
	UpdateBodyCollision();
	if (m_tInfo.vPos.y < 0)
		m_bDestroyed = true;
}

void CBAliceDoll::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_pObjectInfo->wstrObjectImage_ObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();
	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, (m_bFliped ? -m_tInfo.vSize.x : m_tInfo.vSize.x), m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);
	matWorld = matScale * matTrans ;
	const RECT& rect = m_vecAnimation[0]->vecRect[0];
	float 	fCenterX = float(((rect.right - rect.left) * 0.5f));
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
	const RECT& tRenderRect = m_vecAnimation[m_eCurState]->vecRect[m_uiAnimationFrame];
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &tRenderRect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	RenderCollision();
	if (m_bDead)
	{
		RenderDieEffect(m_tInfo.vPos);
	}
}

void CBAliceDoll::Release_GameObject()
{
}

void CBAliceDoll::Set_Alice(CBossAlice * _pAlice)
{
	m_pAlice = _pAlice;
}

void CBAliceDoll::UpdateAnimation()
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
				case MOVE:
					break;
				case ATTACK:
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

void CBAliceDoll::UpdateBodyCollision()
{
	float fSizeX = m_tInfo.vSize.x;
	float fSizeY = m_tInfo.vSize.y;
	float fReduceSizeLeft = 0.25f;
	float fReduceSizeRight = 0.25f;
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

void CBAliceDoll::UpdatePattern()
{
	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_pAlice->Get_DollGoUp() || m_eCurState == MOVE)
	{
		m_tInfo.vPos.y -= fDeltaTime * m_pObjectInfo->fMoveSpeed;
		m_eCurState = MOVE;
		return;
	}
	else
	{
		m_tInfo.fAngle += fDeltaTime *  m_pObjectInfo->fMoveSpeed;
		_vec3 vPos = m_pAlice->Get_Center();
		_vec3 vDis = m_fDistance * _vec3{ cosf(D3DXToRadian(-m_tInfo.fAngle)),sinf(D3DXToRadian(-m_tInfo.fAngle)) ,0.f };
		m_tInfo.vPos = vPos + vDis;
	}
	
	m_fCoolTime -= fDeltaTime;
	if (m_fCoolTime <= 0)
	{
		m_fCoolTime = m_fAtkRatio;
		const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Alice_B_Yellow");
		INFO tInfo;
		ZeroMemory(&tInfo, sizeof(INFO));
		tInfo.vPos = m_tInfo.vPos;
		tInfo.vSize = { 1.f,1.f,0.f };
		tInfo.vDir = { m_pAlice->Get_Fliped() ? -1.f : 1.f, 0.f, 0.f };
		tInfo.fAngle = (!m_pAlice->Get_Fliped() ? 0.f : 180.f);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, CAliceBBlue::Create(pObjectInfo, tInfo));

	}
}
