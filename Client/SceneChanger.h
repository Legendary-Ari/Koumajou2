#pragma once
#include "GameObject.h"
class CSceneChanger :
	public CGameObject
{
private:
	CSceneChanger();
public:
	virtual ~CSceneChanger();

	static CGameObject* Create(const _vec3& _SrcPos, const _vec3& _DstPos, CScene_Manager::ID _eId);

	virtual HRESULT Ready_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
public:
	virtual void	OnOverlaped(CGameObject* _pHitObject, _vec3 vHitPos) override;
	virtual void	UpdateAttackCollision() override;
	virtual const RENDERID::ID& Get_RenderId() const override;
	void			Set_SceneId(CScene_Manager::ID _eId);
public:
	void	SetSrcPos(const _vec3& _SrcPos);
	void	SetDstPos(const _vec3& _DstPos);
	void	SetSceneId(CScene_Manager::ID _eId);
	void	Set_Change();
private:
	_vec3	m_vSrcPos;
	_vec3	m_vDstPos;
	CScene_Manager::ID m_eSceneId;
	bool	m_bChange;
};

