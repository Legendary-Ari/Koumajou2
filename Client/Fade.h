#pragma once
#include "GameObject.h"
class CFade :
	public CGameObject
{
private:
	CFade();
public:
	virtual ~CFade();
public:
	static	CGameObject* Create(bool _bIn);

	void	Set_FadeInOrOut(bool _bIn);

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

	virtual const RENDERID::ID& Get_RenderId() const override;
private:
	bool	m_bFade;
	int		m_iAlpha;
};

