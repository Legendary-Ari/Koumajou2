#pragma once
#include "Gui.h"
class CWeaponSelectCursor :
	public CGui
{
private:
	CWeaponSelectCursor();
public:
	virtual ~CWeaponSelectCursor();
public:
	static	CGameObject* Create(const OBJECTINFO * _pObjectInfo);

	virtual HRESULT Ready_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
private:
	bool		IsSelected(UINT _uiCurSel);
private:
	const _vec3	m_vRingPos[CVSkill::SKILL_END + 1] = {
		{ 139.f, 240.f, 0.f }, // Knife
		{ 186.f, 260.f, 0.f }, // Clock
		{ 235.f, 240.f, 0.f }, // Alice
		{ 284.f, 260.f, 0.f },  // Meling
		{ 331.f, 240.f, 0.f }, // 69
		{ 380.f, 260.f, 0.f }, // Reimu
		{ 426.f, 240.f, 0.f },  // Flandre
		{ 476.f, 260.f, 0.f } // END
	};
	CVSkill* m_pVSkill[CVSkill::SKILL_END];
	UINT		m_uiCurSel;
	const _vec3 m_vSelectedPos[3] = {
		{ 101.f, 25.f, 0.f },
		{ 80.f, 75.f, 0.f },
		{ 33.f, 100.f, 0.f } 
	};
	int			m_iSelected[3];
	int			m_iSelectedIdx;
	bool		m_bSceneChange;
};

