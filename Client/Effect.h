#pragma once
#include "GameObject.h"
class CEffect :
	public CGameObject
{
private:
	explicit CEffect();
public:
	virtual ~CEffect();
public:
	static	CGameObject* Create(const ANIMATION*	_tAnimationInfo, D3DXVECTOR3 _vPos);

	void Set_Prefab(const ANIMATION * _pPrefab);

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
protected:
	const ANIMATION*	m_pAnimationInfo;
	float				m_fTimeStack;
	int					m_iCurFrame;
};

