#pragma once
#include "GameObject.h"
class CEnemy :
	public CGameObject
{
public:
	explicit	CEnemy();
	virtual		~CEnemy();
public:
	// CGameObject을(를) 통해 상속됨
	static	CGameObject* Create(const OBJECTINFO*	_tObjectInfo, const ACTORINFO* _pActorInfo);

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

};

