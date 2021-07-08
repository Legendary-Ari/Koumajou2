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

CMyMath::CMyMath()
{
}

CMyMath::~CMyMath()
{
}

