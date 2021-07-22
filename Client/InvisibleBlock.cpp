#include "stdafx.h"
#include "InvisibleBlock.h"


CInvisibleBlock::CInvisibleBlock()
{
}


CInvisibleBlock::~CInvisibleBlock()
{
}

CGameObject * CInvisibleBlock::Create(const INFO& _tInfo)
{
	CGameObject* pInstance = new CInvisibleBlock;
	pInstance->Set_Info(_tInfo);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CInvisibleBlock::Ready_GameObject()
{
	m_vecTileCollision.resize(1);
	m_vecTileCollision[0].eId = COLLISION::C_RECT;
	m_bOverlapable = false;
	UpdateTileCollision();

	return S_OK;
}

void CInvisibleBlock::InitUpdate_GameObject()
{
}

int CInvisibleBlock::Update_GameObject()
{
	UpdateTileCollision();
	return OBJ_NOEVENT;
}

void CInvisibleBlock::Late_Update_GameObject()
{
}

void CInvisibleBlock::Render_GameObject()
{
	RenderCollision();
}

void CInvisibleBlock::Release_GameObject()
{
}

void CInvisibleBlock::UpdateTileCollision()
{
	float fSizeX = m_tInfo.vSize.x;
	float fSizeY = m_tInfo.vSize.y;
	float fReduceSizeLeft = 0.5f;
	float fReduceSizeRight = 0.5f;
	float fReduceSizeUp = 0.5f;
	float fReduceSizeDown = 0.5f;

	_vec2 v2Radius = { 1.f, 1.f };
	v2Radius.x *= fSizeX;
	v2Radius.y *= fSizeY;
	m_vecTileCollision[0].tFRect =
	{
		(float)(m_tInfo.vPos.x - m_tInfo.vSize.x * fReduceSizeLeft),
		(float)(m_tInfo.vPos.y - m_tInfo.vSize.y * fReduceSizeUp),
		(float)(m_tInfo.vPos.x + m_tInfo.vSize.x * fReduceSizeRight),
		(float)(m_tInfo.vPos.y + m_tInfo.vSize.y * fReduceSizeDown)
	};
}
