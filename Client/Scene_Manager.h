#pragma once
#include "Scene.h"
class CScene_Manager
{
	DECLARE_SINGLETON(CScene_Manager)
public:
	enum ID { STAGE_1_1, STAGE_1_2, STAGE_1_3, STAGE_1_4, STAGE_2_1, STAGE_2_2, SCENE_LOADING, SCENE_STAGE, END };
private:
	CScene_Manager();
	~CScene_Manager();
public:
	HRESULT Change_Scene_Manager(const ID eID);
public:
	void Update_Scene_Manager(); 
	void Render_Scene_Manager(); 
	void Release_Scene_Manager(); 
private:
	ID m_eCurScene; 
	ID m_eNextScene; 
	CScene* m_pScene; 
};

