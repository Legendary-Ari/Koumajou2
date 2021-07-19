#pragma once
#include "Effect.h"
class CVS_Clock;
class CLunaDial :
	public CEffect
{
	CLunaDial();
public:
	virtual ~CLunaDial();
public:
	static	CGameObject* Create(const ANIMATION*	_tAnimationInfo, CVS_Clock* _pClock);

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Release_GameObject() override;
	void		Set_Clock(CVS_Clock* _pClock);
private:
	CVS_Clock*	m_pClock;
};

