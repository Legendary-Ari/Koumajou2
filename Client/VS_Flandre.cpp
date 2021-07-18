#include "stdafx.h"
#include "VS_Flandre.h"


CVS_Flandre::CVS_Flandre()
{
}


CVS_Flandre::~CVS_Flandre()
{
}

CGameObject * CVS_Flandre::Create()
{
	CGameObject* pInstance = new CVS_Flandre;
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CVS_Flandre::Ready_GameObject()
{
	m_pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"IconFlandre");
	if (!m_pObjectInfo)
		return E_FAIL;
	ZeroMemory(&m_tInfo, sizeof(INFO));
	return S_OK;
}

void CVS_Flandre::InitUpdate_GameObject()
{
}

int CVS_Flandre::Update_GameObject()
{
	return 0;
}

void CVS_Flandre::Late_Update_GameObject()
{
}

void CVS_Flandre::Release_GameObject()
{
}

void CVS_Flandre::Use(const INFO & tInfo)
{
}
