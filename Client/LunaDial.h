#pragma once
#include "Effect.h"
class CLunaDial :
	public CEffect
{
	CLunaDial();
public:
	virtual ~CLunaDial();
public:
	static	CGameObject* Create(const ANIMATION*	_tAnimationInfo);

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Release_GameObject() override;
};

