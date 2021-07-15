#pragma once
#include "Enemy.h"
class CAluraUne final:
	public CEnemy
{
private:
	enum STATE { IDLE, ATTACK, STATE_END };
private:
	CAluraUne();
public:
	virtual ~CAluraUne();
public:
	static CGameObject* Create(const ACTORINFO* _pActorInfo, const OBJECTINFO* _pObjectInfo);

	virtual HRESULT Ready_GameObject() override;
	virtual int		Update_GameObject() override;
	virtual void	Late_Update_GameObject() override;
	virtual void	Render_GameObject() override;
private:
	virtual void	UpdateBodyCollision() override;
	virtual void	UpdatePattern() override;
	virtual void	UpdateState() override;
	virtual void	UpdateAnimation() override;
	virtual void	UpdateDie() override;
private:
	float m_fRamainedDieTime;
	const float m_fMaxDieTime;
	float m_fCoolDownAtkRemainTime;
	int m_iRemainAtk;
	const int m_iMaxAtkMount;
};

