#pragma once
#include "GameObject.h"
class CAluraUneProjectile :
	public CGameObject
{
private:
	explicit CAluraUneProjectile();
public:
	virtual ~CAluraUneProjectile();
public:
	static	CGameObject* Create(const OBJECTINFO*	_tObjectInfo, D3DXVECTOR3 _vPos);

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
private:
	virtual void UpdateAnimation() override;
	virtual void UpdateAttackCollision() override;
};

