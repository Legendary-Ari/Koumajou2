#include "stdafx.h"
#include "Single_Texture.h"


CSingle_Texture::CSingle_Texture()
	:m_tRect({})
{
}


CSingle_Texture::~CSingle_Texture()
{
	Release_Texture(); 
}

HRESULT CSingle_Texture::Insert_Texture(const vector<RECT>& vecRect, const TEXINFO* _pTexInfo, const wstring & wstrStateKey /*= L""*/)
{
	m_pTexInfo = _pTexInfo;
	m_tRect = vecRect.front();
	return S_OK;
}

const TEXINFO * CSingle_Texture::Get_TexInfo()
{
	return m_pTexInfo;
}

RECT CSingle_Texture::Get_Rect(const wstring & wstrStateKey, const DWORD dwIndex)
{
	return m_tRect;
}

void CSingle_Texture::Release_Texture()
{

}
