#pragma once
#include "GameObject.h"
class CPillor :
	public CGameObject
{
public:
	CPillor();
public:
	virtual ~CPillor();

	static CGameObject* Create(const ACTORINFO * _pActorInfo, const OBJECTINFO * _pObjectInfo);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
	void UpdateTileCollision();
};

