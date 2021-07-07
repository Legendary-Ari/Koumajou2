#include "stdafx.h"
#include "Scroll_Manager.h"

D3DXVECTOR3 CScroll_Manager::m_vTargetScroll{};
D3DXVECTOR3 CScroll_Manager::m_vCurScroll{};
CScroll_Manager::CScroll_Manager()
{
}


CScroll_Manager::~CScroll_Manager()
{
}

void CScroll_Manager::Set_Scroll(const D3DXVECTOR3& vScroll)
{
	m_vTargetScroll = vScroll; 
}

void CScroll_Manager::Set_Scroll(ID eScrollID, float fScroll)
{
	switch (eScrollID)
	{
	case CScroll_Manager::SCROLL_X:
		m_vTargetScroll.x = fScroll; 
		break;
	case CScroll_Manager::SCROLL_Y:
		m_vTargetScroll.y = fScroll;
		break;
	default:
		break;
	}
}

void CScroll_Manager::Add_Scroll(const D3DXVECTOR3 & vScroll)
{
	m_vTargetScroll += vScroll;

}

void CScroll_Manager::Add_Scroll(ID eScrollID, float fScroll)
{
	switch (eScrollID)
	{
	case CScroll_Manager::SCROLL_X:
		m_vTargetScroll.x += fScroll;
		break;
	case CScroll_Manager::SCROLL_Y:
		m_vTargetScroll.y += fScroll;
		break;
	default:
		break;
	}
}

void CScroll_Manager::UpdateScroll()
{
	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	
	m_vCurScroll = CMyMath::VecInterpTo(m_vCurScroll, m_vTargetScroll, fDeltaTime, 8.f);

}
