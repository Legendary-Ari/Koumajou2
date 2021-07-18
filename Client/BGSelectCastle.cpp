#include "stdafx.h"
#include "BGSelectCastle.h"


CBGSelectCastle::CBGSelectCastle()
{
}


CBGSelectCastle::~CBGSelectCastle()
{
}

CGameObject * CBGSelectCastle::Create(const OBJECTINFO * _pObjectInfo)
{
	CGameObject* pInstance = new CBGSelectCastle;
	pInstance->Set_Prefab(_pObjectInfo);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CBGSelectCastle::Ready_GameObject()
{

	m_tInfo.vPos = { (float)(CLIENTCX >> 1) + ((CScene_Manager::END - CGame_Manager::Get_Instance()->m_eCurStage) * 30.f), (float)(CLIENTCY >> 1), 0.f };
	m_tInfo.vSize = { 0.8f,0.8f,1.0f };
	m_bOverlapable = false;
	m_bBlockable = false;
	return S_OK;
}

void CBGSelectCastle::InitUpdate_GameObject()
{
}

int CBGSelectCastle::Update_GameObject()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	return OBJ_NOEVENT;
}

void CBGSelectCastle::Late_Update_GameObject()
{
}

void CBGSelectCastle::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_pObjectInfo->wstrObjectImage_ObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXMATRIX matScale, matTrans, matRotZ, matWorld;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();
	//ÃÊ±âÈ­
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_tInfo.fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	matWorld = matScale *matRotZ* matTrans;
	float fCenterX;
	float fCenterY;
	RECT rect = m_pObjectInfo->tRect;
	if (rect.bottom == 0)
	{
		fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
	else
	{
		fCenterX = float(((rect.right - rect.left)*0.5f));
		fCenterY = float(((rect.bottom - rect.top) * 0.5f));
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
}

void CBGSelectCastle::Release_GameObject()
{
}
