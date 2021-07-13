#include "stdafx.h"
#include "MyMath.h"

float CMyMath::FInterpTo( float _fCurrent, float _fTarget, float _fDeltaTime, float _fInterpSpeed )
{
	if ( _fInterpSpeed <= 0.f )
	{
		return _fTarget;
	}

	const float fDist = _fTarget - _fCurrent;

	if ( sqrt( fDist ) < 0.01f )
	{
		return _fTarget;
	}

	const float fDeltaMove = fDist * MyMath::Clamp<float>( _fDeltaTime * _fInterpSpeed, 0.f, 1.f );;

	return _fCurrent + fDeltaMove;
}

D3DXVECTOR3 CMyMath::VecInterpTo(D3DXVECTOR3 _fCurrent, D3DXVECTOR3 _fTarget, float _fDeltaTime, float _fInterpSpeed)
{
	if (_fInterpSpeed <= 0.f)
	{
		return _fTarget;
	}

	const D3DXVECTOR3 fDist = _fTarget - _fCurrent;

	if (D3DXVec3Length(&fDist) < 0.001f)
	{
		return _fTarget;
	}

	const D3DXVECTOR3 fDeltaMove = fDist * MyMath::Clamp<float>(_fDeltaTime * _fInterpSpeed, 0.f, 1.f);;

	return _fCurrent + fDeltaMove;
}

bool CMyMath::PtInFRect(const FRECT & _fRect, const _vec2 & _v2Point)
{
	if (_v2Point.x > _fRect.left &&
		_v2Point.x < _fRect.right &&
		_v2Point.y > _fRect.top &&
		_v2Point.y < _fRect.bottom
		)
		return true;
	else
		return false;
}

_vec2 CMyMath::Get_FRect_Center(const FRECT & _fRect)
{
	return _vec2(
		((_fRect.right + _fRect.left) * 0.5f),
		((_fRect.top + _fRect.bottom) * 0.5f));
}

_vec2 CMyMath::Get_FRect_WH(const FRECT & _fRect)
{
	return _vec2(
		((_fRect.right - _fRect.left) * 0.5f),
		((_fRect.top - _fRect.bottom) * 0.5f));
}

CMyMath::CMyMath()
{
}

CMyMath::~CMyMath()
{
}

