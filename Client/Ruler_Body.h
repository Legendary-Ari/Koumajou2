#pragma once
#include "Enemy.h"
class CRuler_Body :
	public CEnemy
{
private:
	explicit CRuler_Body();
public:
	virtual ~CRuler_Body();
public:
	static CGameObject* Create(const ACTORINFO* _pActorInfo, const OBJECTINFO* _pObjectInfo);

	virtual HRESULT Ready_GameObject() override;
};

