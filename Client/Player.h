#pragma once
#include "GameObject.h"
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
	explicit CPlayer();
public:
	virtual ~CPlayer();
public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual int		Update_GameObject() override;
	virtual void	InitUpdate_GameObject() override;
	virtual void	Late_Update_GameObject() override;
	virtual void	Render_GameObject() override;
	virtual void	Release_GameObject() override;
	virtual void	OnBlocked(CGameObject* pHitObject, DIRECTION::ID _eId) override;
	virtual void	Set_OnGround(bool _b) override;
public:
	static CGameObject* Create(const ACTORINFO* _pActorInfo, const OBJECTINFO* _pPrefab);

public:
	virtual void	OnBlockedTile(CGameObject* pHitObject, DIRECTION::ID _eId)override;
	virtual void	OnOverlaped(CGameObject* pHitObject) override ;
	const vector<COLLISION>& Get_TileCollision() const { return m_vecBodyTileCollision; }
private:
	virtual void	UpdateState() override;
	virtual void	UpdateAnimation() override;
	virtual void	UpdateBodyCollision() override;
	virtual void	UpdateAttackCollision() override;
	
	void	UpdateMoveWithPressKey();
	void	Offset();
	
private:
	
	vector<COLLISION> m_vecBodyTileCollision;

	bool	m_bHit;
	bool	m_bInvincible;
	const float	m_fHitTimeLength;
	const float	m_fInvincibleTimeLength;
	float	m_fHitCumulateTime;

	bool	m_bPrevIsFliped;
	bool	m_bAttacking;
	bool	m_bCrouch;
	bool	m_bStoping;
	bool	m_bDodge;
	float	m_fActionRamainedTime;
	const float	m_fJumpMaxTime;



};

