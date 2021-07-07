#pragma once
#include "GameObject.h"
class CPlayer final:
	public CGameObject
{
public:
	enum STATE { IDLE, S_WALKING, WALKING, STOPING, CROUCH, STANDUP,  TURNING, U_JUMPING, JUMPING, FALLING, ATTACK, JUMP_ATTACK, HIT, F_LANDING, L_FLYING, R_FLYING, DODGE, DODGE_ATTACK, D_ATTACK, JUMP_D_ATTACK, KNIFE, D_KNIFE, STATE_END	};
private:
	explicit CPlayer();
public:
	virtual ~CPlayer();
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
public:
	static CGameObject* Create(const ACTORINFO* _pActorInfo, const OBJECTINFO* _pPrefab);

private:
	virtual void	UpdateState() override;
	virtual void	UpdateAnimation() override;
	void	UpdateMoveWithPressKey();
	void	Offset();
	
private:
	
	bool	m_bJumping;
	bool	m_bFalling;
	float	m_fAnimationCumulatedTime;
	bool	m_bIsFliped;
	bool	m_bPrevIsFliped;
	bool	m_bAttacking;
	bool	m_bCrouch;
	bool	m_bStoping;
	STATE m_ePrevState;
	STATE m_eCurState;
};

