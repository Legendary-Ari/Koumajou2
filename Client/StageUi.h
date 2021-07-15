#pragma once
#include "Gui.h"
class CStageUi :
	public CGui
{
private:
	explicit CStageUi();
public:
	virtual ~CStageUi();
public:
	static CGameObject* Create();

	virtual HRESULT Ready_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual int		Update_GameObject() override;
	virtual void	Late_Update_GameObject() override;
	virtual void	Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	void 	RenderHp();
	void	RenderMp();
	void	RenderRing();
	void	RenderIcon();
	void	RenderChiText();
	void	RenderLifeText();
private:
	INFO	m_vHpBarInfo;
	INFO	m_vMPBarInfo;
	INFO	m_vRingInfo;
	INFO	m_vIconInfo[3];
	const float*	m_pCurHp;
	float	m_fPrevHp;
	const	UINT	m_uiIconMount;
};

