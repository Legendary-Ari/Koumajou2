#pragma once
#include "GameObject.h"
class CVSkill abstract
	:public CGameObject
{
public:
	explicit CVSkill();
	virtual ~CVSkill();
public:
	virtual void Use(const INFO& tInfo)PURE;
private:
	OBJECTINFO* m_pObjectInfo;
};

