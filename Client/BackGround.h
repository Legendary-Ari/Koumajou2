#pragma once
#include "GameObject.h"
class CBackGround :
	public CGameObject
{
private:
	explicit CBackGround();
public:
	virtual ~CBackGround();
public:
	static	CGameObject* Create(const ACTORINFO*	_pPlacement);

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
protected:
};

