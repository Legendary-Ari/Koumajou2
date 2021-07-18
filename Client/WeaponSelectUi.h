#pragma once
#include "Gui.h"
class CWeaponSelectUi :
	public CGui
{
private:
	CWeaponSelectUi();
public:
	virtual ~CWeaponSelectUi();
public:
	static CGameObject* Create(const OBJECTINFO* _pObjectInfo);

	virtual HRESULT Ready_GameObject() override;
	virtual void	InitUpdate_GameObject() override;
	virtual int		Update_GameObject() override;
	virtual void	Late_Update_GameObject() override;
	virtual void	Render_GameObject() override;
	virtual void	Release_GameObject() override;
};

