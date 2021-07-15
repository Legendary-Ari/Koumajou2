#pragma once
#include "GameObject.h"
class CEffect :
	public CGameObject
{
public:
	explicit CEffect();
public:
	virtual ~CEffect();
public:
	static	CGameObject* Create(const ANIMATION*	_tAnimationInfo, const INFO& _tInfo);

	void Set_Animation(const ANIMATION * _pPrefab);
	void Set_Dir(_vec3 _vDir);

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

	virtual const RENDERID::ID& Get_RenderId() const override;
protected:
	const ANIMATION*	m_pAnimationInfo;
	float				m_fTimeStack;
};

