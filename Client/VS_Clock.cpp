#include "stdafx.h"
#include "VS_Clock.h"
#include "LunaDial.h"

CVS_Clock::CVS_Clock()
{
}


CVS_Clock::~CVS_Clock()
{
}

CGameObject * CVS_Clock::Create()
{
	CGameObject* pInstance = new CVS_Clock;
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CVS_Clock::Ready_GameObject()
{
	m_pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"IconClock");
	if (!m_pObjectInfo)
		return E_FAIL;
	ZeroMemory(&m_tInfo, sizeof(INFO));
	return S_OK;
}

void CVS_Clock::InitUpdate_GameObject()
{
}

int CVS_Clock::Update_GameObject()
{
	return 0;
}

void CVS_Clock::Late_Update_GameObject()
{
}
void CVS_Clock::Release_GameObject()
{
}

void CVS_Clock::Use(const INFO & tInfo)
{
	if (m_bCoolDown)
		return;
	const ANIMATION* pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"SakuyaTime");
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::EFFECT, CLunaDial::Create(pAnim,this));
	m_bCoolDown = true;
}

void CVS_Clock::ReleaseCoolDown()
{
	m_bCoolDown = false;
}
