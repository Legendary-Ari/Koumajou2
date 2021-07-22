#include "stdafx.h"
#include "LunaDial.h"
#include "Enemy.h"
#include "VS_Clock.h"

CLunaDial::CLunaDial()
{
}


CLunaDial::~CLunaDial()
{
	Release_GameObject();
}

CGameObject * CLunaDial::Create(const ANIMATION * _tAnimationInfo, CVS_Clock* _pClock)
{
	CGameObject* pInstance = new CLunaDial;
	static_cast<CEffect*>(pInstance)->Set_Animation(_tAnimationInfo);
	static_cast<CLunaDial*>(pInstance)->Set_Clock(_pClock);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CLunaDial::Ready_GameObject()
{
	for (auto& pEnemy : CGameObject_Manager::Get_Instance()->Get_EnemyList())
	{
		static_cast<CEnemy*>(pEnemy)->Set_TimeStop(true);
	}
	m_tInfo.vSize = { 1.0f,1.0f,0.f };
	m_tInfo.fAngle = 0.f;
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::VSKILL);
	{
		int i = rand() % 4;
		if (i == 0)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"sak_A027.wav", CSoundMgr::VSKILL);
		}
		else if (i == 1)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"sak_A029.wav", CSoundMgr::VSKILL);
		}
		else if (i == 2)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"sak_A031.wav", CSoundMgr::VSKILL);
		}
		else if (i == 3)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"sak_A031_B.wav", CSoundMgr::VSKILL);
		}
	}
	
	return S_OK;
}

int CLunaDial::Update_GameObject()
{

	if (m_bDestroyed)
		return OBJ_DESTROYED;

	m_fTimeStack += CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_pAnimationInfo->fPlay_Speed <= m_fTimeStack)
	{
		m_fTimeStack = 0.f;
		++m_uiAnimationFrame;

		if (m_pAnimationInfo->vecRect.size() <= m_uiAnimationFrame)
		{
			m_uiAnimationFrame = 0;
			m_pClock->ReleaseCoolDown();
			return OBJ_DESTROYED;
		}

	}

	m_tInfo.vPos = CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info().vPos;
	m_tInfo.vPos.y -= 50.f;

	return OBJ_NOEVENT;
}

void CLunaDial::InitUpdate_GameObject()
{

}

void CLunaDial::Late_Update_GameObject()
{
}

void CLunaDial::Release_GameObject()
{
	for (auto& pEnemy : CGameObject_Manager::Get_Instance()->Get_EnemyList())
	{
		static_cast<CEnemy*>(pEnemy)->Set_TimeStop(false);
	}
}

void CLunaDial::Set_Clock(CVS_Clock * _pClock)
{
	m_pClock = _pClock;
}
