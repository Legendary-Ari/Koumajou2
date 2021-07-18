#pragma once
#include "Scene.h"
class CSceneWeaponSelect :
	public CScene
{
private:
	CSceneWeaponSelect();
public:
	virtual ~CSceneWeaponSelect();
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual void InitUpdate_Scene() override;
	virtual void Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
public:
	static CScene* Create();
};

