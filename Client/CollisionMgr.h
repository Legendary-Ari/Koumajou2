#pragma once

#ifndef __COLLISIONMGR_H__
#define __COLLISIONMGR_H__

class CGameObject;
class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	static void Collision( list<CGameObject*>& _Dst, list<CGameObject*>& _Src );
	static void Collision_Ex( list<CGameObject*>& _Dst, list<CGameObject*>& _Src );
	static void Collision_BackGroundEx( list<CGameObject*>& _Src );

	static bool IsObj_Overlapped(const vector<COLLISION>& _Dst, const vector<COLLISION>& _Src );

	static bool IsObj_OverlappedEx( const vector<COLLISION>& _Dst, const vector<COLLISION>& _Src, float* _fX, float* _fY );

	static bool Check_Sphere(const RECT& _Dst, const RECT& _Src, float* _x = nullptr, float* _y = nullptr);
	static bool Check_Rect(const RECT& _Dst, const RECT& _Src, float* _x = nullptr, float* _y = nullptr);
	static bool Check_RectAndSphere(const RECT & _Dst, const RECT & _Src, float* _x = nullptr, float* _y = nullptr);
	static bool Check_Collision(const COLLISION& _Dst, const COLLISION& _Src);
};

#endif // !__COLLISIONMGR_H__