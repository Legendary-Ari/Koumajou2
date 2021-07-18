#include "stdafx.h"
#include "VS_Alice.h"


CVS_Alice::CVS_Alice()
{
}


CVS_Alice::~CVS_Alice()
{
}

CGameObject * CVS_Alice::Create()
{
	CGameObject* pInstance = new CVS_Alice;
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CVS_Alice::Ready_GameObject()
{
	m_pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"IconAlice");
	if (!m_pObjectInfo)
		return E_FAIL;
	ZeroMemory(&m_tInfo, sizeof(INFO));
	return S_OK;
}

void CVS_Alice::InitUpdate_GameObject()
{
}

int CVS_Alice::Update_GameObject()
{
	return 0;
}

void CVS_Alice::Late_Update_GameObject()
{
}

void CVS_Alice::Release_GameObject()
{
}

void CVS_Alice::Use(const INFO & tInfo)
{
}
