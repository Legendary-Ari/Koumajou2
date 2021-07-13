#include "stdafx.h"
#include "Scroll_Manager.h"

D3DXVECTOR3 CScroll_Manager::m_vTargetScroll{};
D3DXVECTOR3 CScroll_Manager::m_vCurScroll{};
int CScroll_Manager::m_iMapSizeX = CLIENTCX;
int CScroll_Manager::m_iMapSizeY = CLIENTCY;
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
	ScrollLock();
	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	
	m_vCurScroll = CMyMath::VecInterpTo(m_vCurScroll, m_vTargetScroll, fDeltaTime, 8.f);
	
}

void CScroll_Manager::SetMapSize(int _vMapX, int _vMapY)
{
	m_iMapSizeX = _vMapX;
	m_iMapSizeY = _vMapY;
}

int CScroll_Manager::GetMapSizeX()
{
	return m_iMapSizeX;
}

int CScroll_Manager::GetMapSizeY()
{
	return m_iMapSizeY;
}

void CScroll_Manager::ScrollLock()
{
	if (m_vTargetScroll.x > 0)
		m_vTargetScroll.x = 0;
	if (m_vTargetScroll.x < -m_iMapSizeX + CLIENTCX)
		m_vTargetScroll.x = (float)(-m_iMapSizeX + CLIENTCX);
	if (m_vTargetScroll.y > 0)
		m_vTargetScroll.y = 0;
	if (m_vTargetScroll.y < (-m_iMapSizeY + CLIENTCY))
		m_vTargetScroll.y = (float)(-m_iMapSizeY + CLIENTCY);

}
