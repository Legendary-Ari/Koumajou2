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
	void			Add_PosX(float _fPosX);
	void			Add_PosY(float _fPosY);
	inline virtual const RENDERID::ID&	Get_RenderId() const { return (RENDERID::ID)m_pObjectInfo->eRenderId; }
	inline const INFO&	Get_Info() const { return m_tInfo; }
	inline virtual const vector<COLLISION>& Get_BodyCollision() const { return m_vecBodyCollision; }
	inline virtual const vector<COLLISION>& Get_AttackCollision() const { return m_vecAttackCollision; }
	inline const bool& Get_IsBlockable() const { return m_bBlockable; }
	inline const bool& Get_IsOverlapable() const { return m_bOverlapable; }
	
protected:
	virtual void UpdateState();
	virtual void UpdateAnimation();
	virtual void UpdateBodyCollision();
	virtual void UpdateAttackCollision();
	void		RenderCollision();

protected:
	bool	m_bDestroyed;
	bool	m_bIsFliped;
	bool	m_bRenderCollision;
	bool	m_bVisible;
	bool	m_bBlockable;
	bool	m_bOverlapable;
	INFO m_tInfo; 
	const OBJECTINFO*	m_pObjectInfo;
	const ACTORINFO*	m_pActorInfo;
	vector<const ANIMATION*> m_vecAnimation;
	vector<COLLISION> m_vecBodyCollision;
	vector<COLLISION> m_vecAttackCollision;

	UINT	m_uiAnimationFrame;
	//INFO* m_pInfo; 
};

