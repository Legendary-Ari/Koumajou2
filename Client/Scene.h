#pragma once
class CScene abstract
{
public:
	explicit CScene();
	virtual ~CScene();
public:
	virtual HRESULT Ready_Scene()PURE;
	virtual void	Update_Scene()PURE;
	virtual void	InitUpdate_Scene()PURE;
	virtual void	Render_Scene()PURE;
	virtual void	Release_Scene()PURE;
	const _vec3&	Get_StartPos() const;
protected:
	_vec3		m_vStartPos;
	static BYTE m_ePlaying;
};