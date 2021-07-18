#pragma once
#include "Gui.h"
class CBossHpUi :
	public CGui
{
private:
	CBossHpUi();
public:
	virtual ~CBossHpUi();
public:
	static CGameObject* Create(const OBJECTINFO* _pObjectInfo);

	virtual HRESULT Ready_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual int		Update_GameObject() override;
	virtual void	Late_Update_GameObject() override;
	virtual void	Render_GameObject() override;
	virtual void Release_GameObject() override;
private:
	void 	RenderHp();
	void	RenderIcon();
private:
	INFO	m_tHpBarInfo;
	INFO	m_tIconInfo;
	const float*	m_pCurHp;
};

