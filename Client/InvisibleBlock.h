#pragma once
#include "BackCollision.h"
class CInvisibleBlock :
	public CGameObject
{
private:
	CInvisibleBlock();
public:
	virtual ~CInvisibleBlock();
public:
	static CGameObject* Create(const INFO& _tInfo);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
	void UpdateTileCollision();
	virtual const RENDERID::ID& Get_RenderId() const { return RENDERID::BACKCOLLISION; }
};

