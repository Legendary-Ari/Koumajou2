#include "stdafx.h"
#include "Scroll_Manager.h"

D3DXVECTOR3 CScroll_Manager::m_vScroll{};
CScroll_Manager::CScroll_Manager()
{
}


CScroll_Manager::~CScroll_Manager()
{
}

void CScroll_Manager::Set_Scroll(const D3DXVECTOR3& vScroll)
{
	m_vScroll += vScroll; 
}

void CScroll_Manager::Set_Scroll(ID eScrollID, float fScroll)
{
	switch (eScrollID)
	{
	case CScroll_Manager::SCROLL_X:
		m_vScroll.x += fScroll; 
		break;
	case CScroll_Manager::SCROLL_Y:
		m_vScroll.y += fScroll;
		break;
	default:
		break;
	}
}
