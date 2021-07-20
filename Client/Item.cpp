#include "stdafx.h"
#include "Item.h"


CItem::CItem()
{
}


CItem::~CItem()
{
}

void CItem::UpdateAnimation()
{
	m_fAnimationCumulatedTime += CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_fAnimationCumulatedTime >= m_vecAnimation[0]->fPlay_Speed)
	{
		++m_uiAnimationFrame;
		if (m_uiAnimationFrame >= m_vecAnimation[0]->vecRect.size())
			m_uiAnimationFrame = 0;
		m_fAnimationCumulatedTime = 0.f;
	}
}

void CItem::Late_Update_GameObject()
{
	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	_vec3 vDiff = CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info().vPos - m_tInfo.vPos;
	if (D3DXVec3Length(&vDiff) < 10.f)
	{
		Give_ThisItem();
	}
}