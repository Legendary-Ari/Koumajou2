#pragma once
#include "GameObject.h"
class CBGSelectCastle :
	public CGameObject
{
private:
	CBGSelectCastle();
public:
	virtual ~CBGSelectCastle();
public:
	static	CGameObject* Create(const OBJECTINFO * _pObjectInfo);

	virtual HRESULT Ready_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
};

