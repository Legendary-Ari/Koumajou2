#pragma once
class CScroll_Manager
{
public:
	enum ID {SCROLL_X, SCROLL_Y};
public:
	CScroll_Manager();
	~CScroll_Manager();
public:
	static const D3DXVECTOR3& Get_Scroll() { return m_vCurScroll; }
	static void Set_Scroll(const D3DXVECTOR3& vScroll ); 
	static void Set_Scroll(ID eScrollID, float fScroll);
	static void Add_Scroll(const D3DXVECTOR3& vScroll);
	static void Add_Scroll(ID eScrollID, float fScroll);
	static void Force_Set_Scroll(const D3DXVECTOR3& vScroll);
	static void UpdateScroll();
	static void SetMapSize(int _vMapX, int _vMapY);
	static int GetMapSizeX();
	static int GetMapSizeY();
	static void ScrollLock();
private:
	static D3DXVECTOR3 m_vTargetScroll; 
	static D3DXVECTOR3 m_vCurScroll;
	static int m_iMapSizeX;
	static int m_iMapSizeY;
};

