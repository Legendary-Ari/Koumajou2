#pragma once
#include "VSkill.h"
class CGame_Manager
{
DECLARE_SINGLETON(CGame_Manager)
private:
	enum SKILL_ID {KNIFE, CLOCK, MEILING, ALICE, SKILL_END};
private:
	CGame_Manager();
	~CGame_Manager();
public:
	HRESULT Ready_Game_Manager();
public:
	const CVSkill* pVSkill[SKILL_END];
};

