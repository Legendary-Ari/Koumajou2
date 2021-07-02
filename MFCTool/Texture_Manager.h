#pragma once
class CTexture; 
class CTexture_Manager
{
	DECLARE_SINGLETON(CTexture_Manager)
public:
	enum TEX_ID {SINGLE_TEX, MULTI_TEX};
private:
	CTexture_Manager();
	~CTexture_Manager();
public:
	HRESULT Insert_Texture_Manager(TEX_ID eID, const vector<RECT>& vecRect,
		const wstring & wstrFilePath,
		const wstring& wstrObjectKey,
		const wstring & wstrStateKey = L"");

	HRESULT Insert_TexInfo(const wstring& wstrFilePath);

	 const TEXINFO * Get_TexInfo(const wstring& wstrObjectKey,
		 const wstring & wstrStateKey = L"",
		 const DWORD dwIndex = 0);

	 void Release_Texture_Manager() ;

	 void Release_Texture(const wstring& wstrObjectKey);
private:
	//ObjectKey
	map<wstring, CTexture*> m_mapTexture;
	map<wstring, TEXINFO*> m_mapLoadedTexInfo;
};

