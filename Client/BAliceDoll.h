#pragma once
#include "GameObject.h"
class CBossAlice;
class CBAliceDoll :
	public CGameObject
{
private:
	enum STATE { IDLE, MOVE, ATTACK,STATE_END };
	CBAliceDoll();
public:
	virtual ~CBAliceDoll();
public:
	static	CGameObject* Create(const OBJECTINFO * _tObjectInfo, const INFO& _tInfo, CBossAlice* _pAlice);

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
	void			Set_Alice(CBossAlice* _pAlice);
private:
	virtual void UpdateAnimation() override;
	virtual void UpdateBodyCollision() override;
	void UpdatePattern();



	CBossAlice*	m_pAlice;
	float m_fDistance;
	const float m_fAtkRatio;
	float	m_fCoolTime;
};