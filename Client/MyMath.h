#pragma once

#ifndef __MYMAYH_H__
#define __MYMAYH_H__

namespace MyMath
{
	template <typename T>
	const T& Clamp( const T& t, const T& min, const T& max )
	{
		if ( t < min )
			return min;
		else if ( t > max )
			return max;
		else
			return t;
	}
}

class CMyMath
{
private:
	CMyMath();
	~CMyMath();
public:
	static float FInterpTo(float _fCurrent, float _fTarget, float _fDeltaTime, float _fInterpSpeed);

	static D3DXVECTOR3 VecInterpTo(D3DXVECTOR3 _fCurrent, D3DXVECTOR3 _fTarget, float _fDeltaTime, float _fInterpSpeed);
	static _vec2 Get_FRect_Center(const FRECT& _fRect);
	static _vec2 Get_FRect_WH(const FRECT & _fRect);
	static bool PtInFRect(const FRECT& _fRect, const _vec2& _v2Point);

};


#endif // !__MYMAYH_H__
