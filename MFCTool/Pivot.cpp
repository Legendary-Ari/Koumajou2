#include "stdafx.h"
#include "Pivot.h"
#include "MainFrm.h"
#include "MFCToolView.h"

CPivot::CPivot()
{
}


CPivot::~CPivot()
{
}

void CPivot::Add_PosX(float _fX)
{
	m_tPos.x += _fX;
	UpdateRect();
}

void CPivot::Add_PosY(float _fY)
{
	m_tPos.y += _fY;
	UpdateRect();
}

void CPivot::Add_Pos(POINT _v2Pos)
{
	m_tPos.x += _v2Pos.x;
	m_tPos.y += _v2Pos.y;
	UpdateRect();
}

void CPivot::Set_Pos(POINT _v2Pos)
{
	m_tPos = _v2Pos;
	UpdateRect();
}

int CPivot::IsPtInRect(POINT _point)
{
	int by = -1;
	if (PtInRect(&m_tRect[X], _point))
	{
		by += 1;
	}
	if (PtInRect(&m_tRect[Y], _point))
	{
		by += 2;
	}
	return by;
}

void CPivot::Render()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CMFCToolView* pMFCToolView = dynamic_cast<CMFCToolView*>(pMain->m_tMainSplitter.GetPane(0, 1));
	CGraphic_Device::Get_Instance()->Get_Sprite()->End();

	float fScrollX = -pMFCToolView->GetScrollPos(SB_HORZ);
	float fScrollY = -pMFCToolView->GetScrollPos(SB_VERT);

	_vec2 v2LinePos[2];
	v2LinePos[0] = { (float)m_tPos.x, (float)m_tPos.y };
	v2LinePos[1] = { (float)m_tPos.x, (float)m_tRect[Y].top };
	for (int i = 0; i < 2; ++i)
	{
		v2LinePos[i].x += fScrollX;
		v2LinePos[i].y += fScrollY;
	}
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(v2LinePos, 2, D3DCOLOR_ARGB(255, 150, 255, 150));

	v2LinePos[0] = { (float)m_tPos.x + 5.f, (float)m_tRect[Y].top - 5.f };
	v2LinePos[1] = { (float)m_tPos.x, (float)m_tRect[Y].top };
	for (int i = 0; i < 2; ++i)
	{
		v2LinePos[i].x += fScrollX;
		v2LinePos[i].y += fScrollY;
	}
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(v2LinePos, 2, D3DCOLOR_ARGB(255, 150, 255, 150));

	v2LinePos[0] = { (float)m_tPos.x - 5.f, m_tRect[Y].top - 5.f };
	v2LinePos[1] = { (float)m_tPos.x, (float)m_tRect[Y].top };
	for (int i = 0; i < 2; ++i)
	{
		v2LinePos[i].x += fScrollX;
		v2LinePos[i].y += fScrollY;
	}
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(v2LinePos, 2, D3DCOLOR_ARGB(255, 150, 255, 150));

	v2LinePos[0] = { (float)m_tPos.x, (float)m_tPos.y };
	v2LinePos[1] = { (float)m_tRect[X].right, (float)m_tPos.y };
	for (int i = 0; i < 2; ++i)
	{
		v2LinePos[i].x += fScrollX;
		v2LinePos[i].y += fScrollY;
	}
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(v2LinePos, 2, D3DCOLOR_ARGB(255, 255, 150, 150));

	v2LinePos[0] = {(float)m_tRect[X].right - 5.f, m_tPos.y - 5.f };
	v2LinePos[1] = {(float)m_tRect[X].right, (float)m_tPos.y };
	for (int i = 0; i < 2; ++i)
	{
		v2LinePos[i].x += fScrollX;
		v2LinePos[i].y += fScrollY;
	}
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(v2LinePos, 2, D3DCOLOR_ARGB(255, 255, 150, 150));

	v2LinePos[0] = { (float)m_tRect[X].right - 5.f, m_tPos.y + 5.f };
	v2LinePos[1] = { (float)m_tRect[X].right, (float)m_tPos.y };
	for (int i = 0; i < 2; ++i)
	{
		v2LinePos[i].x += fScrollX;
		v2LinePos[i].y += fScrollY;
	}
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(v2LinePos, 2, D3DCOLOR_ARGB(255, 255, 150, 150));

	CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);
}

void CPivot::UpdateRect()
{
	m_tRect[X] =
	{
		(LONG)(m_tPos.x		   ),
		(LONG)(m_tPos.y - 5.f	   ),
		(LONG)(m_tPos.x + 30.f   ),
		(LONG)(m_tPos.y + 5.f	   ),
	};
	m_tRect[Y] =
	{
		(LONG)(m_tPos.x - 5.f),
		(LONG)(m_tPos.y - 30.f),
		(LONG)(m_tPos.x + 5.f),
		(LONG)(m_tPos.y)
	};
}
