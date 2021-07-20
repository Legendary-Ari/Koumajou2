#include "stdafx.h"
#include "VSkill.h"
#include "Player.h"

CVSkill::CVSkill()
{
}


CVSkill::~CVSkill()
{
}

void CVSkill::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_pObjectInfo->wstrObjectImage_ObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

	D3DXMATRIX matScale, matRotZ, matTrans, matOrbit, matWorld;
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_tInfo.fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	D3DXMatrixRotationZ(&matOrbit, D3DXToRadian(m_tInfo.fAngle));
	matWorld = matScale * matRotZ * matTrans * matOrbit;
	const RECT& rect = m_pObjectInfo->tRect;
	float 	fCenterX = float(((rect.right - rect.left)*0.5f));
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

const UINT & CVSkill::Get_Cost() const
{
	return m_uiCost;
}

void CVSkill::Use_Cost()
{
	static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Sub_Chi(m_uiCost);
}
