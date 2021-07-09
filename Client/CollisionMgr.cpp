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

	for ( auto& pDst : _Dst )
	{
		for ( auto& pSrc : _Src )
		{
			if ( IsObj_Overlapped( pDst->Get_BodyCollision(), pSrc->Get_BodyCollision()) )
			{
				if(pDst->Get_IsOverlapable())
					pDst->OnOverlaped(pSrc);
				if (pSrc->Get_IsOverlapable())
					pSrc->OnOverlaped(pDst);
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

void CCollisionMgr::Collision_BackGroundEx( list<CGameObject*>& _Src , bool _bForceEx)
{
	auto& listBackGround = CGameObject_Manager::Get_Instance()->Get_BackGroundObject();
	float fX = 0.f, fY = 0.f;
	for (auto& pDst : listBackGround)
	{
		for (auto& pSrc : _Src)
		{
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(pSrc);

			if (IsObj_OverlappedEx(pDst->Get_BodyCollision(), pPlayer ? pPlayer->Get_TileCollision() : pSrc->Get_BodyCollision(), &fX, &fY))
			{
				if (fX < fY)
				{
					if (pDst->Get_Info().vPos.x < pSrc->Get_Info().vPos.x)
					{
						pSrc->Add_PosX(fX);
						pSrc->OnBlockedTile(pDst, DIRECTION::W);
						pDst->OnBlockedTile(pSrc, DIRECTION::E);
						continue;
					}
					else
					{
						pSrc->Add_PosX(-fX);
						pSrc->OnBlockedTile(pDst, DIRECTION::E);
						pDst->OnBlockedTile(pSrc, DIRECTION::W);
						continue;
					}
				}
				else
				{
					if (pDst->Get_Info().vPos.y < pSrc->Get_Info().vPos.y)
					{
						pSrc->Add_PosY(fY);
						pSrc->OnBlockedTile(pDst, DIRECTION::N);
						pDst->OnBlockedTile(pSrc, DIRECTION::S);
						continue;
					}
					else
					{
						pSrc->Add_PosY(-fY);
						pSrc->OnBlockedTile(pDst, DIRECTION::S);
						pDst->OnBlockedTile(pSrc, DIRECTION::N);
						continue;
					}
				}

			}			
		}
	}
}

bool CCollisionMgr::IsObj_Overlapped(const vector<COLLISION>& _Dst, const vector<COLLISION>& _Src )
{
	RECT rc = {};
	for ( auto& tDst : _Dst )
	{
		for ( auto& tSrc : _Src )
		{
			if (tDst.eId == COLLISION::C_RECT, tSrc.eId == COLLISION::C_RECT)
			{
				if (Check_Rect(tDst.tFRect, tSrc.tFRect))
				{
					return true;
				}
			}
			else if (tDst.eId == COLLISION::C_RECT, tSrc.eId == COLLISION::C_SPHERE)
			{
				if (Check_RectAndSphere(tDst.tFRect, tSrc.tFRect))
				{
					return true;
				}
			}
			else if (tDst.eId == COLLISION::C_SPHERE, tSrc.eId == COLLISION::C_RECT)
			{
				if (Check_RectAndSphere(tSrc.tFRect, tDst.tFRect))
				{
					return true;
				}
			}
			else if (tDst.eId == COLLISION::C_SPHERE, tSrc.eId == COLLISION::C_SPHERE)
			{
				if (Check_Sphere(tDst.tFRect, tSrc.tFRect))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool CCollisionMgr::IsObj_OverlappedEx( const vector<COLLISION>& _Dst, const vector<COLLISION>& _Src, float* _fX, float* _fY )
{
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

	for (auto& pDst : listBackGround)
	{
		if (!pDst->Get_IsOverlapable())
			continue;
		vector<COLLISION> vecPlusBottom(_pSrc->Get_BodyCollision());
		for (auto& tCol : vecPlusBottom)
			tCol.tFRect.bottom += 5;
		if (IsObj_Overlapped(pDst->Get_BodyCollision(), _pSrc->Get_BodyCollision()))
		{
			return true;
		}
	}

	return false;
}

bool CCollisionMgr::Check_Sphere(const FRECT& _Dst, const FRECT& _Src, float* _x, float* _y)
{
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
	FRECT rc = {};

	//float fDstW = _Dst.right - _Dst.left;
	//float fDstH = _Dst.bottom - _Dst.top;
	//float fSrcW = _Src.right - _Src.left;
	//float fSrcH = _Src.bottom - _Src.top;

	if (_Dst.left < _Src.left)
	{
		rc.left = _Src.left;
	}
	else
	{
		rc.left = _Dst.left;
	}
	rc.right = min(_Src.right, _Dst.right);

	if (_Dst.top < _Src.top)
	{
		rc.top = _Src.top;
	}
	else
	{
		rc.top = _Dst.top;
	}
	rc.bottom = min(_Src.bottom, _Dst.bottom);

	float rcW = float(rc.right - rc.left);
	float rcH = (rc.bottom - rc.top);
	
	if (rcW > 0 && rcH > 0)
	{
		if (_x != nullptr)
		{
			*_x = rcW;
			*_y = rcH;
			return true;
		}

	}

	return false;
}

bool CCollisionMgr::Check_RectAndSphere(const FRECT& _Rect, const FRECT& _Sphere, float* _x, float* _y)
{
	D3DXVECTOR3 vDot[4];
	D3DXVECTOR3 vSphereCenter{(_Sphere.left + _Sphere.right) * 0.5f, (_Sphere.top + _Sphere.bottom) * 0.5f , 0.f};
	float	fSphereRadiusX2 = float(_Sphere.right - _Sphere.left);

	vDot[0] = { (float)(_Rect.left), (float)(_Rect.top), 0.f };
	vDot[1] = { (float)(_Rect.left), (float)(_Rect.bottom), 0.f };
	vDot[2] = { (float)(_Rect.right),(float)( _Rect.bottom), 0.f };
	vDot[3] = { (float)(_Rect.right),(float)( _Rect.top), 0.f };

	for (int i = 0; i < 4; ++i)
	{
		_vec3 vDiff = vDot[i] - vSphereCenter;
		if (fSphereRadiusX2 >= D3DXVec3Length(&vDiff))
		{
			_vec3 vNoramlizedDiff;
			D3DXVec3Normalize(&vNoramlizedDiff, &vDiff);
			_vec3 vCollision = (vNoramlizedDiff * fSphereRadiusX2 * 0.5f) - vDiff;
			*_x = -vCollision.x;
			*_y = -vCollision.y;
			return true;
		}
	}


	return false;
}

bool CCollisionMgr::Check_Collision(const COLLISION & _Dst, const COLLISION & _Src)
{
	return false;
}

