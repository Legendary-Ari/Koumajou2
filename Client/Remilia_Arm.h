#pragma once
#include "Enemy.h"
class CBossRemilia;
class CRemilia_Arm :
	public CEnemy
{
private:
	CRemilia_Arm();
public:
	virtual ~CRemilia_Arm();
public:
	static CGameObject* Create(bool _bLeft, CBossRemilia* _pRemilia);

	virtual HRESULT Ready_GameObject() override;
	virtual int		Update_GameObject() override;
	virtual void	Late_Update_GameObject() override;
	virtual void	Render_GameObject() override;
	virtual const RENDERID::ID& Get_RenderId() const { return RENDERID::BACKCOLLISION; }
	void		Shoot();
	void	Set_Remilia(CBossRemilia* _pRemilia);
private:
	virtual void	UpdateBodyCollision() override;
	virtual void	UpdateDie() override;

	void	Set_IsLeft(bool _bLeft);

private:
	bool	m_bLeft;
	float	m_fDieRandomSign;
	float	m_fRemainedDieTime;
	const float	m_fMaxDieTime;
	_vec3	m_vMoveTargetPos;
	CBossRemilia*	m_pRemilia;
};

