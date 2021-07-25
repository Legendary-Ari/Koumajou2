#pragma once
#include "Scene.h"
class CGameOverScene :
	public CScene
{
private:
	CGameOverScene();
public:
	virtual ~CGameOverScene();

	static CScene* Create();

	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void InitUpdate_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
private:
	float m_fSceneTime;
};

