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

	return OBJ_NOEVENT;
}

void CRuler_Sword::Set_RulerBody(const CGameObject * _pRulerBody)
{
	m_pRulerBody = _pRulerBody;
}
