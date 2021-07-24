#include "stdafx.h"
#include "Remilia_Lazor.h"


CRemilia_Lazor::CRemilia_Lazor()
	:m_fDelayTime(1.2f)
	,m_fSpawnTime(0.f)
	,m_fLifeTime(2.0f)
{
}


CRemilia_Lazor::~CRemilia_Lazor()
{
	Release_GameObject();
}

CGameObject * CRemilia_Lazor::Create(const _vec3& _vSrcPos, const _vec3& _vDstPos)
{
	CGameObject* pInstance = new CRemilia_Lazor;
	static_cast<CRemilia_Lazor*>(pInstance)->Set_SrcPos(_vSrcPos);
	static_cast<CRemilia_Lazor*>(pInstance)->Set_DstPos(_vDstPos);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CRemilia_Lazor::Ready_GameObject()
{
	m_pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Remilia_Lazer");
	m_tInfo.vPos = m_vSrcPos;
	_vec3 vDiff = m_vDstPos - m_vSrcPos;
	m_iCollisionSize = (int)(D3DXVec3Length(&vDiff) / 10.f);
	m_vecAttackCollision.resize(m_iCollisionSize);
	for (int i = 0; i < m_iCollisionSize; ++i)
	{
		m_vecAttackCollision[i].eId = COLLISION::C_SPHERE;
	}
	m_tInfo.fAngle = -D3DXToDegree(atan2f(vDiff.y, vDiff.x));

	m_tInfo.vSize.x = m_iCollisionSize / 27.f;
	m_tInfo.vSize.y = 0.1f;
	m_tInfo.vSize.z = 0;

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSS_BULLET3);
	CSoundMgr::Get_Instance()->PlaySound(L"062.wav", CSoundMgr::BOSS_BULLET3);

	return S_OK;
}

void CRemilia_Lazor::InitUpdate_GameObject()
{
}

int CRemilia_Lazor::Update_GameObject()
{
	m_fSpawnTime += CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_fSpawnTime > m_fLifeTime)
		return OBJ_DESTROYED;

	
	if (m_fSpawnTime < m_fDelayTime)
	{
		m_tInfo.vSize.y = 0.1f;
	}
	else
	{
		if (m_tInfo.vSize.y == 0.1f)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSS_BULLET2);
			CSoundMgr::Get_Instance()->PlaySound(L"052.wav", CSoundMgr::BOSS_BULLET2);
			
		}
		m_tInfo.vSize.y = 0.4f;
	}

	return OBJ_NOEVENT;
}

void CRemilia_Lazor::Late_Update_GameObject()
{
	if (m_fSpawnTime < m_fDelayTime)
	{
		return;
	}
	
	UpdateAttackCollision();
}

void CRemilia_Lazor::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_pObjectInfo->wstrObjectImage_ObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

	D3DXMATRIX matScale, matTrans, matRotZ, matWorld;
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, m_tInfo.vSize.z);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_tInfo.fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);
	matWorld = matScale * matRotZ * matTrans;
	const RECT& rect = m_pObjectInfo->tRect;
	float 	fCenterX = 0;
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	RenderCollision();
}

void CRemilia_Lazor::Release_GameObject()
{
}

void CRemilia_Lazor::Set_SrcPos(_vec3 _vSrcPos)
{
	m_vSrcPos = _vSrcPos;
}

void CRemilia_Lazor::Set_DstPos(_vec3 _vDstPos)
{
	m_vDstPos = _vDstPos;
}

void CRemilia_Lazor::UpdateAttackCollision()
{
	for (int i = 0; i < m_iCollisionSize; ++i)
	{
		_vec3 vDiff = m_vDstPos - m_vSrcPos;

		_vec3 vTemp = m_vSrcPos + vDiff * (float(i) / float(m_iCollisionSize));

		m_vecAttackCollision[i].tFRect =
		{
			vTemp.x - 5.f,
			vTemp.y - 5.f,
			vTemp.x + 5.f,
			vTemp.y + 5.f
		};

	}
}
