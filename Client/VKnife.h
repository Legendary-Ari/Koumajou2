#pragma once
#include "VSkill.h"
class CVKnife :
	public CVSkill
{
private:
	CVKnife();
public:
	virtual ~CVKnife();

	static CGameObject* Create(OBJECTINFO* _pObjectInfo);

	virtual HRESULT Ready_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
	virtual void Use(const INFO& tInfo) override;

private:
	OBJECTINFO* m_pBulletInfo;
};

