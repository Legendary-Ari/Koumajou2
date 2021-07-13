#pragma once
#include "Enemy.h"
class CRoseStem :
	public CEnemy
{
private:
	CRoseStem();
public:
	virtual ~CRoseStem();
public:
	static CGameObject* Create(const INFO& _tInfo, const OBJECTINFO* _pObjectInfo, const CGameObject* _pRose, float _OrbitRadius);

	virtual HRESULT Ready_GameObject() override;
	virtual void	InitUpdate_GameObject() override;
	virtual int		Update_GameObject() override;
	virtual void	Late_Update_GameObject() override;
	virtual void	Render_GameObject() override;

	void			Set_Rose(const CGameObject* _pRose);
	void			Set_OrbitRadius(float _fOrbitRadius);
private:
	virtual void	UpdateAttackCollision() override;
private:
	const CGameObject*	m_pRose;
	_vec3			m_vCenterPos;
	float			m_fOrbitRadius;
};

