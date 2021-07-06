#pragma once
#include "GameObject.h"
class CEnemy :
	public CGameObject
{
public:
	explicit	CEnemy();
	virtual		~CEnemy();
public:
	// CGameObject��(��) ���� ��ӵ�
	static	CGameObject* Create(const OBJECTINFO*	_tObjectInfo, const ACTORINFO* _pActorInfo);

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

};

