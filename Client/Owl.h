#pragma once
#include "Enemy.h"
class COwl :
	public CEnemy
{
private:
	enum STATE { IDLE, STATE_END };
private:
	COwl();
public:
	virtual ~COwl();
public:
	static CGameObject* Create(const ACTORINFO* _pActorInfo, const OBJECTINFO* _pObjectInfo);

	virtual HRESULT Ready_GameObject() override;
	virtual int		Update_GameObject() override;
	virtual void	Render_GameObject() override;
	inline virtual const vector<COLLISION>& Get_TileCollision() const override { return vector<COLLISION>(); }
private:
	virtual void	UpdatePattern() override;
	virtual void	UpdateBodyCollision() override;
	virtual void	UpdateDie() override;
private:
	float		m_fCumulatedDieTime;
	const float	m_fMaxDieTime;
	float			m_fDieRandomSign;
	const float m_fDieRotateSpeed;
	_vec3		m_vTargetDir;
	_vec3		m_vTargetPos;
};

