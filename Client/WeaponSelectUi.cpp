#include "stdafx.h"
#include "WeaponSelectUi.h"


CWeaponSelectUi::CWeaponSelectUi()
{
}


CWeaponSelectUi::~CWeaponSelectUi()
{
}

CGameObject * CWeaponSelectUi::Create(const OBJECTINFO * _pObjectInfo)
{
	CGameObject* pInstance = new CWeaponSelectUi;
	pInstance->Set_Prefab(_pObjectInfo);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Delete(pInstance);
		return pInstance;
	}
	return pInstance;
}

HRESULT CWeaponSelectUi::Ready_GameObject()
{
	m_tInfo.vPos = { 320.f, 240.f, 0.f };
	m_tInfo.vDir = { 0.f, -1.f, 0.f };
	m_tInfo.vSize = { 0.6f,0.6f,0.f };
	return S_OK;
}

void CWeaponSelectUi::InitUpdate_GameObject()
{
}

int CWeaponSelectUi::Update_GameObject()
{
	return 0;
}

void CWeaponSelectUi::Late_Update_GameObject()
{
}

void CWeaponSelectUi::Render_GameObject()
{

	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_pObjectInfo->wstrObjectImage_ObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	matWorld = matScale * matTrans;
	const RECT& rect = m_pObjectInfo->tRect;
	float 	fCenterX = float(((rect.right - rect.left)*0.5f));
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CWeaponSelectUi::Release_GameObject()
{
}
