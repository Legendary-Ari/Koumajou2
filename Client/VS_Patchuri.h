#pragma once
#include "VSkill.h"
class CVS_Patchuri :
	public CVSkill
{
private:
	CVS_Patchuri();
public:
	virtual ~CVS_Patchuri();
public:
	static CGameObject* Create();

	virtual HRESULT Ready_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Release_GameObject() override;
	virtual void Use(const INFO& tInfo) override;
};

