#pragma once
#include "GameObject.h"
class CSubMeling :
	public CGameObject
{
public:
	CSubMeling();
	virtual ~CSubMeling();
public:
	static CGameObject * Create(const INFO& tInfo);

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
	virtual void UpdateTileCollision() override;
	float		m_fRemainUpTime;
	bool		m_bIsGoingUp;
};

