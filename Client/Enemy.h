#pragma once
#include "GameObject.h"
class CEnemy abstract:
	public CGameObject
{
public:
	explicit	CEnemy();
	virtual		~CEnemy();
public:

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

};

