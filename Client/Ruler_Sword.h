#pragma once
#include "Enemy.h"
class CRuler_Sword :
	public CEnemy
{
private:
	CRuler_Sword();
public:
	virtual ~CRuler_Sword();
public:
	static CGameObject* Create(const ACTORINFO* _pActorInfo, const OBJECTINFO* _pObjectInfo, const CGameObject* _pRuler_Body);

	virtual HRESULT Ready_GameObject() override;
	virtual int		Update_GameObject() override;

	void Set_RulerBody(const CGameObject* _pRulerBody);
private:
	const CGameObject* m_pRulerBody;
	static const float m_fDisBodyToSword;
};

