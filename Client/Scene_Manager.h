#pragma once
#include "Scene.h"
class CScene_Manager
{
	DECLARE_SINGLETON(CScene_Manager)
public:
	enum ID { MENU, SELECT, STAGE_1_1, STAGE_1_4, STAGE_2_1, STAGE_2_4, SCENE_LOADING, GAMEOVER, END };
private:
	CScene_Manager();
	~CScene_Manager();
public:
	HRESULT Change_Scene_Manager(const ID eID, const ID eNextId = CScene_Manager::END);
public:
	void Update_Scene_Manager(); 
	void InitUpdate_Scene_Manager();
	void Render_Scene_Manager(); 
	void Release_Scene_Manager(); 
	const _vec3& Get_StartPos() const;
	HRESULT Reset();
private:
	ID m_ePrevScene;
	ID m_eCurScene; 
	ID m_eNextScene; 
	CScene* m_pScene; 
public:
	static ID g_eStartScene;
};

