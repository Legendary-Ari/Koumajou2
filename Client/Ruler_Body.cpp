#include "stdafx.h"
#include "Ruler_Body.h"
#include "Prefab_Manager.h"
#include "Ruler_Sword.h"

CRuler_Body::CRuler_Body()
{
}


CRuler_Body::~CRuler_Body()
{
}

CGameObject * CRuler_Body::Create(const ACTORINFO * _pActorInfo, const OBJECTINFO * _pObjectInfo)
{
	CGameObject* pInstance = new CRuler_Body;
	pInstance->Set_Prefab(_pObjectInfo);
	pInstance->Set_ActorInfo(_pActorInfo);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;

}

HRESULT CRuler_Body::Ready_GameObject()
{
	if (FAILED(CEnemy::Ready_GameObject()))
		return E_FAIL;
	const OBJECTINFO* pPrefab = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Ruler_Sword");
	for (int i = 0; i < 8; ++i)
	{
		INFO tInfo = m_tInfo;
		tInfo.fAngle = i * 45.f;
		const ACTORINFO tActorInfo{ tInfo,false,L"Ruler_Sword",L"" };

		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pPrefab->eObjId, CRuler_Sword::Create(&tActorInfo, pPrefab, this));

	}

	m_vecBodyCollision.resize(1);
	m_vecBodyCollision[0].eId = COLLISION::C_SPHERE;
	return S_OK;
}

int CRuler_Body::Update_GameObject()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;
	if (!UpdateActive())
		return OBJ_NOEVENT;
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


	UpdateAnimation();
	UpdateBodyCollision();
	UpdateAttackCollision();

	return OBJ_NOEVENT;
}

void CRuler_Body::Render_GameObject()
{
	CEnemy::Render_GameObject();
	RenderCollision();
}

void CRuler_Body::UpdateBodyCollision()
{
	float fSize = m_tInfo.vSize.x;
	float fReduceSize = 0.8f;

	RECT rect = m_pObjectInfo->tRect;

	_vec2 v2Radius = { (float)((rect.right - rect.left) * 0.5f), (float)((rect.bottom - rect.top) * 0.5f) };
	v2Radius *= fSize;
	m_vecBodyCollision[0].tFRect =
	{
		(float)(m_tInfo.vPos.x - v2Radius.x * fReduceSize * m_tInfo.vSize.x),
		(float)(m_tInfo.vPos.y - v2Radius.y * fReduceSize * m_tInfo.vSize.y),
		(float)(m_tInfo.vPos.x + v2Radius.x * fReduceSize * m_tInfo.vSize.x),
		(float)(m_tInfo.vPos.y + v2Radius.y * fReduceSize * m_tInfo.vSize.y)
	};
	
}
