#pragma once
#include "Gui.h"
class CWeaponSelectedUi :
	public CGui
{
private:
	CWeaponSelectedUi();
public:
	virtual ~CWeaponSelectedUi();
public:
	static CGameObject* Create(const OBJECTINFO* _pObjectInfo);

	virtual HRESULT Ready_GameObject() override;
	virtual void	InitUpdate_GameObject() override;
	virtual int		Update_GameObject() override;
	virtual void	Late_Update_GameObject() override;
	virtual void	Render_GameObject() override;
	virtual void	Release_GameObject() override;
};

