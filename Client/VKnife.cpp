#include "stdafx.h"
#include "VKnife.h"
#include "Knife.h"

CVKnife::CVKnife()
{
}

CVKnife::~CVKnife()
{
}

CGameObject * CVKnife::Create()
{
	CGameObject* pInstance = new CVKnife;
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CVKnife::Ready_GameObject()
{
	m_pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"IconKnife");
	if (!m_pObjectInfo)
		return E_FAIL;
	ZeroMemory(&m_tInfo, sizeof(INFO));
	return S_OK;
}

void CVKnife::InitUpdate_GameObject()
{

}

int CVKnife::Update_GameObject()
{
	return 0;
}

void CVKnife::Late_Update_GameObject()
{
}


void CVKnife::Release_GameObject()
{
}

void CVKnife::Use(const INFO& tInfo)
{
	
}
