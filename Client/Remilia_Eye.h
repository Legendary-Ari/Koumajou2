#pragma once
#include "Enemy.h"
class CBossRemilia;
class CRemilia_Eye :
	public CEnemy
{
private:
	CRemilia_Eye();
public:
	virtual ~CRemilia_Eye();
public:
	static CGameObject* Create(int idx, CBossRemilia* _pRemilia);

	virtual HRESULT Ready_GameObject() override;
	virtual int		Update_GameObject() override;
	virtual void	Late_Update_GameObject() override;
	virtual void	Render_GameObject() override;
	void			Set_Index(int idx);
private:
	virtual void	UpdateBodyCollision() override;
	virtual void	UpdateDie() override;

	void	Set_Remilia(CBossRemilia* _pRemilia);
private:
	float	m_fDieRandomSign;
	float	m_fRemainedDieTime;
	const float	m_fMaxDieTime;
	const float m_fDistanceFromRemilia;
	int		m_iIdx;
	CBossRemilia*	m_pRemilia;
};

