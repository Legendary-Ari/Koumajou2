#pragma once
#include "GameObject.h"
class CBossSkillEffect :
	public CGameObject
{
private:
	explicit CBossSkillEffect();
public:
	virtual ~CBossSkillEffect();
public:
	static	CGameObject* Create(const OBJECTINFO*	_tObejctInfo, const _vec3& _vPos);
	
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

	virtual const RENDERID::ID& Get_RenderId() const override;
protected:
	float				m_fRemainKillTime;
	float				m_fKillTime;
};

