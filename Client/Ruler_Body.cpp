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
}
