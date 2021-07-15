#include "stdafx.h"
#include "BossAlice.h"


CBossAlice::CBossAlice()
	: m_fHitMaxTime(0.2f)
{
}


CBossAlice::~CBossAlice()
{
}

CGameObject * CBossAlice::Create(OBJECTINFO * _pObjectInfo)
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
	m_tInfo.vPos = {650.f, 400.f,0.f};
	m_tInfo.vSize = { 1.f, 1.f, 1.f };
}

int CBossAlice::Update_GameObject()
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
		m_fHitRemainTime -= fDeltaTime;
		ZeroMemory(&m_vecBodyCollision[0].tFRect, sizeof(FRECT));
		if (m_fHitRemainTime < 0)
		{
			m_bHit = false;
			m_fHitRemainTime = 0;
		}

	}

	UpdatePattern();
}

void CBossAlice::InitUpdate_GameObject()
{
}

void CBossAlice::Late_Update_GameObject()
{
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
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &tRenderRect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	RenderCollision();
}

void CBossAlice::Release_GameObject()
{
}

void CBossAlice::OnOverlaped(CGameObject * _pHitObject, _vec3 vHitPos)
{
}

bool CBossAlice::UpdateActive()
{
	return false;
}

void CBossAlice::UpdatePattern()
{
}

void CBossAlice::UpdateAnimation()
{
}
