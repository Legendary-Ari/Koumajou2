#pragma once
#include "Gui.h"
class CNumberUi final:
	public CGui
{
public:
	enum ID {BIG, NORMAL, SMALL};
private:
	explicit CNumberUi();
	virtual ~CNumberUi();

private:
	virtual HRESULT Ready_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
public:
	static void RenderNumber(UINT _ui, CNumberUi::ID _eId, const _vec3& _vPos);
};
