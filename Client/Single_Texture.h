#pragma once
#include "Texture.h"
class CSingle_Texture final:
	public CTexture
{
public:
	explicit CSingle_Texture();
	virtual ~CSingle_Texture();
public:
	// CTexture을(를) 통해 상속됨
	virtual HRESULT Insert_Texture(const vector<RECT>& vecRect, const TEXINFO* _pTexInfo, const wstring & wstrStateKey = L"") override;
	virtual const TEXINFO * Get_TexInfo() override;
	virtual RECT Get_Rect(const wstring& wstrStateKey = L"", const DWORD dwIndex = 0)override;
	virtual void Release_Texture() override;
private:
	RECT	m_tRect;
};

