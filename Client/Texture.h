#pragma once
class CTexture abstract
{
public:
	explicit CTexture();
	virtual ~CTexture();
public:
	virtual HRESULT Insert_Texture(const vector<RECT>& vecRect, const TEXINFO* _pTexInfo,
		const wstring& wstrStateKey = L"" )PURE; 
	virtual const TEXINFO* Get_TexInfo()PURE; 
	virtual RECT Get_Rect(const wstring& wstrStateKey = L"",
		const DWORD dwIndex = 0)PURE;
	virtual void Release_Texture() PURE; 
	
protected:
	const TEXINFO*	m_pTexInfo;
	// ObjectKey, StateKey
	//�̰� ��Ƽ �ؽ����� ���� ���� 
	//map<wstring, CMulti_Texture(map<wstring, vector<TEXINFO*>>)> �����������K 
};

