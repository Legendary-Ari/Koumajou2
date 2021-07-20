#pragma once
#include "GameObject.h"
class CItem abstract:
	public CGameObject
{
public:
	explicit CItem();
	virtual ~CItem();

public:
	virtual void Late_Update_GameObject() override;
	virtual const RENDERID::ID& Get_RenderId() const { return RENDERID::EFFECT; }
public:
	virtual void	Give_ThisItem()PURE;
protected:
	void			UpdateAnimation() override;
};

