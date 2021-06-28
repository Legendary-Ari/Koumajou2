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
	void			Set_Placement(const PLACEMENT* _pPlacement);
	inline const RENDERID::ID&	Get_RenderId() const { return m_eRenderId; }
protected:
	bool	m_bDestroyed;
#define m_bDead m_bDestroyed
	
	INFO m_tInfo; 
	float m_fAngle = 0.f;
	const OBJECTINFO*	m_pObjectInfo;
	const PLACEMENT*	m_pPlacement;

	RENDERID::ID		m_eRenderId;
	//INFO* m_pInfo; 
};

