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
	virtual void Set_Hit(bool _bHit);
	virtual void Set_Die();

	virtual void OnOverlaped(CGameObject* _pHitObject, _vec3 vHitPos) override;

	void			Set_Active(bool _bActive);
	void			Set_Fliped(bool _bFliped);
protected:
	virtual void UpdateGravity() override;
	virtual bool UpdateActive();
	virtual void UpdatePattern();
	virtual void UpdateActionCoolDown();
	virtual void UpdateAnimation() override;
protected:
	bool	m_bActived;
	bool	m_bHit;
	float	m_fHitCumulatedTime;
	const float m_fHitMaxTime;
	float	m_fActionCoolDownRemainTime;
	float	m_fMaxAttackCoolDownTime;
	float	m_fJumpPower;
	float	m_fAttackWakeDistance;
};

