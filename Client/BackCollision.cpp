#include "stdafx.h"
#include "BackCollision.h"


CBackCollision::CBackCollision()
{
}


CBackCollision::~CBackCollision()
{
}

CGameObject * CBackCollision::Create(const ACTORINFO * _pActorInfo, const OBJECTINFO * _pObjectInfo)
{
	CGameObject* pInstance = new CBackCollision;
	pInstance->Set_Prefab(_pObjectInfo);
	pInstance->Set_ActorInfo(_pActorInfo);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CBackCollision::Ready_GameObject()
{
	m_tInfo = m_pActorInfo->tInfo;
	m_vecBodyCollision.resize(1);
	m_vecBodyCollision[0].eId = COLLISION::C_RECT;

	return S_OK;
}

int CBackCollision::Update_GameObject()
{
	UpdateBodyCollision();
	return OBJ_NOEVENT;
}

void CBackCollision::Late_Update_GameObject()
{
}

void CBackCollision::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_pObjectInfo->wstrObjectImage_ObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXMATRIX matScale, matTrans, matRotZ, matWorld;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();
	//ÃÊ±âÈ­
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_tInfo.fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);
	matWorld = matScale *matRotZ* matTrans;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	RenderCollision();
}

void CBackCollision::Release_GameObject()
{
}

void CBackCollision::UpdateBodyCollision()
{

	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_pObjectInfo->wstrObjectImage_ObjectKey);
	if (nullptr == pTexInfo)
		return;
	_vec2 v2Radius = { (float)(pTexInfo->tImageInfo.Width>>1), (float)(pTexInfo->tImageInfo.Height>>1) };
	
	m_vecBodyCollision[0].tRect =
	{
		(LONG)(m_tInfo.vPos.x - v2Radius.x),
		(LONG)(m_tInfo.vPos.y - v2Radius.y),
		(LONG)(m_tInfo.vPos.x + v2Radius.x),
		(LONG)(m_tInfo.vPos.y + v2Radius.y)
	};
}
