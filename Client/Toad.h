#pragma once
#include "Enemy.h"
class CToad final:
	public CEnemy
{
private:
	enum STATE {IDLE, ATTACK, JUMP, DIE, STATE_END };
private:
	CToad();
public:
	virtual ~CToad();
public:
	static CGameObject* Create(const ACTORINFO* _pActorInfo, const OBJECTINFO* _pObjectInfo);

	virtual HRESULT Ready_GameObject() override;
	virtual int		Update_GameObject() override;
	virtual void	Late_Update_GameObject() override;
	virtual void	Render_GameObject() override;
private:
	virtual void	UpdateBodyCollision() override;
	virtual void	UpdateAttackCollision() override;
	virtual void	UpdatePattern() override;
	virtual void	UpdateState() override;
	virtual void	UpdateAnimation() override;
	virtual void	UpdateJump()override;
	virtual void	UpdateDie() override ;

};

