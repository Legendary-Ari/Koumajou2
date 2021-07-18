#include "stdafx.h"
#include "VS_Reimu.h"


CVS_Reimu::CVS_Reimu()
{
}


CVS_Reimu::~CVS_Reimu()
{
}

CGameObject * CVS_Reimu::Create()
{
	CGameObject* pInstance = new CVS_Reimu;
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CVS_Reimu::Ready_GameObject()
{
	m_pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"IconReimu");
	if (!m_pObjectInfo)
		return E_FAIL;
	ZeroMemory(&m_tInfo, sizeof(INFO));
	return S_OK;
}

void CVS_Reimu::InitUpdate_GameObject()
{
}

int CVS_Reimu::Update_GameObject()
{
	return 0;
}

void CVS_Reimu::Late_Update_GameObject()
{
}

void CVS_Reimu::Release_GameObject()
{
}

void CVS_Reimu::Use(const INFO & tInfo)
{
}
