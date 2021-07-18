#pragma once
#include "VSkill.h"
class CGame_Manager
{
DECLARE_SINGLETON(CGame_Manager)
public:
	
private:
	CGame_Manager();
	~CGame_Manager();
public:
	HRESULT Ready_Game_Manager();
public:
	UINT		m_uiSelectedSubWeapon[3];
	CScene_Manager::ID m_eCurStage;
};

