#pragma once
// 0000 0001
#define Ȱ	0x01
// 0000 0010
#define ��	0x02
// 0000 0100
#define ��	0x04
#define OBJ_DESTROYED 1 
#define OBJ_NOEVENT 0 
#ifdef _AFX
#define ERR_MSG(msg)		AfxMessageBox(msg)
#else 
#define ERR_MSG(msg)		MessageBox(nullptr,msg, L"System_Error", MB_OK)
#endif // _AFX



#define DECLARE_SINGLETON(ClassName)					\
public:													\
static ClassName* Get_Instance()						\
{														\
	if (nullptr == m_pInstance)							\
		m_pInstance = new ClassName;					\
	return m_pInstance;									\
}														\
static void Destroy_Instance()							\
{														\
	if (m_pInstance)									\
	{													\
		delete m_pInstance;								\
		m_pInstance = nullptr;							\
	}													\
}														\
private:												\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)					\
ClassName* ClassName::m_pInstance = nullptr;

#define DEBUG_STRING(L, Data, X, Y)																																				\
{TCHAR szBuff[64];																																								\
swprintf_s(szBuff, L, Data);																																					\
D3DXMATRIX mat;																																									\
D3DXMatrixTranslation(&mat, X, Y, 0.f);																																			\
																																												\
CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&mat);																												\
CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), szBuff, lstrlen(szBuff), nullptr, 0, D3DCOLOR_ARGB(255, 255, 0, 0));}	   

#define DEBUG_STRING3(L, Data, Data2, Data3, X, Y)																																\
{TCHAR szBuff[64];																																								\
swprintf_s(szBuff, L, Data, Data2, Data3);																																		\
D3DXMATRIX mat;																																									\
D3DXMatrixTranslation(&mat, X, Y, 0.f);																																			\
																																												\
CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&mat);																												\
CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), szBuff, lstrlen(szBuff), nullptr, 0, D3DCOLOR_ARGB(255, 255, 0, 0));}	   
