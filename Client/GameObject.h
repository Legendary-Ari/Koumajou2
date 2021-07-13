#pragma once
class CGameObject abstract
{
public:
	explicit CGameObject();
	virtual ~CGameObject();
public:
	void Set_Pos(const D3DXVECTOR3& vPos);
public:
	virtual HRESULT Ready_GameObject()PURE;
	virtual void	InitUpdate_GameObject()PURE;
	virtual int		Update_GameObject()PURE;
	virtual void	Late_Update_GameObject()PURE; 
	virtual void	Render_GameObject( )PURE; 
	virtual void	Release_GameObject()PURE; 

	virtual void	OnOverlaped(CGameObject* _pHitObject);
	virtual void	OnBlocked(CGameObject* pHitObject, DIRECTION::ID _eId);
	virtual void	OnBlockedTile(CGameObject* pHitObject, DIRECTION::ID _eId);
	virtual void	Set_OnGround(bool _b);
	virtual void	Set_Prefab(const OBJECTINFO* _pPrefab);
	void			Set_ActorInfo(const ACTORINFO* _pPlacement);
	void			Set_Info(const INFO& tInfo);
	void			Add_PosX(float _fPosX);
	void			Add_PosY(float _fPosY);
	virtual const RENDERID::ID&	Get_RenderId() const;
	inline const INFO&	Get_Info() const { return m_tInfo; }
	inline virtual const vector<COLLISION>& Get_BodyCollision() const { return m_vecBodyCollision; }
	inline virtual const vector<COLLISION>& Get_AttackCollision() const { return m_vecAttackCollision; }
	inline virtual const vector<COLLISION>& Get_TileCollision() const { return m_vecTileCollision; }
	inline const bool& Get_IsBlockable() const { return m_bBlockable; }
	inline const bool& Get_IsOverlapable() const { return m_bOverlapable; }
	inline const float& Get_Damage() const { return m_pObjectInfo->fAtk; }
	
protected:
	virtual void UpdateState();
	virtual void UpdateAnimation();
	virtual void UpdateTileCollision();
	virtual void UpdateBodyCollision();
	virtual void UpdateAttackCollision();
	virtual void UpdateDie();
	virtual void UpdateJump();
	virtual void UpdateGravity();
	void		RenderCollision();
	virtual void RenderDieEffect(_vec3 _vPos);

protected:
	bool	m_bDestroyed;
	bool	m_bDead;
	bool	m_bDieInit;
	bool	m_bFliped;
	static bool	m_bRenderCollision;
	bool	m_bVisible;
	//----------Collision
	bool	m_bBlockable;
	bool	m_bOverlapable;
	//------------JumpState--------
	bool m_bJumping;
	bool m_bFalling;
	bool m_bOnGround;
	bool m_bFlying;
	const float m_fGravityAccelerlation;
	//------------------
	INFO m_tInfo; 
	float		m_fCurHp;
	const OBJECTINFO*	m_pObjectInfo;
	const ACTORINFO*	m_pActorInfo;
	vector<const ANIMATION*> m_vecAnimation;
	vector<COLLISION> m_vecBodyCollision;
	vector<COLLISION> m_vecAttackCollision;
	vector<COLLISION> m_vecTileCollision;

	UINT	m_uiAnimationFrame;
	float	m_fAnimationCumulatedTime;

	float	m_fCoolDownDieEffectRemainTime;
	float	m_fMaxCoolDownDieEffectTime;

	BYTE m_ePrevState;
	BYTE m_eCurState;
};

