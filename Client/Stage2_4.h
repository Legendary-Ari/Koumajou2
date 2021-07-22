#pragma once
#include "Scene.h"
class CStage2_4 :
	public CScene
{
private:
	explicit CStage2_4();
public:
	virtual ~CStage2_4();
public:
	// CScene��(��) ���� ��ӵ�
	virtual HRESULT Ready_Scene() override;
	virtual void InitUpdate_Scene() override;
	virtual void Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
public:
	static CScene* Create();
};

