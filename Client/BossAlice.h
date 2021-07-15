#pragma once
#include "GameObject.h"
class CBossAlice :
	public CGameObject
{

private:
	enum STATE {IDLE, MOVE, ATTACK, SUMMON, STATE_END};
	CBossAlice();
public:
	virtual ~CBossAlice();
public:
	static CGameObject* Create(OBJECTINFO* _pObjectInfo);
public:
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

	virtual void OnOverlaped(CGameObject* _pHitObject, _vec3 vHitPos) override;

protected:
	virtual bool UpdateActive();
	virtual void UpdatePattern();
	//virtual void UpdateActionCoolDown();
	virtual void UpdateAnimation() override;
protected:
	bool	m_bHit;
	float	m_fHitRemainTime;
	const float m_fHitMaxTime;
	float	m_fRamainedDieTime;
	float	m_fMaxDieTime;
};

