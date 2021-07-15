#include "stdafx.h"
#include "SceneChanger.h"


CSceneChanger::CSceneChanger()
	:m_bChange(false)
{
}


CSceneChanger::~CSceneChanger()
{
}

CGameObject * CSceneChanger::Create(const _vec3& _SrcPos, const _vec3& _DstPos, CScene_Manager::ID _eId)
{
	CGameObject* pInstance = new CSceneChanger;
	static_cast<CSceneChanger*>(pInstance)->SetSrcPos(_SrcPos);
	static_cast<CSceneChanger*>(pInstance)->SetDstPos(_DstPos);
	static_cast<CSceneChanger*>(pInstance)->Set_SceneId(_eId);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CSceneChanger::Ready_GameObject()
{
	

	return S_OK;
}

void CSceneChanger::InitUpdate_GameObject()
{
	UpdateAttackCollision();
}

int CSceneChanger::Update_GameObject()
{
	return 0;
}

void CSceneChanger::Late_Update_GameObject()
{
	if (m_bChange)
	{
		CGameObject_Manager::Get_Instance()->Get_Player()->Set_Pos(m_vDstPos);
		_vec3 vScroll = m_vDstPos;
		vScroll.x -= float(CLIENTCX >> 1);
		vScroll.y -= float(CLIENTCY >> 1);
		CScroll_Manager::Force_Set_Scroll(vScroll);
		CScene_Manager::Get_Instance()->Change_Scene_Manager(m_eSceneId);
	}
		
}

void CSceneChanger::Render_GameObject()
{
	RenderCollision();
}

void CSceneChanger::Release_GameObject()
{
}

void CSceneChanger::OnOverlaped(CGameObject * _pHitObject, _vec3 vHitPos)
{
	return;
}

void CSceneChanger::UpdateAttackCollision()
{
	m_vecAttackCollision.resize(1);
	m_vecAttackCollision[0].tFRect =
	{
		m_vSrcPos.x - 25.f,
		m_vSrcPos.y - 100.f,
		m_vSrcPos.x + 25.f,
		m_vSrcPos.y + 100.f
	};
}

const RENDERID::ID & CSceneChanger::Get_RenderId() const
{
	return RENDERID::UI;
}

void CSceneChanger::Set_SceneId(CScene_Manager::ID _eId)
{
	m_eSceneId = _eId;
}

void CSceneChanger::SetSrcPos(const _vec3& _SrcPos)
{
	m_vSrcPos = _SrcPos;
}

void CSceneChanger::SetDstPos(const _vec3& _DstPos)
{
	m_vDstPos = _DstPos;
}

void CSceneChanger::SetSceneId(CScene_Manager::ID _eId)
{
	m_eSceneId = _eId;
}

void CSceneChanger::Set_Change()
{
	m_bChange = true;
}
