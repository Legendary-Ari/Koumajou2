#include "stdafx.h"
#include "Texture_Manager.h"
#include "Single_Texture.h"
#include "Multi_Texture.h"
IMPLEMENT_SINGLETON(CTexture_Manager)
CTexture_Manager::CTexture_Manager()
{
}


CTexture_Manager::~CTexture_Manager()
{
	Release_Texture_Manager(); 
}

HRESULT CTexture_Manager::Insert_Texture_Manager(TEX_ID eID, const vector<RECT>& vecRect, const wstring & wstrFilePath, const wstring & wstrObjectKey, const wstring & wstrStateKey)
{
	TEXINFO* pTexInfo = nullptr;
	map<wstring, CTexture*>::iterator iter_Texture_find = m_mapTexture.find(wstrObjectKey);
	if (iter_Texture_find != m_mapTexture.end())
		return S_OK;
	
	auto& iter_LoadedTex_find = m_mapLoadedTexInfo.find(wstrFilePath);
	if (iter_LoadedTex_find == m_mapLoadedTexInfo.end())
	{
		pTexInfo = new TEXINFO;
		if (FAILED(D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &pTexInfo->tImageInfo)))
		{
			Safe_Delete(pTexInfo);
			ERR_MSG(L"이미지 삽입 실패 Texture_Manager");
			return E_NOTIMPL;
		}

		if (FAILED(D3DXCreateTextureFromFileEx(CGraphic_Device::Get_Instance()->Get_Device(),
			wstrFilePath.c_str(),
			pTexInfo->tImageInfo.Width, // 그림 가로
			pTexInfo->tImageInfo.Height, // 그림 세로 
			pTexInfo->tImageInfo.MipLevels, // 밉레벨 
											// 이미지 밉래벨은 있을수도 있고 없을 수도 있다. 
			0,
			pTexInfo->tImageInfo.Format,
			D3DPOOL_MANAGED, // 우리는 짬짜면을 쓰겠다. ㅇㅋ? 
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			0,
			nullptr,
			nullptr,
			&pTexInfo->pTexture)))
		{
			Safe_Delete(pTexInfo);
			ERR_MSG(L"이미지 삽입 실패 Texture_Manager");
			return E_NOTIMPL;
		}
	}
	else
	{
		pTexInfo = iter_LoadedTex_find->second;
	}
	CTexture* pTexture = nullptr;
	switch (eID)
	{
	case CTexture_Manager::SINGLE_TEX:
		pTexture = new CSingle_Texture;
		break;
	case CTexture_Manager::MULTI_TEX:
		pTexture = new CMulti_Texture;
		break;
	default:
		break;
	}
	if (FAILED(pTexture->Insert_Texture(vecRect, pTexInfo, wstrStateKey)))
	{
		pTexInfo->pTexture->Release();
		Safe_Delete(pTexInfo);
		Safe_Delete(pTexture);
		ERR_MSG(L"이미지 삽입 실패 Texture::Insert");
		return E_NOTIMPL;
	}
	m_mapLoadedTexInfo.emplace(wstrFilePath, pTexInfo);
	m_mapTexture.emplace(wstrObjectKey, pTexture);
	return S_OK;
	//map<wstring, CTexture*>::iterator iter_find = m_mapTexture.find(wstrObjectKey);
	//CTexture* pTexture = nullptr;
	//if (m_mapTexture.end() == iter_find)
	//{
	//	
	//	switch (eID)
	//	{
	//	case CTexture_Manager::SINGLE_TEX:
	//		pTexture = new CSingle_Texture; 
	//		break;
	//	case CTexture_Manager::MULTI_TEX:
	//		pTexture = new CMulti_Texture; 
	//		break;
	//	default:
	//		break;
	//	}
	//	if (FAILED(pTexture->Insert_Texture(vecRect, wstrFilePath, wstrStateKey)))
	//		goto ERR; 
	//	
	//	m_mapTexture.emplace(wstrObjectKey, pTexture);
	//}
	//else if (MULTI_TEX == eID)
	//{
	//	if (FAILED(m_mapTexture[wstrObjectKey]->Insert_Texture(vecRect, wstrFilePath, wstrStateKey)))
	//		goto ERR; 

	//}
	//return S_OK; 
//ERR:		
//	ERR_MSG(L"이미지 삽입 실패 Texture_Manager");
//	return E_NOTIMPL;

}

HRESULT CTexture_Manager::Insert_TexInfo(const wstring & wstrFilePath)
{
	TEXINFO* pTexInfo;
	auto& iter_LoadedTex_find = m_mapLoadedTexInfo.find(wstrFilePath);
	if (iter_LoadedTex_find != m_mapLoadedTexInfo.end())
		return S_OK;
	
	pTexInfo = new TEXINFO;
	if (FAILED(D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &pTexInfo->tImageInfo)))
	{
		Safe_Delete(pTexInfo);
		ERR_MSG(L"이미지 삽입 실패 Texture_Manager::Insert_TexInfo");
		return E_NOTIMPL;
	}

	if (FAILED(D3DXCreateTextureFromFileEx(CGraphic_Device::Get_Instance()->Get_Device(),
		wstrFilePath.c_str(),
		pTexInfo->tImageInfo.Width, // 그림 가로
		pTexInfo->tImageInfo.Height, // 그림 세로 
		pTexInfo->tImageInfo.MipLevels, // 밉레벨 
										// 이미지 밉래벨은 있을수도 있고 없을 수도 있다. 
		0,
		pTexInfo->tImageInfo.Format,
		D3DPOOL_MANAGED, // 우리는 짬짜면을 쓰겠다. ㅇㅋ? 
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		nullptr,
		nullptr,
		&pTexInfo->pTexture)))
	{
		Safe_Delete(pTexInfo);
		ERR_MSG(L"이미지 삽입 실패 Texture_Manager::Insert_TexInfo");
		return E_NOTIMPL;
	}
	m_mapLoadedTexInfo.emplace(wstrFilePath, pTexInfo);
	return S_OK;
}

const TEXINFO * CTexture_Manager::Get_TexInfo(const wstring & wstrObjectKey, const wstring & wstrStateKey, const DWORD dwIndex)
{
	map<wstring, CTexture*>::iterator iter_find = m_mapTexture.find(wstrObjectKey); 
	if(m_mapTexture.end() == iter_find)
		return nullptr;

	return iter_find->second->Get_TexInfo();
	//return m_mapTexture[wstrObjectKey]->Get_TexInfo(wstrStateKey, dwIndex); 
}

void CTexture_Manager::DrawPic(CString Objectkey, const RECT & _rect, int Index, const CStatic & PictureBox, CString StateKey)
{
	CGraphic_Device::Get_Instance()->Render_Begin();
	if (_rect.bottom == 0)
		Index = -1;
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()
		->Get_TexInfo(Objectkey.GetString(), StateKey.GetString(), (Index == -1) ? 0 : Index);
	if (nullptr == pTexInfo)
		return;
	float fCenterX;
	float fCenterY;
	if (Index != -1)
	{
		fCenterX = float(((_rect.right - _rect.left)*0.5f));
		fCenterY = float(((_rect.bottom - _rect.top) * 0.5f));
	}
	else
	{
		fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	}
	D3DXMATRIX matScale, matTrans, matWorld;
	if (Index != -1)
	{
		if (fCenterX > fCenterY)
			D3DXMatrixScaling(&matScale, (float)WINCX / (_rect.right - _rect.left), (float)WINCY / (_rect.right - _rect.left), 0.f);
		else
			D3DXMatrixScaling(&matScale, WINCX / (_rect.bottom - _rect.top), WINCY / (_rect.bottom - _rect.top), 0.f);
	}
	else
		D3DXMatrixScaling(&matScale, float(WINCX) / (pTexInfo->tImageInfo.Width), float(WINCY) / (pTexInfo->tImageInfo.Height), 0.f);
	D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, (Index == -1) ? nullptr : &_rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	CGraphic_Device::Get_Instance()->Render_End(PictureBox.m_hWnd);
}


void CTexture_Manager::Release_Texture_Manager()
{
	for (pair<const wstring, CTexture*>& rPair : m_mapTexture)
		Safe_Delete(rPair.second);
	m_mapTexture.clear(); 
	for (auto& rPair : m_mapLoadedTexInfo)
	{
		rPair.second->pTexture->Release();
		Safe_Delete(rPair.second);
	}
		
	m_mapLoadedTexInfo.clear();
}

void CTexture_Manager::Release_Texture(const wstring & wstrObjectPath)
{
	auto& iter_find = m_mapLoadedTexInfo.find(wstrObjectPath);
	if (m_mapLoadedTexInfo.end() == iter_find)
		return;
	iter_find->second->pTexture->Release();
	m_mapLoadedTexInfo.erase(iter_find);
}
