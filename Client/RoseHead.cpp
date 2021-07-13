#include "stdafx.h"
#include "RoseHead.h"


CRoseHead::CRoseHead()
	:m_fOrbitRadius(30.f)
{
}


CRoseHead::~CRoseHead()
{
}

CGameObject * CRoseHead::Create(const INFO& _tInfo, const OBJECTINFO * _pObjectInfo, const CGameObject * _pRose, float _OrbitRadius)
{
	CGameObject* pInstance = new CRoseHead;
	pInstance->Set_Prefab(_pObjectInfo);
	pInstance->Set_Info(_tInfo);
	static_cast<CRoseHead*>(pInstance)->Set_Rose(_pRose);
	static_cast<CRoseHead*>(pInstance)->Set_OrbitRadius(_OrbitRadius);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CRoseHead::Ready_GameObject()
{
	const ANIMATION* pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + m_pObjectInfo->wstrIdleAnimImage_StateKey);
	if (!pAnim)
	{
		ERR_MSG(L"CRoseHead�� �̹����� ã�� ���߽��ϴ�.");
		return E_FAIL;
	}
	m_vecAnimation.resize(1);
	m_vecAnimation[0] = pAnim;

	m_bFlying = true;

	m_vecBodyCollision.resize(1);
	m_vecBodyCollision[0].eId = COLLISION::C_SPHERE;

	return S_OK;
}

void CRoseHead::InitUpdate_GameObject()
{
	
	m_vCenterPos = m_tInfo.vPos;
	if (m_tInfo.vPos.x > CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info().vPos.x)
	{
		m_vCenterPos.x -= (m_vCenterPos.x - m_pRose->Get_Info().vPos.x) * 2.f;
		m_bFliped = true;
	}
	else
	{
		m_bFliped = false;
	}
	m_tInfo.vPos = m_vCenterPos + _vec3{ m_fOrbitRadius * cosf(D3DXToRadian(-m_tInfo.fAngle)), m_fOrbitRadius * sinf(D3DXToRadian(-m_tInfo.fAngle)), 0.f };
}

int CRoseHead::Update_GameObject()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;
	if (!UpdateActive())
		return OBJ_NOEVENT;
	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_bHit)
	{
		m_fHitCumulatedTime += fDeltaTime;
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
	m_tInfo.fAngle += fDeltaTime * 100.f;
	m_tInfo.vPos = m_vCenterPos + _vec3{ m_fOrbitRadius * cosf(D3DXToRadian (-m_tInfo.fAngle)), m_fOrbitRadius * sinf(D3DXToRadian (-m_tInfo.fAngle)), 0.f };
	
	return OBJ_NOEVENT;
}

void CRoseHead::Late_Update_GameObject()
{
	UpdateBodyCollision();
}

void CRoseHead::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_vecAnimation[0]->wstrObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

	D3DXMATRIX matScale, matTrans, matWorld, matParentTransl;
	D3DXMatrixScaling(&matScale, (m_bFliped ? -1.0f : 1.0f), 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);
	matWorld = matScale * matTrans;
	const RECT& rect = m_vecAnimation[0]->vecRect[0];
	float 	fCenterX = float(((rect.right - rect.left) * 0.5f));
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
	const RECT& tRenderRect = m_vecAnimation[0]->vecRect[m_uiAnimationFrame];
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &tRenderRect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	RenderCollision();
}

void CRoseHead::Set_Rose(const CGameObject * _pRose)
{
	m_pRose = _pRose;
}

void CRoseHead::Set_OrbitRadius(float _fOrbitRadius)
{
	m_fOrbitRadius = _fOrbitRadius;
}

void CRoseHead::UpdateBodyCollision()
{
	float fSizeX = m_tInfo.vSize.x;
	float fSizeY = m_tInfo.vSize.y;
	float fReduceSizeLeft = 0.0f;
	float fReduceSizeRight = 1.0f;
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