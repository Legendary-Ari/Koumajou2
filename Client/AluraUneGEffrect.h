#pragma once
#include "Effect.h"
class CAluraUneGEffrect :
	public CEffect
{
public:
	CAluraUneGEffrect();
	virtual ~CAluraUneGEffrect();
public:
	static	CGameObject* Create(const ANIMATION*	_tAnimationInfo, D3DXVECTOR3 _vPos);

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
};

