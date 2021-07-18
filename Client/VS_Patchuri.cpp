#include "stdafx.h"
#include "VS_Patchuri.h"


CVS_Patchuri::CVS_Patchuri()
{
}


CVS_Patchuri::~CVS_Patchuri()
{
}

CGameObject * CVS_Patchuri::Create()
{
	CGameObject* pInstance = new CVS_Patchuri;
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CVS_Patchuri::Ready_GameObject()
{
	m_pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"IconPatchuri");
	if (!m_pObjectInfo)
		return E_FAIL;
	ZeroMemory(&m_tInfo, sizeof(INFO));
	return S_OK;
}

void CVS_Patchuri::InitUpdate_GameObject()
{
}

int CVS_Patchuri::Update_GameObject()
{
	return 0;
}

void CVS_Patchuri::Late_Update_GameObject()
{
}

void CVS_Patchuri::Release_GameObject()
{
}

void CVS_Patchuri::Use(const INFO & tInfo)
{
}
