#pragma once
#include "GameObject.h"
class CRemilia_Lazor :
	public CGameObject
{
private:
	CRemilia_Lazor();
public:
	virtual ~CRemilia_Lazor();
	static CGameObject * Create(const _vec3& vSrcPos, const _vec3& vDstPos);

	virtual HRESULT Ready_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
	void		Set_SrcPos(_vec3 m_vSrcPos);
	void		Set_DstPos(_vec3 m_vDstPos);
private:
	virtual void UpdateAttackCollision() override;
private:
	_vec3	m_vSrcPos;
	_vec3	m_vDstPos;
	const float m_fDelayTime;
	const float m_fLifeTime;
	float m_fSpawnTime;
	int		m_iCollisionSize;
};

