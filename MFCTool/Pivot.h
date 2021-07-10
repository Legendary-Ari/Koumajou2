#pragma once
class CPivot
{
public:
	enum ID{X,Y,XY};
public:
	CPivot();
	~CPivot();
public:
	void Add_PosX(float _fX);
	void Add_PosY(float _fY);
	void Add_Pos(POINT _v2Pos);
	void Set_Pos(POINT _v2Pos);
	int IsPtInRect(POINT _point);
	void Render();
	void SetStartMove(const POINT& _startPoint);
	inline const POINT& Get_Pos() const { return m_tPos; }

private:
	void UpdateRect();
private:
	RECT m_tRect[XY];
	POINT m_tPos;
};

