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

	virtual void	Set_Prefab(const OBJECTINFO* _pPrefab);
	void			Set_ActorInfo(const ACTORINFO* _pPlacement);
	inline const RENDERID::ID&	Get_RenderId() const { return (RENDERID::ID)m_pObjectInfo->eRenderId; }
	inline const INFO&	Get_Info() const { return m_tInfo; }
protected:
	virtual void UpdateState();
	virtual void UpdateAnimation();

protected:
	bool	m_bDestroyed;
	
	INFO m_tInfo; 
	const OBJECTINFO*	m_pObjectInfo;
	const ACTORINFO*	m_pActorInfo;
	vector<const ANIMATION*> m_vecAnimation;

	UINT	m_uiAnimationFrame;
	//INFO* m_pInfo; 
};

