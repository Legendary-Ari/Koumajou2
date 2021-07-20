#include "stdafx.h"
#include "Rose.h"
#include "RoseStem.h"
#include "RoseHead.h"
#include "Spawn_Manager.h"

CRose::CRose()
	:m_uiRoseStemMount(7)
{
}


CRose::~CRose()
{
}

CGameObject * CRose::Create(const ACTORINFO * _pActorInfo, const OBJECTINFO * _pObjectInfo)
{
	CGameObject* pInstance = new CRose;
	pInstance->Set_Prefab(_pObjectInfo);
	pInstance->Set_ActorInfo(_pActorInfo);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CRose::Ready_GameObject()
{
	if (FAILED(CEnemy::Ready_GameObject()))
		return E_FAIL;

	m_vecAttackCollision.resize(1);
	m_vecAttackCollision[0].eId = COLLISION::C_RECT;
	//CGameObject_Manager::Add_GameObject_Manager()
	m_vecRoseParts.resize(m_uiRoseStemMount+1);
	for (int i = 0; i < m_uiRoseStemMount; ++i)
	{
		const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Rose_Stem");
		INFO tInfo;
		ZeroMemory(&tInfo, sizeof(INFO));
		tInfo.vPos.x = m_tInfo.vPos.x + i * 5.f;
		tInfo.vPos.y = m_tInfo.vPos.y + i * -10.f;
		tInfo.vSize = { 1.0f,1.0f,0.f };
		tInfo.fAngle = i * 10.f;
		m_vecRoseParts[i] = static_cast<CEnemy*>(CRoseStem::Create(tInfo, pObjectInfo,this, i * 4.f));
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, m_vecRoseParts[i]);
	}
	{
		const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Rose_Head");
		INFO tInfo;
		ZeroMemory(&tInfo, sizeof(INFO));
		tInfo.vPos.x = m_tInfo.vPos.x + m_uiRoseStemMount * 5.f;
		tInfo.vPos.y = m_tInfo.vPos.y + m_uiRoseStemMount * -10.f;
		tInfo.vSize = { 1.0f,1.0f,0.f };
		tInfo.fAngle = m_uiRoseStemMount * 10.f;
		m_vecRoseParts[m_uiRoseStemMount] = static_cast<CEnemy*>(CRoseHead::Create(tInfo , pObjectInfo, this, m_uiRoseStemMount * 4.f));
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, m_vecRoseParts[m_uiRoseStemMount]);
	}

	m_uiChi = 60;

	return S_OK;
}

void CRose::InitUpdate_GameObject()
{
	if (m_tInfo.vPos.x > CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info().vPos.x)
	{
		m_bFliped = true;
	}
	else
	{
		m_bFliped = false;
	}
}

int CRose::Update_GameObject()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;
	if (!UpdateActive())
		return OBJ_NOEVENT;
	for (auto& pRostParts : m_vecRoseParts)
	{
		pRostParts->Set_Active(true);
	}
	if (m_bDead)
	{
		if (m_bDieInit)
		{
			ZeroMemory(&m_vecAttackCollision[0].tFRect, sizeof(FRECT));
			m_bDieInit = false;
		}
		return OBJ_DESTROYED;
	}
	if (m_bHit)
	{
		m_fHitCumulatedTime += CTime_Manager::Get_Instance()->Get_DeltaTime();
		if (m_fHitCumulatedTime >= m_fHitMaxTime)
		{
			m_bHit = false;
			m_fHitCumulatedTime = 0;
		}
		else
		{
			return OBJ_NOEVENT;
		}
	}


	UpdateAttackCollision();
	return OBJ_NOEVENT;
}

void CRose::Render_GameObject()
{
	CEnemy::Render_GameObject();
	RenderCollision();
}

void CRose::Set_Hit(bool _bHit)
{
	for (int i=0; i<m_uiRoseStemMount-1; ++i)
	{
		m_vecRoseParts[i]->Set_Hit(true);
	}
		
}

void CRose::Set_Die()
{
	for (UINT i=0; i<m_vecRoseParts.size()-1; ++i)
		m_vecRoseParts[i]->Set_Die();
	m_bDieInit = true;
	m_bDead = true;
}

void CRose::UpdateAttackCollision()
{
	if (!m_bDead)
		return;
	float fSizeX = m_tInfo.vSize.x;
	float fSizeY = m_tInfo.vSize.y;
	float fReduceSizeLeft = 0.5f;
	float fReduceSizeRight = 0.5f;
	float fReduceSizeUp = 0.5f;
	float fReduceSizeDown = 0.5f;

	RECT rect = m_pObjectInfo->tRect;

	_vec2 v2Radius = { (float)((rect.right - rect.left) * 0.5f), (float)((rect.bottom - rect.top) * 0.5f) };
	v2Radius.x *= fSizeX;
	v2Radius.y *= fSizeY;
	m_vecAttackCollision[0].tFRect =
	{
		(float)(m_tInfo.vPos.x - v2Radius.x * m_tInfo.vSize.x * fReduceSizeLeft),
		(float)(m_tInfo.vPos.y - v2Radius.y * m_tInfo.vSize.y * fReduceSizeUp),
		(float)(m_tInfo.vPos.x + v2Radius.x * m_tInfo.vSize.x * fReduceSizeRight),
		(float)(m_tInfo.vPos.y + v2Radius.y * m_tInfo.vSize.y * fReduceSizeDown)
	};
}
