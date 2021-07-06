#include "stdafx.h"
#include "Multi_Texture.h"


CMulti_Texture::CMulti_Texture()
{
}


CMulti_Texture::~CMulti_Texture()
{
	Release_Texture(); 
}
HRESULT CMulti_Texture::Insert_Texture(const vector<RECT>& vecRect, const TEXINFO* _pTexInfo, const wstring & wstrStateKey /*= L""*/)
{
	m_pTexInfo = _pTexInfo;
	auto& iter_find = m_mapMultiTex.find(wstrStateKey);
	if (iter_find != m_mapMultiTex.end())
	{
		return E_NOTIMPL;
	}
	m_mapMultiTex.emplace(wstrStateKey, vecRect);
	return S_OK;
}

const TEXINFO * CMulti_Texture::Get_TexInfo()
{
	return m_pTexInfo;
}

RECT CMulti_Texture::Get_Rect(const wstring & wstrStateKey, const DWORD dwIndex)
{
	auto& iter_find = m_mapMultiTex.find(wstrStateKey);
	if (iter_find == m_mapMultiTex.end())
	{
		return RECT();
	}
	if (iter_find->second.size() <= dwIndex)
		return RECT();
	return iter_find->second[dwIndex];
}

void CMulti_Texture::Release_Texture()
{

}
