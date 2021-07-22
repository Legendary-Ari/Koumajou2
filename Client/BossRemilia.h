#pragma once
#include "GameObject.h"
class CBossRemilia :
	public CGameObject
{
private:
	enum STATE { IDLE, MOVE, ATTACK, SPELL, GUNGNIL, DIE, STATE_END };
	enum PATTERN { P_IDLE, P_PATTERN1, P_MOVE1, P_PATTERN2, P_MOVE2 , P_PATTERN3 , P_MOVE3, P_MOVE4, P_PATTERN4, P_SPELL1, P_SPELL2, P_PATTERN_END };
	CBossRemilia();
public:
	virtual ~CBossRemilia();
public:
	static CGameObject* Create(const OBJECTINFO* _pObjectInfo);
public:
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
	//const _vec3&		Get_Center() const { return m_vBulletCenter; }

	virtual void OnOverlaped(CGameObject* _pHitObject, _vec3 vHitPos) override;
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
	void	Pattern3();
	void	Pattern4();
	void	Spell1();
	void	Spell2();
private:
	bool	m_bHit;
	float	m_fHitRemainTime;
	float	m_fRemainPatternTime;
	_vec3	m_vBulletCenter;
	_vec3	m_vDieDir;
	_vec3	m_vMoveTargetPos;
	float	m_fRemainDieTime;
	float	m_fRamainedDieTime;
	float	m_fMaxDieTime;
	

	vector<float>	m_vecPatternTime;
	PATTERN	m_ePrevPattern;
	PATTERN	m_eCurPattern;
	const float m_fCoolTimePattern3Bullet;
};

