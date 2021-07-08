#include "stdafx.h"
#include "BackGround.h"


CBackGround::CBackGround()
{
}


CBackGround::~CBackGround()
{
	Release_GameObject();
}

CGameObject * CBackGround::Create(const ACTORINFO * _pActorInfo, const OBJECTINFO * _pObjectInfo)
{
	CGameObject* pInstance = new CBackGround;
	pInstance->Set_ActorInfo(_pActorInfo);
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
	m_tInfo = m_pActorInfo->tInfo;
	m_tInfo.vSize = { 1.3f,1.3f,1.0f };
	return S_OK;
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
	D3DXMatrixTranslation(&matTrans, (float)(WINCX>>1), (float)(WINCY>>1), 0.f);
	matWorld = matScale *matRotZ* matTrans;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CBackGround::Release_GameObject()
{
}

const RENDERID::ID & CBackGround::Get_RenderId() const
{
	return RENDERID::BACKGROUND; 
}
