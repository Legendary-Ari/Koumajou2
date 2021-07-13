#include "stdafx.h"
#include "RoseStem.h"


CRoseStem::CRoseStem()
	:m_fOrbitRadius(10.f)
{
}


CRoseStem::~CRoseStem()
{
}

CGameObject * CRoseStem::Create(const INFO& _tInfo, const OBJECTINFO * _pObjectInfo, const CGameObject * _pRose, float _OrbitRadius)
{
	CGameObject* pInstance = new CRoseStem;
	pInstance->Set_Prefab(_pObjectInfo);
	pInstance->Set_Info(_tInfo);
	static_cast<CRoseStem*>(pInstance)->Set_Rose(_pRose);
	static_cast<CRoseStem*>(pInstance)->Set_OrbitRadius(_OrbitRadius);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CRoseStem::Ready_GameObject()
{
	m_bFlying = true;
	
	m_vecAttackCollision.resize(1);
	m_vecAttackCollision[0].eId = COLLISION::C_SPHERE;

	return S_OK;
}

void CRoseStem::InitUpdate_GameObject()
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

int CRoseStem::Update_GameObject()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;
	if (!UpdateActive())
		return OBJ_NOEVENT;
	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();

	if (m_bHit)
	{
		m_fHitCumulatedTime += CTime_Manager::Get_Instance()->Get_DeltaTime();
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
	m_tInfo.vPos = m_vCenterPos + _vec3{ m_fOrbitRadius * cosf(D3DXToRadian(-m_tInfo.fAngle)), m_fOrbitRadius * sinf(D3DXToRadian(-m_tInfo.fAngle)), 0.f };
	return OBJ_NOEVENT;
}

void CRoseStem::Late_Update_GameObject()
{
	UpdateAttackCollision();
}

void CRoseStem::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_pObjectInfo->wstrObjectImage_ObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);
	matWorld = matScale * matTrans;
	const RECT& rect = m_pObjectInfo->tRect;
	float 	fCenterX = float(((rect.right - rect.left)*0.5f));
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	RenderCollision();
}

void CRoseStem::Set_Rose(const CGameObject * _pRose)
{
	m_pRose = _pRose;
}

void CRoseStem::Set_OrbitRadius(float _fOrbitRadius)
{
	m_fOrbitRadius = _fOrbitRadius;
}

void CRoseStem::UpdateAttackCollision()
{
	float fSizeX = m_tInfo.vSize.x;
	float fSizeY = m_tInfo.vSize.y;
	float fReduceSizeLeft = 0.25f;
	float fReduceSizeRight = 0.25f;
	float fReduceSizeUp = 0.25f;
	float fReduceSizeDown = 0.25f;

	RECT rect = m_pObjectInfo->tRect;

	_vec2 v2Radius = { (float)((rect.right - rect.left) * 0.5f), (float)((rect.bottom - rect.top) * 0.5f) };
	v2Radius.x *= fSizeX;
	v2Radius.y *= fSizeY;
	m_vecAttackCollision[0].tFRect =
	{
		(float)(m_tInfo.vPos.x - v2Radius.x * m_tInfo.vSize.x * fReduceSizeLeft),
		(float)(m_tInfo.vPos.y - v2Radius.y * m_tInfo.vSize.y * fReduceSizeUp),
		(float)(m_tInfo.vPos.x + v2Radius.x * m_tInfo.vSize.x * fReduceSizeRight),
		(float)(m_tInfo.vPos.y + v2Radius.y * m_tInfo.vSize.y * fReduceSizeDown)
	};
}
