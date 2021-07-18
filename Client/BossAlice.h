#pragma once
#include "GameObject.h"
class CBossAlice :
	public CGameObject
{

private:
	enum BULLET_TIME {BLUE_ABOVE, BLUE_BELOW, GREEN};
	enum STATE {IDLE, MOVE, ATTACK, SUMMON, DIE, STATE_END};
	enum PATTERN {P_IDLE, P_PATTERN1, P_IDLE2, P_PATTERN2, P_IDLE3, P_SUMMON, P_MOVE1, P_MOVE2, P_PATTERN_END};
	CBossAlice();
public:
	virtual ~CBossAlice();
public:
	static CGameObject* Create(const OBJECTINFO* _pObjectInfo);
public:
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
	const _vec3&		Get_Center() const { return m_vBulletCenter; }
	

	virtual void OnOverlaped(CGameObject* _pHitObject, _vec3 vHitPos) override;
	bool		Get_DollGoUp() const;
	const bool& Get_Fliped() const;
protected:
	virtual void UpdatePattern();
	//virtual void UpdateActionCoolDown();
	virtual void UpdateDie() override;
	virtual void UpdateState() override;
	virtual void UpdateAnimation() override;
	virtual void UpdateBodyCollision() override;
private:
	void	Pattern1();
	void	Pattern2();
	void	Summon();
	void	CreateBlueBullet(const INFO& _tInfo);
	void	CreateGreenBullet(const INFO& _tInfo);
protected:
	bool	m_bHit;
	float	m_fHitRemainTime;
	const float m_fHitMaxTime;
	float	m_fRamainedDieTime;
	float	m_fMaxDieTime;
	float	m_fActionRemainedTime;
	const float	m_fPatternTime[P_PATTERN_END] = { 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 2.f, 2.f };
	PATTERN		m_ePatternIndex;
	_vec3	m_vTargetPos;
	_vec3	m_vDieDir;
	float	m_fRemainDieTime;
	float	m_fRemainBulletTime[3];
	UINT		m_uiBlueRemainBullet[2];
	const UINT m_uiMaxBlueBullet;
	const float m_fBlueDistance;
	const float m_fGreenDistance;
	bool		m_bPaternInit;
	_vec3	m_vBulletCenter;
	bool	m_bSummon;
	float	m_fCoolDownSummon;
	UINT	m_uiRemainSummon;
	_vec3	m_vTarget;
};

