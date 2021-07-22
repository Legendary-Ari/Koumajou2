#pragma once
#include "Scene.h"
class CStage1_1 final :
	public CScene
{
private:
	explicit CStage1_1();
public:
	virtual ~CStage1_1();
public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual void InitUpdate_Scene() override;
	virtual void Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
public:
	static CScene* Create(); 
};

