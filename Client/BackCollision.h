#pragma once
#include "GameObject.h"
class CBackCollision :
	public CGameObject
{
private:
	CBackCollision();
public:
	virtual ~CBackCollision();
public:
	static CGameObject* Create(const ACTORINFO* _pActorInfo, const OBJECTINFO* _pObjectInfo);

	virtual HRESULT Ready_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
protected:
	virtual void	UpdateBodyCollision() override;
};

