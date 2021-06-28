#pragma once
#include "GameObject.h"
class CBullet :
	public CGameObject
{
private:
	explicit CBullet();
public:
	virtual ~CBullet();
public:
	static	CGameObject* Create(const OBJECTINFO*	_tObjectInfo, D3DXVECTOR3 _vPos, float _fAngle );

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
private:

};

