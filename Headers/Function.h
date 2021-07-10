#pragma once

template<class T>
void Safe_Delete(T& rObj)
{
	if (rObj)
	{
		delete rObj; 
		rObj = nullptr; 
	}
}

struct RectSort
{
	bool operator()(const RECT& _r1, const RECT& _r2)
	{
		return _r1.left > _r2.left;
	}
};