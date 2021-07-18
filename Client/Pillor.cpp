#include "stdafx.h"
#include "Pillor.h"


CPillor::CPillor()
{
}


CPillor::~CPillor()
{
}

CGameObject * CPillor::Create(const ACTORINFO * _pActorInfo, const OBJECTINFO * _pObjectInfo)
{
	{
		CGameObject* pInstance = new CPillor;
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
}

HRESULT CPillor::Ready_GameObject()
{
	m_tInfo = m_pActorInfo->tInfo;
	m_vecTileCollision.resize(1);
	m_vecTileCollision[0].eId = COLLISION::C_RECT;
	m_bOverlapable = false;

	return S_OK;
}

void CPillor::InitUpdate_GameObject()
{
}

int CPillor::Update_GameObject()
{
	UpdateTileCollision();
	return 0;
}

void CPillor::Late_Update_GameObject()
{
}

void CPillor::Render_GameObject()
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
	const RECT& rect = m_pObjectInfo->tRect;
	float 	fCenterX = float(((rect.right - rect.left)*0.5f));
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	RenderCollision();
}

void CPillor::Release_GameObject()
{
}

void CPillor::UpdateTileCollision()
{

	float fSizeX = m_tInfo.vSize.x;
	float fSizeY = m_tInfo.vSize.y;
	float fReduceSizeLeft = 0.0f;
	float fReduceSizeRight = 0.75f;
	float fReduceSizeUp = 0.5f;
	float fReduceSizeDown = 0.5f;

	RECT rect = m_pObjectInfo->tRect;

	_vec2 v2Radius = { (float)((rect.right - rect.left) * 0.5f), (float)((rect.bottom - rect.top) * 0.5f) };
	v2Radius.x *= fSizeX;
	v2Radius.y *= fSizeY;
	m_vecTileCollision[0].tFRect =
	{
		(float)(m_tInfo.vPos.x - v2Radius.x * m_tInfo.vSize.x * fReduceSizeLeft),
		(float)(m_tInfo.vPos.y - v2Radius.y * m_tInfo.vSize.y * fReduceSizeUp),
		(float)(m_tInfo.vPos.x + v2Radius.x * m_tInfo.vSize.x * fReduceSizeRight),
		(float)(m_tInfo.vPos.y + v2Radius.y * m_tInfo.vSize.y * fReduceSizeDown)
	};
}

