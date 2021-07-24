#pragma once
#include "GameObject.h"
class CRemiliaBullet :
	public CGameObject
{
private:
	CRemiliaBullet();
public:
	virtual ~CRemiliaBullet();
	static CGameObject * Create(const OBJECTINFO * _tObjectInfo, const INFO& tInfo, float _fSpeed);

	virtual void	OnBlockedTile(CGameObject* pHitObject, DIRECTION::ID _eId)override;
	virtual void	OnBlocked(CGameObject* pHitObject, DIRECTION::ID _eId) override;
	virtual void	OnOverlaped(CGameObject* _pHitObject, _vec3 vHitPos) override;

	virtual HRESULT Ready_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
	void		Set_Speed(float _fSpeed);
private:
	virtual void UpdateAttackCollision() override;
	virtual void UpdateBodyCollision() override;
	virtual void UpdateTileCollision() override;
private:
	static int m_pSoundIdx;
	float		m_fSpeed;
};

