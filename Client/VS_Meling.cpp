#include "stdafx.h"
#include "VS_Meling.h"
#include "SubMeling.h"

CVS_Meling::CVS_Meling()
{
}


CVS_Meling::~CVS_Meling()
{
}

CGameObject * CVS_Meling::Create()
{
	CGameObject* pInstance = new CVS_Meling;
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CVS_Meling::Ready_GameObject()
{
	m_pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"IconMeiling");
	m_uiCost = 25;
	if (!m_pObjectInfo)
		return E_FAIL;
	ZeroMemory(&m_tInfo, sizeof(INFO));
	return S_OK;
}

void CVS_Meling::InitUpdate_GameObject()
{
}

int CVS_Meling::Update_GameObject()
{
	return 0;
}

void CVS_Meling::Late_Update_GameObject()
{
}

void CVS_Meling::Release_GameObject()
{
}

void CVS_Meling::Use(const INFO & tInfo)
{
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager( OBJECTINFO::PLAYER_BULLET, CSubMeling::Create(tInfo));
	Use_Cost();
}
