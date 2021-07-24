#pragma once
#include "GameObject.h"
class CRemilia_Eye;
class CRemilia_Arm;
class CBossRemilia :
	public CGameObject
{
private:
	enum STATE { IDLE, MOVE, ATTACK, SPELL, GUNGNIL, DIE, STATE_END };
	enum PATTERN { P_IDLE, P_PATTERN1, P_MOVE1, P_PATTERN2, P_MOVE2 , P_PATTERN3 , P_MOVE3, P_PATTERN4, P_MOVE4_1, P_PATTERN4_1, P_SPELL1, P_SPELL2, P_SPELL3, P_PATTERN_END };
	enum ARM {LEFT, RIGHT};
	enum B_ID {B_PATTERN1, B_END};
	enum RATIO {R_PATTERN2, R_PATTERN3, R_PATTERN4, R_SPELL, R_END};
	CBossRemilia();
public:
	virtual ~CBossRemilia();
public:
	static CGameObject* Create(const OBJECTINFO* _pObjectInfo);
public:
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void InitUpdate_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
	//const _vec3&		Get_Center() const { return m_vBulletCenter; }

	virtual void OnOverlaped(CGameObject* _pHitObject, _vec3 vHitPos) override;
	virtual const RENDERID::ID& Get_RenderId() const { return RENDERID::BACKCOLLISION; }
	void	Set_ArmNullptr(bool bIsLeft);
	void	Set_EyeNullptr(int idx);
protected:
	virtual void UpdatePattern();
	//virtual void UpdateActionCoolDown();
	virtual void UpdateDie() override;
	virtual void UpdateState() override;
	virtual void UpdateAnimation() override;
	virtual void UpdateBodyCollision() override;
private:
	void	Pattern1();
	void	Pattern1_Init();
	void	Pattern2_Init();
	void	Pattern2();
	void	Pattern3_Init();
	void	Pattern3();
	void	Pattern4_Init();
	void	Pattern4();
	void	Spell1_Init();
	void	Spell1();
	void	Spell2_Init();
	void	Spell2();
	void	Spell3_Init();
	void	Spell3();
private:
	bool	m_bHit;
	float	m_fHitRemainTime;
	float	m_fHitMaxTime;
	float	m_fRemainPatternTime;
	_vec3	m_vBulletCenter;
	_vec3	m_vDieDir;
	_vec3	m_vMoveTargetPos;
	float	m_fRemainDieTime;
	float	m_fRamainedDieTime;
	float	m_fMaxDieTime;
	

	vector<float>	m_vecPatternTime;
	PATTERN	m_ePrevPattern;
	PATTERN	m_eCurPattern;
	bool	m_bDonePattern[B_END];
	const float	m_fAtkRadio[R_END] = 
	{
		0.23f, // PATTERN2,
		0.01f, // PATTERN3
		1.5f,	// PATTERN4
		0.06f	// SPELL
	};
	
	UINT m_uiPattern2Mount;
	UINT m_uiPattern3Idx;
	UINT m_uiSpellIdx;
	const UINT m_uiMaxSpellIdx;
	_vec3 m_vPattern3Pos[12];
	vector<pair<int, int>> m_vecPattern3LazorIdx;
	const UINT	m_uiPattern3PosMount;
	float m_fAtkCoolTime;
	float m_fAtkCoolTime2;
	const float m_fCoolTimePattern3Bullet;
	CRemilia_Arm*	m_pArm[2];
	CRemilia_Eye*	m_pEye[3];
};

