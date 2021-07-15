#pragma once
#include "Enemy.h"
class CRoseHead :
	public CEnemy
{
private:
	CRoseHead();
public:
	virtual ~CRoseHead();
public:
	static CGameObject* Create(const INFO& _tInfo, const OBJECTINFO* _pObjectInfo,CGameObject* _pRose, float _OrbitRadius);

	virtual HRESULT Ready_GameObject() override;
	virtual void	InitUpdate_GameObject() override;
	virtual int		Update_GameObject() override;
	virtual void	Late_Update_GameObject() override;
	virtual void	Render_GameObject() override;

	virtual void	OnBlockedTile(CGameObject* pHitObject, DIRECTION::ID _eId);

	void			Set_Rose(CGameObject* _pRose);
	void			Set_OrbitRadius(float _fOrbitRadius);
private:
	virtual void	UpdateBodyCollision() override;
	virtual void	UpdateTileCollision() override;
	virtual void	UpdateAnimation() override;
private:
	CGameObject*	m_pRose;
	_vec3			m_vCenterPos;
	float			m_fOrbitRadius;
};

