#pragma once
#include "GameObject.h"
class CVSkill abstract
	:public CGameObject
{
public:
	enum SKILL_ID { KNIFE, CLOCK, MEILING, ALICE, PATCHURI, REIMU, FLANDRE, SKILL_END };
public:
	explicit CVSkill();
	virtual ~CVSkill();
public:
	virtual void Use(const INFO& tInfo)PURE;
	virtual void Render_GameObject();
	inline virtual const RENDERID::ID& Get_RenderId() const { return RENDERID::UI2; }
	const UINT&		Get_Cost() const;
	void			Use_Cost();
protected:
	UINT		m_uiCost;
};

