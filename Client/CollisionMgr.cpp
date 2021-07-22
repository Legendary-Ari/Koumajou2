#include "stdafx.h"
#include "CollisionMgr.h"
#include "GameObject.h"
#include "Player.h"


CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::Collision( list<CGameObject*>& _Dst, list<CGameObject*>& _Src )
{
	_vec3 vColPos;
	for ( auto& pDst : _Dst )
	{
		for ( auto& pSrc : _Src )
		{
			if ( IsObj_Overlapped( pDst->Get_BodyCollision(), pSrc->Get_AttackCollision(), &vColPos) )
			{
				if (pDst->Get_IsOverlapable())
					pDst->OnOverlaped(pSrc, vColPos);

			}
			if (IsObj_Overlapped(pDst->Get_AttackCollision(), pSrc->Get_BodyCollision(), &vColPos))
			{

				if (pSrc->Get_IsOverlapable())
					pSrc->OnOverlaped(pDst, vColPos);
			}
		}
	}
}

void CCollisionMgr::Collision_Ex( list<CGameObject*>& _Dst, list<CGameObject*>& _Src )
{
	float fX = 0.f, fY = 0.f;
	for ( auto& pDst : _Dst )
	{
		for ( auto& pSrc : _Src )
		{
			if (!(pSrc->Get_IsBlockable() && pDst->Get_IsBlockable()))
				continue;
			if ( IsObj_OverlappedEx(pDst->Get_BodyCollision(), pSrc->Get_BodyCollision(),&fX,&fY) )
			{
				if ( fX < fY )
				{
					if ( pDst->Get_Info().vPos.x < pSrc->Get_Info().vPos.x )
					{
						pSrc->Add_PosX( fX );
						pSrc->OnBlocked(pDst, DIRECTION::W);
						pDst->OnBlocked( pSrc, DIRECTION::E );
						continue;
					}
					else
					{
						pSrc->Add_PosX( -fX );
							pSrc->OnBlocked( pDst, DIRECTION::E );
							pDst->OnBlocked( pSrc, DIRECTION::W );
						continue;
					}
				}
				else
				{
					if ( pDst->Get_Info().vPos.y < pSrc->Get_Info().vPos.y)
					{
						pSrc->Add_PosY( fY );
							pSrc->OnBlocked( pDst, DIRECTION::N );
							pDst->OnBlocked( pSrc, DIRECTION::S );
						continue;
					}
					else
					{
						pSrc->Add_PosY( -fY );
							pSrc->OnBlocked( pDst, DIRECTION::S );
							pDst->OnBlocked( pSrc, DIRECTION::N );
						continue;
					}
				}
				
			}
		}
	}
}
void CCollisionMgr::Collision_BackGroundEx(list<CGameObject*>& _Src, bool _bForceEx)
{
	auto& listBackGround = CGameObject_Manager::Get_Instance()->Get_BackGroundObject();
	float fX = 0.f, fY = 0.f;
	for (auto& pDst : listBackGround)
	{
		for (auto& pSrc : _Src)
		{
			if (IsObj_OverlappedEx(pDst->Get_TileCollision(), pSrc->Get_TileCollision(), &fX, &fY))
			{
				if (fX < fY)
				{
					if (pDst->Get_Info().vPos.x < pSrc->Get_Info().vPos.x)
					{
						pSrc->Add_PosX(fX);
						pSrc->OnBlockedTile(pDst, DIRECTION::W);
						pDst->OnBlockedTile(pSrc, DIRECTION::E);
					}
					else
					{
						pSrc->Add_PosX(-fX);
						pSrc->OnBlockedTile(pDst, DIRECTION::E);
						pDst->OnBlockedTile(pSrc, DIRECTION::W);
					}
				}
				else
				{
					if (pDst->Get_Info().vPos.y < pSrc->Get_Info().vPos.y)
					{
						pSrc->Add_PosY(fY);
						pSrc->OnBlockedTile(pDst, DIRECTION::N);
						pDst->OnBlockedTile(pSrc, DIRECTION::S);
					}
					else
					{
						pSrc->Add_PosY(-fY);
						pSrc->OnBlockedTile(pDst, DIRECTION::S);
						pDst->OnBlockedTile(pSrc, DIRECTION::N);
					}
				}

			}
		}
	}
}

void CCollisionMgr::PlayerCheckBottom()
{
	auto& listBackGround = CGameObject_Manager::Get_Instance()->Get_BackGroundObject();
	float fX = 0.f, fY = 0.f;
	CGameObject* pSrc = CGameObject_Manager::Get_Instance()->Get_Player();
	if (pSrc == nullptr)
		return;
	for (auto& pDst : listBackGround)
	{
		vector<COLLISION> vecCol = pSrc->Get_TileCollision();
		if (vecCol.empty())
			continue;
		else
		{
			for (auto& Col : vecCol)
			{
				Col.tFRect.bottom += 0.9f;
			}
		}
		if (IsObj_OverlappedEx(pDst->Get_TileCollision(), vecCol, &fX, &fY))
		{
			if (fX > fY)
			{
				if (pDst->Get_Info().vPos.y > pSrc->Get_Info().vPos.y)
				{
					pSrc->Set_OnGround(true);
					return;
				}
			}
		}
	}
	pSrc->Set_OnGround(false);
}

bool CCollisionMgr::IsObj_Overlapped(const vector<COLLISION>& _Dst, const vector<COLLISION>& _Src, _vec3* vHitPos)
{
	RECT rc = {};
	if (_Dst.empty() || _Src.empty())
		return false;
	float fX, fY;
	for ( auto& tDst : _Dst )
	{
		for ( auto& tSrc : _Src )
		{
			if (tDst.eId == COLLISION::C_RECT, tSrc.eId == COLLISION::C_RECT)
			{
				if (Check_Rect(tDst.tFRect, tSrc.tFRect, &fX, &fY))
				{
					_vec2 v2Center = CMyMath::Get_FRect_Center(tDst.tFRect);
					vHitPos->x = v2Center.x - fX;
					vHitPos->y = v2Center.y - fY;
					vHitPos->z = 0;
					return true;
				}
			}
			else if (tDst.eId == COLLISION::C_RECT, tSrc.eId == COLLISION::C_SPHERE)
			{
				if (Check_RectAndSphere(tDst.tFRect, tSrc.tFRect, &fX, &fY))
				{
					_vec2 v2Center = CMyMath::Get_FRect_Center(tDst.tFRect);
					vHitPos->x = v2Center.x - fX;
					vHitPos->y = v2Center.y - fY;
					vHitPos->z = 0;
					return true;
				}
			}
			else if (tDst.eId == COLLISION::C_SPHERE, tSrc.eId == COLLISION::C_RECT)
			{
				if (Check_RectAndSphere(tDst.tFRect, tSrc.tFRect, &fX, &fY))
				{
					_vec2 v2Center = CMyMath::Get_FRect_Center(tDst.tFRect);
					vHitPos->x = v2Center.x - fX;
					vHitPos->y = v2Center.y - fY;
					vHitPos->z = 0;
					return true;
				}
			}
			else if (tDst.eId == COLLISION::C_SPHERE, tSrc.eId == COLLISION::C_SPHERE)
			{
				if (Check_Sphere(tDst.tFRect, tSrc.tFRect, &fX, &fY))
				{
					_vec2 v2Center = CMyMath::Get_FRect_Center(tDst.tFRect);
					vHitPos->x = v2Center.x - fX;
					vHitPos->y = v2Center.y - fY;
					vHitPos->z = 0;
					return true;
				}
			}
		}
	}
	return false;
}

bool CCollisionMgr::IsObj_OverlappedEx( const vector<COLLISION>& _Dst, const vector<COLLISION>& _Src, float* _fX, float* _fY )
{
	if (_Dst.empty() || _Src.empty())
		return false;
	RECT rc = {};
	for ( auto& tDst : _Dst )
	{
		for ( auto& tSrc : _Src )
		{
			if (tDst.eId == COLLISION::C_RECT, tSrc.eId == COLLISION::C_RECT)
			{
				if (Check_Rect(tDst.tFRect, tSrc.tFRect, _fX, _fY))
				{
					return true;
				}
			}
			else if (tDst.eId == COLLISION::C_RECT, tSrc.eId == COLLISION::C_SPHERE)
			{
				if (Check_RectAndSphere(tDst.tFRect, tSrc.tFRect, _fX, _fY))
				{
					return true;
				}
			}
			else if (tDst.eId == COLLISION::C_SPHERE, tSrc.eId == COLLISION::C_RECT)
			{
				if (Check_RectAndSphere(tSrc.tFRect, tDst.tFRect, _fX, _fY))
				{
					*_fX = -(*_fX);
					*_fY = -(*_fY);
					return true;
				}
			}
			else if (tDst.eId == COLLISION::C_SPHERE, tSrc.eId == COLLISION::C_SPHERE)
			{
				if (Check_Sphere(tDst.tFRect, tSrc.tFRect, _fX, _fY))
				{
					return true;
				}
			}
		}
	}
	return false;
}

//Use This After Ex
bool CCollisionMgr::IsWalking(const CPlayer* _pSrc)
{
	auto& listBackGround = CGameObject_Manager::Get_Instance()->Get_BackGroundObject();
	float fX = 0.f, fY = 0.f;
	_vec3 vColPos;
	for (auto& pDst : listBackGround)
	{
		if (!pDst->Get_IsOverlapable())
			continue;
		vector<COLLISION> vecPlusBottom(_pSrc->Get_BodyCollision());
		for (auto& tCol : vecPlusBottom)
			tCol.tFRect.bottom += 5;
		if (IsObj_Overlapped(pDst->Get_BodyCollision(), _pSrc->Get_BodyCollision(), &vColPos))
		{
			return true;
		}
	}

	return false;
}

bool CCollisionMgr::Check_Sphere(const FRECT& _Dst, const FRECT& _Src, float* _x, float* _y)
{
	if (!_Dst.bottom || !_Src.bottom)
		return false;
	D3DXVECTOR3 vDstCenter{ (_Dst.left + _Dst.right) * 0.5f,(_Dst.top + _Dst.bottom) * 0.5f,0.f };
	D3DXVECTOR3 vSrcCenter{ (_Src.left + _Src.right) * 0.5f,(_Src.top + _Src.bottom) * 0.5f,0.f };

	D3DXVECTOR3 vSrcToDst = vDstCenter - vSrcCenter;
	float fDia = D3DXVec3Length(&vSrcToDst);

	float fDis = (float)( vDstCenter.x - _Dst.left + vSrcCenter.x - _Src.left );

	if (fDia <= fDis)
	{
		D3DXVec3Normalize(&vSrcToDst, &vSrcToDst);
		_vec3 vCollision = vSrcToDst * -(fDis - fDia);
		*_x = vCollision.x;
		*_y = vCollision.y;
		return true;
	}
		

	return false;
}

bool CCollisionMgr::Check_Rect(const FRECT& _Dst,const FRECT& _Src, float* _x, float* _y )
{
	if (!_Dst.bottom || !_Src.bottom)
		return false;
	FRECT rc = {};

	if (_Dst.left > _Src.right) return false;
	if (_Dst.right < _Src.left) return false;
	if (_Dst.top > _Src.bottom) return false;
	if (_Dst.bottom < _Src.top) return false;

	rc.left = max(_Dst.left, _Src.left);
	rc.top = max(_Dst.top, _Src.top);
	rc.right = min(_Dst.right, _Src.right);
	rc.bottom = min(_Dst.bottom, _Src.bottom);

	float rcW = float(rc.right - rc.left);
	float rcH = float(rc.bottom - rc.top);
	
	if (rcW > 0 && rcH > 0)
	{
		if (_x != nullptr)
		{
			*_x = rcW;
			*_y = rcH;
		}
		return true;
	}

	return false;
}

bool CCollisionMgr::Check_RectAndSphere(const FRECT& _Rect, const FRECT& _Sphere, float* _x, float* _y)
{
	if (!_Rect.bottom || !_Sphere.bottom)
		return false;
	FRECT tExpandedRect = _Rect;
	_vec2 v2WH = CMyMath::Get_FRect_WH(_Rect);
	tExpandedRect.left -= v2WH.x*0.5f;
	tExpandedRect.right += v2WH.x*0.5f;
	tExpandedRect.top -= v2WH.y*0.5f;
	tExpandedRect.bottom += v2WH.y*0.5f;

	_vec2 v2RectCenter = CMyMath::Get_FRect_Center(_Rect);
	_vec2 v2SphereCenter = CMyMath::Get_FRect_Center(_Sphere);

	if (Check_RectAndPoint(tExpandedRect, v2SphereCenter))
	{
		if (tExpandedRect.left < v2SphereCenter.x &&
			tExpandedRect.right > v2SphereCenter.x )
		{
			if (v2RectCenter.y < v2SphereCenter.y)
			{
				if(_x)
					*_x = _Sphere.right - _Rect.left;
			}
			if(_y)
				*_y = 0;
			
			return true;
		}
		else if (tExpandedRect.top < v2SphereCenter.y &&
				tExpandedRect.bottom > v2SphereCenter.y)
		{
			if (v2RectCenter.y < v2SphereCenter.y)
			{
				if (_y)
					*_y = _Sphere.bottom - _Rect.top;
			}
			if (_x)
				*_x = 0;

			return true;
		}
		else
		{
			_vec2 _v2Point;
			_v2Point = { tExpandedRect.left, tExpandedRect.top };
			float fX, fY;
			if (Check_SphereAndPoint(_Sphere, _v2Point, &fX, &fY))
			{
				if (_x)
					*_x = fX;
				if (_y)
					*_y = fY;

				return true;
			}
			_v2Point = { tExpandedRect.left, tExpandedRect.top };
			if (Check_SphereAndPoint(_Sphere, _v2Point, &fX, &fY))
			{
				if (_x)
					*_x = fX;
				if (_y)
					*_y = fY;

				return true;
			}
			_v2Point = { tExpandedRect.right, tExpandedRect.top };
			if (Check_SphereAndPoint(_Sphere, _v2Point, &fX, &fY))
			{
				if (_x)
					*_x = fX;
				if (_y)
					*_y = fY;

				return true;
			}
			_v2Point = { tExpandedRect.left, tExpandedRect.top };
			if (Check_SphereAndPoint(_Sphere, _v2Point, &fX, &fY))
			{
				if (_x)
					*_x = fX;
				if (_y)
					*_y = fY;

				return true;
			}
		}
	}
	return false;
}

bool CCollisionMgr::Check_SphereAndPoint(const FRECT & _Dst, _vec2 _v2Point, float *_x, float *_y)
{

	float fRadius = CMyMath::Get_FRect_WH(_Dst).x * 0.5f;

	_vec2 v2Center = CMyMath::Get_FRect_Center(_Dst);
	_vec2 v2Diff = _v2Point - v2Center;
	if (fRadius < D3DXVec2Length(&v2Diff))
		return false;
	else
	{
		_vec2 v2DiffNormalized;
		D3DXVec2Normalize(&v2DiffNormalized, &v2Diff);
		_vec2 v2Push = (fRadius - D3DXVec2Length(&v2Diff)) * v2DiffNormalized;
		*_x = v2Push.x;
		*_y = v2Push.y;
		return true;
	}	

}

bool CCollisionMgr::Check_RectAndPoint(const FRECT & _Dst, _vec2 _v2Point)
{
	if (_Dst.left < _v2Point.x &&
		_Dst.right > _v2Point.x &&
		_Dst.top < _v2Point.y &&
		_Dst.bottom > _v2Point.y)
		return true;
	else
		return false;
}

bool CCollisionMgr::Check_Collision(const COLLISION & _Dst, const COLLISION & _Src)
{
	return false;
}

