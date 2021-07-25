#pragma once
#include "Scene.h"
class CLogoScene :
	public CScene
{
	CLogoScene();
public:
	virtual ~CLogoScene();
public:

	static CScene* Create();

	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void InitUpdate_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
private:
	int		m_idx;
};

