#include "stdafx.h"
#include "Terrain.h"
#include "MFCToolView.h"

CTerrain::CTerrain()
{
}


CTerrain::~CTerrain()
{
	Release_Terrain(); 
}

HRESULT CTerrain::Ready_Terrain()
{
	m_vecTile.reserve(TILEX * TILEY);
	float fX = 0.f, fY = 0.f; 
	TILE* pTile = nullptr; 
	for (size_t i = 0 ; i < TILEY ; ++i)
	{
		for (size_t j = 0; j < TILEX; ++j)
		{
			fX = float((j * TILECX) + ((i % 2) * (TILECX >> 1))); 
			fY = float(i * (TILECY >> 1)); 
			pTile = new TILE; 
			pTile->vPos = { fX, fY, 0.f }; 
			pTile->vSize = { 1.f, 1.f, 1.f }; 
			pTile->byDrawID = 8; 
			pTile->byOption = 0; 
			m_vecTile.emplace_back(pTile); 
		}
	}
	return S_OK;
}

void CTerrain::Render_Terrain()
{
	D3DXMATRIX matScale, matTrans;
	int iSize = m_vecTile.size(); 
	TCHAR szIndex[32] = L""; 

	for (int i = 0 ; i < iSize ; ++i)
	{
		swprintf_s(szIndex, L"%d", i);
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"Terrain", L"Tile", m_vecTile[i]->byDrawID); 

		if (nullptr == pTexInfo)
			return; 
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1); 
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, m_vecTile[i]->vSize.x, m_vecTile[i]->vSize.y, 0.f); 
		D3DXMatrixTranslation(&matTrans, m_vecTile[i]->vPos.x - m_pView->GetScrollPos(SB_HORZ), m_vecTile[i]->vPos.y - m_pView->GetScrollPos(SB_VERT), 0.f);
		matScale *= matTrans; 
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matScale); 

		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255)); 

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), szIndex, lstrlen(szIndex), nullptr, 0, D3DCOLOR_ARGB(255, 0, 0, 0)); 

	}
	CGraphic_Device::Get_Instance()->Get_Sprite()->End();
	D3DXVECTOR2 vLinePos[2] = { { 100.f, 100.f },{ 200.f, 200.f } };
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLinePos, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
	CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);

}

void CTerrain::MiniRender_Terrain()
{
	D3DXMATRIX matScale, matTrans, matRatio;
	D3DXMatrixScaling(&matRatio, 0.3f, 0.3f, 0.f); 
	int iSize = m_vecTile.size();
	for (int i = 0; i < iSize; ++i)
	{
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"Terrain", L"Tile", m_vecTile[i]->byDrawID);

		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, m_vecTile[i]->vSize.x, m_vecTile[i]->vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_vecTile[i]->vPos.x - m_pView->GetScrollPos(SB_HORZ), m_vecTile[i]->vPos.y - m_pView->GetScrollPos(SB_VERT), 0.f);
		matScale *= matTrans;
		matScale *= matRatio; 
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matScale);

		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CTerrain::Release_Terrain()
{
	for (TILE*& pTile : m_vecTile)
		Safe_Delete(pTile); 
	m_vecTile.clear(); 
	m_vecTile.shrink_to_fit(); 
}

void CTerrain::TilePicking_Terrain(const D3DXVECTOR3 & vMousePos, const BYTE & byDrawID, const BYTE & byOption)
{
	int iIndex = Get_TileIndex(vMousePos); 

	if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
		return; 

	m_vecTile[iIndex]->byDrawID = byDrawID; 
	m_vecTile[iIndex]->byOption = byOption; 
}

int CTerrain::Get_TileIndex(const D3DXVECTOR3 & vMousePos)
{

	DWORD dwSize = m_vecTile.size(); 
	for (DWORD i = 0 ; i  < dwSize ; ++i)
	{
		if (IsPicking_Terrain(vMousePos, i))
			return i; 
	}
	return -1;
}

bool CTerrain::IsPicking_Terrain(const D3DXVECTOR3 & vMousePos, int iIndex)
{
	// 자 인덱스를 찾아봅시다! 
	// 내가 내적을 이용한 방식을 보여줄게. 

	// 지금 내가 말한대로 한번 구현 해보자. 
	// 꼭지점을 구해라! 
	D3DXVECTOR3 vVertex[4] = 
	{
		{m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY >> 1), 0.f}, 
		{m_vecTile[iIndex]->vPos.x + (TILECX >> 1), m_vecTile[iIndex]->vPos.y, 0.f}, 
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY >> 1), 0.f },
		{ m_vecTile[iIndex]->vPos.x - (TILECX >> 1), m_vecTile[iIndex]->vPos.y, 0.f },
	}; 

	// 각 꼭지점에서 바라보는 방향벡터 구하기! 
	D3DXVECTOR3 vVertexDir[4] =
	{
		vVertex[1] - vVertex[0], 
		vVertex[2] - vVertex[1],
		vVertex[3] - vVertex[2],
		vVertex[0] - vVertex[3],
	};

	// 각 꼭지점을 바라보는 방향벡터에서 법선(이 벡터의 수직인 벡터) 벡터 구하기
	//																 
	// ex ) 양의 엑스축 방향을 바라보는  ㅡ> 가 있다고 가정 할 때 수직인 벡터 ↑ 를 뜻한다.  
	D3DXVECTOR3 vVertexNormal[4] = 
	{
		{-vVertexDir[0].y, vVertexDir[0].x, 0.f },  
		{ -vVertexDir[1].y, vVertexDir[1].x, 0.f },
		{ -vVertexDir[2].y, vVertexDir[2].x, 0.f },
		{ -vVertexDir[3].y, vVertexDir[3].x, 0.f },
	};
	// 각 꼭지점에서 마우스를 바라보는 방향 구하기 .
	D3DXVECTOR3 vMouseDir[4] = 
	{
		vMousePos - vVertex[0],
		vMousePos - vVertex[1],
		vMousePos - vVertex[2],
		vMousePos - vVertex[3] 
	}; 
	for (int i = 0 ; i < 4; ++i)
	{
		if (0 < D3DXVec3Dot(&vMouseDir[i], &vVertexNormal[i]))
			return false; 
	}

	return true;
}
