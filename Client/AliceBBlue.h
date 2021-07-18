#pragma once
#include "GameObject.h"

class CAliceBBlue :
	public CGameObject
{
private:
	CAliceBBlue();
public:
	virtual ~CAliceBBlue();
	static CGameObject * Create(const OBJECTINFO * _tObjectInfo, const INFO& tInfo);

	virtual void	OnBlockedTile(CGameObject* pHitObject, DIRECTION::ID _eId)override;
	virtual void	OnBlocked(CGameObject* pHitObject, DIRECTION::ID _eId) override;
	virtual void	OnOverlaped(CGameObject* _pHitObject, _vec3 vHitPos) override;

	virtual HRESULT Ready_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	virtual void UpdateAttackCollision() override;
	virtual void UpdateBodyCollision() override;
	virtual void UpdateTileCollision() override;
private:
	float m_fAcceleration;
};

