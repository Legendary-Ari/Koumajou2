#include "stdafx.h"
#include "BackGround.h"


CBackGround::CBackGround()
{
}


CBackGround::~CBackGround()
{
	Release_GameObject();
}

CGameObject * CBackGround::Create(const OBJECTINFO * _pObjectInfo)
{
	CGameObject* pInstance = new CBackGround;
	pInstance->Set_Prefab(_pObjectInfo);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CBackGround::Ready_GameObject()
{
	m_tInfo.vPos = {(float)(CLIENTCX>>1), (float)(CLIENTCY>>1), 0.f};
	m_tInfo.vSize = { 1.3f,1.3f,1.0f };
	m_bOverlapable = false;
	m_bBlockable = false;
	return S_OK;
}

void CBackGround::InitUpdate_GameObject()
{
}

int CBackGround::Update_GameObject()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	return OBJ_NOEVENT;
}

void CBackGround::Late_Update_GameObject()
{
}

void CBackGround::Render_GameObject()
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

void CBackGround::Release_GameObject()
{
}

const RENDERID::ID & CBackGround::Get_RenderId() const
{
	return RENDERID::BACKGROUND; 
}
