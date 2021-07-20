#pragma once
#include "Item.h"
class CChi :
	public CItem
{
private:
	explicit CChi();
public:
	virtual ~CChi();
public:
	static CGameObject* Create(bool _bIsDeca,_vec3 _vPos);
	virtual HRESULT Ready_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual int Update_GameObject() override;
	
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
	virtual void Give_ThisItem() override;
	void		Set_Chi(UINT _uiChi);
public:

private:
	UINT		m_uiChiMount;
	float		m_fSpeed;
};

