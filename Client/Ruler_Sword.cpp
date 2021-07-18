#include "stdafx.h"
#include "Ruler_Sword.h"

const float CRuler_Sword::m_fDisBodyToSword = 70.f;
CRuler_Sword::CRuler_Sword()
	:m_pRulerBody(nullptr)
{
}


CRuler_Sword::~CRuler_Sword()
{
}

CGameObject * CRuler_Sword::Create(const ACTORINFO * _pActorInfo, const OBJECTINFO * _pObjectInfo, const CGameObject * _pRuler_Body)
{
	CGameObject* pInstance = new CRuler_Sword;
	pInstance->Set_Prefab(_pObjectInfo);
	pInstance->Set_ActorInfo(_pActorInfo);
	static_cast<CRuler_Sword*>(pInstance)->Set_RulerBody(_pRuler_Body);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CRuler_Sword::Ready_GameObject()
{
	if(FAILED(CEnemy::Ready_GameObject()))
		return E_FAIL;

	m_vecBodyCollision.resize(5);
	for (auto& tCol : m_vecBodyCollision)
	{
		tCol.eId = COLLISION::C_SPHERE;
	}
	

	return S_OK;
}

int CRuler_Sword::Update_GameObject()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_tInfo.fAngle += fDeltaTime * 30.f;
	D3DXVECTOR3 vDir = { cos(D3DXToRadian(-(m_tInfo.fAngle-90.f))), sin(D3DXToRadian (-(m_tInfo.fAngle-90.f))), 0 };
	m_tInfo.vPos = m_pRulerBody->Get_Info().vPos + vDir * m_fDisBodyToSword;

	UpdateBodyCollision();
	return OBJ_NOEVENT;
}

void CRuler_Sword::Render_GameObject()
{
	CEnemy::Render_GameObject();
	RenderCollision();
}

void CRuler_Sword::Set_RulerBody(const CGameObject * _pRulerBody)
{
	m_pRulerBody = _pRulerBody;
}

void CRuler_Sword::UpdateBodyCollision()
{
	float fSize = m_tInfo.vSize.x;
	float fReduceSize = 0.1f;

	RECT rect = m_pObjectInfo->tRect;

	_vec3 vToThis = m_tInfo.vPos - m_pRulerBody->Get_Info().vPos;
	float fDistance = D3DXVec3Length(&vToThis);
	D3DXVec3Normalize(&vToThis, &vToThis);
	_vec2 v2Radius = { (float)((rect.right - rect.left) * 0.5f), (float)((rect.bottom - rect.top) * 0.5f) };
	for (UINT i = 0; i < m_vecBodyCollision.size(); ++i)
	{
		_vec3 vColPos = m_tInfo.vPos + vToThis * (static_cast<float>((int)i)-2.f) * fDistance / m_vecBodyCollision.size();
		m_vecBodyCollision[i].tFRect =
		{
			(float)(vColPos.x - v2Radius.x * fReduceSize * m_tInfo.vSize.x),
			(float)(vColPos.y - v2Radius.y * fReduceSize * m_tInfo.vSize.y),
			(float)(vColPos.x + v2Radius.x * fReduceSize * m_tInfo.vSize.x),
			(float)(vColPos.y + v2Radius.y * fReduceSize * m_tInfo.vSize.y)
		};
	}
	
}
