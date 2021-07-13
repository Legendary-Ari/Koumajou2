#pragma once
#include "Enemy.h"
class CSkeleton :
	public CEnemy
{
private:
	enum STATE { IDLE, ATTACK, DIE, STATE_END };
private:
	CSkeleton();
public:
	virtual ~CSkeleton();
public:
	static CGameObject* Create(const ACTORINFO* _pActorInfo, const OBJECTINFO* _pObjectInfo);

	virtual HRESULT Ready_GameObject() override;
	virtual int		Update_GameObject() override;
	virtual void	Render_GameObject() override;

private:
	virtual void	UpdateBodyCollision() override;
};

