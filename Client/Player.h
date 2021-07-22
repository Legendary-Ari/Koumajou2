#pragma once
#include "GameObject.h"
class CVSkill;
class CPlayer final:
	public CGameObject
{
public:
	enum STATE { IDLE, S_WALKING, WALKING, STOPING, CROUCH
		, STANDUP,  TURNING, U_JUMPING, JUMPING, FALLING
		, ATTACK, JUMP_ATTACK, BOUNCE, HIT, F_LANDING, L_FLYING
		, R_FLYING, DODGE, DODGE_ATTACK, D_ATTACK, JUMP_D_ATTACK
		, S_KNIFE, E_KNIFE, DS_KNIFE, DE_KNIFE, STATE_END	};
private:
	enum SKILL { PREV, CUR, NEXT, SKILL_END };
private:
	explicit CPlayer();
public:
	virtual ~CPlayer();
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int		Update_GameObject() override;
	virtual void	InitUpdate_GameObject() override;
	virtual void	Late_Update_GameObject() override;
	virtual void	Render_GameObject() override;
	virtual void	Release_GameObject() override;
	virtual void	OnBlocked(CGameObject* pHitObject, DIRECTION::ID _eId) override;
public:
	static CGameObject* Create(const OBJECTINFO* _pPrefab, const INFO& _tInfo);

public:
	virtual void	OnBlockedTile(CGameObject* pHitObject, DIRECTION::ID _eId)override;
	virtual void	OnOverlaped(CGameObject* _pHitObject, _vec3 vHitPos) override;
	const vector<COLLISION>& Get_TileCollision() const { return m_vecBodyTileCollision; }

	virtual void	Set_OnGround(bool _b) override;
	virtual const float* Get_HpPointer() const { return &m_fCurHp; }
	const float*	Get_MpPointer() const { return &m_fCurMp; }
	const UINT*		Get_ChiPointer() const { return &m_uiCurChi; }
	const UINT*		Get_LifePointer() const { return &m_uiLife; }
	void			Add_Chi(UINT _uiChi) { m_uiCurChi += _uiChi; }
	void			Sub_Chi(UINT _uiChi) { m_uiCurChi -= _uiChi; }
private:
	virtual void	UpdateState() override;
	virtual void	UpdateAnimation() override;
	virtual void	UpdateBodyCollision() override;
	virtual void	UpdateAttackCollision() override;
	
	void	UpdateMoveWithPressKey();
	void	Offset();
	virtual void UpdateJump() override;
private:
	const OBJECTINFO*	m_pBulletInfo;
	vector<COLLISION> m_vecBodyTileCollision;

	bool	m_bHit;
	bool	m_bInvincible;
	const float	m_fHitTimeLength;
	const float	m_fInvincibleTimeLength;
	float	m_fHitCumulateTime;

	bool	m_bPrevIsFliped;
	bool	m_bAttacking;
	bool	m_bKnifeAttacking;
	bool	m_bCrouch;
	bool	m_bStoping;
	bool	m_bDodge;
	float	m_fJumpRamainedTime;
	float	m_fKnifeRemainedTime;
	const float	m_fJumpMaxTime;
	const float m_fMaxKnifeTime;
	float		m_fCurMp;
	UINT		m_uiCurChi;
	UINT		m_uiLife;
	bool		m_bFlyable;
	const float	m_fFlyingSpeed;
	
	CVSkill*	m_pVSkill[SKILL_END];

};

