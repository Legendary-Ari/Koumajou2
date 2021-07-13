#pragma once
#include "Enemy.h"
class CRose :
	public CEnemy
{
private:
	enum STATE { IDLE, DIE, STATE_END };
private:
	CRose();
public:
	virtual ~CRose();
public:
	static CGameObject* Create(const ACTORINFO* _pActorInfo, const OBJECTINFO* _pObjectInfo);

	virtual HRESULT Ready_GameObject() override;
	virtual void	InitUpdate_GameObject() override;
	virtual int		Update_GameObject() override;
	virtual void	Render_GameObject() override;

private:
	virtual void	UpdateAttackCollision() override;
	vector<CEnemy*> m_vecRoseParts;
	const UINT		m_uiRoseStemMount;
};

