#pragma once

#ifndef __COLLISIONMGR_H__
#define __COLLISIONMGR_H__

class CGameObject;
class CPlayer;
class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	static void Collision( list<CGameObject*>& _Dst, list<CGameObject*>& _Src );
	static void Collision_Ex( list<CGameObject*>& _Dst, list<CGameObject*>& _Src );
	static void Collision_BackGroundEx( list<CGameObject*>& _Src, bool _bForceEx = false);
	static void PlayerCheckBottom();

	static bool IsObj_Overlapped(const vector<COLLISION>& _Dst, const vector<COLLISION>& _Src, _vec3* vHitPos);
	static bool IsObj_OverlappedEx( const vector<COLLISION>& _Dst, const vector<COLLISION>& _Src, float* _fX, float* _fY );
	static bool IsWalking(const CPlayer* _pSrc);

	static bool Check_Sphere(const FRECT& _Dst, const FRECT& _Src, float* _x = nullptr, float* _y = nullptr);
	static bool Check_Rect(const FRECT& _Dst, const FRECT& _Src, float* _x = nullptr, float* _y = nullptr);
	static bool Check_RectAndSphere(const FRECT & _Dst, const FRECT & _Src, float* _x = nullptr, float* _y = nullptr);
	static bool Check_SphereAndPoint(const FRECT& _Dst, _vec2 _v2Point, float *_x=nullptr, float *_y=nullptr);
	static bool Check_RectAndPoint(const FRECT& _Dst, _vec2 _v2Point);
	static bool Check_Collision(const COLLISION& _Dst, const COLLISION& _Src);
};

#endif // !__COLLISIONMGR_H__