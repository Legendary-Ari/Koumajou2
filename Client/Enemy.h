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
	virtual void InitUpdate_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

	virtual void OnOverlaped(CGameObject* _pHitObject) override;

	void			Set_Active(bool _bActive);
	void			Set_Fliped(bool _bFliped);
protected:
	virtual void UpdateGravity() override;
	virtual bool UpdateActive();
	virtual void UpdatePattern();
	virtual void UpdateAttackCoolDown();
	virtual void UpdateAnimation() override;
protected:
	bool	m_bActived;
	bool	m_bHit;
	float	m_fHitCumulatedTime;
	const float m_fHitMaxTime;
	float	m_fAttackCoolDownRemainTime;
	float	m_fMaxAttackCoolDownTime;
	float	m_fJumpPower;
};

