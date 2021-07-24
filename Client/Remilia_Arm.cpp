#include "stdafx.h"
#include "Remilia_Arm.h"
#include "BossRemilia.h"
#include "RemiliaBullet.h"

CRemilia_Arm::CRemilia_Arm()
	: m_fMaxDieTime(0.5f)
{
}


CRemilia_Arm::~CRemilia_Arm()
{
}

CGameObject * CRemilia_Arm::Create(bool _bLeft, CBossRemilia* _pRemilia)
{
	CGameObject* pInstance = new CRemilia_Arm;
	static_cast<CRemilia_Arm*>(pInstance)->Set_IsLeft(_bLeft);
	static_cast<CRemilia_Arm*>(pInstance)->Set_Remilia(_pRemilia);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Delete(pInstance);
	}
	return pInstance;
}

HRESULT CRemilia_Arm::Ready_GameObject()
{
	m_pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Remilia_Arm");
	ZeroMemory(&m_tInfo, sizeof(INFO));
	m_vMoveTargetPos = {
		0.f,
		m_pRemilia->Get_Info().vPos.y - (CScroll_Manager::GetMapSizeY() - m_pRemilia->Get_Info().vPos.y) * 0.5f,
		0.f
	};
	if (m_bLeft)
	{
		m_vMoveTargetPos.x = m_pRemilia->Get_Info().vPos.x * 0.25f;
	}
	else
	{
		m_vMoveTargetPos.x = m_pRemilia->Get_Info().vPos.x + (CScroll_Manager::GetMapSizeX() - m_pRemilia->Get_Info().vPos.x) * 0.75f;
	}
	m_bFliped = !m_bLeft;
	m_tInfo.vPos = m_vMoveTargetPos;
	m_tInfo.vSize = { (m_bFliped ? -1.f : 1.f), 1.f, 0.f };
	m_vecBodyCollision.resize(1);
	m_fCurHp = m_pObjectInfo->fMaxHp;
	return S_OK;
}

int CRemilia_Arm::Update_GameObject()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;
	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_bDead)
	{
		if (m_bDieInit)
		{
			float fAngle = D3DXToRadian(rand());
			m_fDieRandomSign = cosf(fAngle);
			m_tInfo.vDir.y = -5.f;
			m_bDieInit = false;
			m_pRemilia->Set_ArmNullptr(!m_bFliped);
		}
		UpdateDie();
		m_tInfo.vPos += m_tInfo.vDir;
		return OBJ_NOEVENT;
	}
	if (m_bHit)
	{
		m_fHitCumulatedTime += fDeltaTime;
		if (m_fHitCumulatedTime >= m_fHitMaxTime)
		{
			m_bHit = false;
			m_fHitCumulatedTime = 0;
		}
	}
	if (m_bLeft)
	{
		m_vMoveTargetPos.x = m_pRemilia->Get_Info().vPos.x * 0.25f;
	}
	else
	{
		m_vMoveTargetPos.x = m_pRemilia->Get_Info().vPos.x + (CScroll_Manager::GetMapSizeX() - m_pRemilia->Get_Info().vPos.x) * 0.75f;
	}
	
	m_vMoveTargetPos.y = m_pRemilia->Get_Info().vPos.y + (float(CScroll_Manager::GetMapSizeY()) - m_pRemilia->Get_Info().vPos.y) * 0.5f;
	_vec3 vDirr = m_vMoveTargetPos - m_tInfo.vPos;
	if (D3DXVec3Length(&vDirr) > 10.f)
	{
		D3DXVec3Normalize(&m_tInfo.vDir, &vDirr);
	}
	else
	{
		ZeroMemory(&m_tInfo.vDir, sizeof(_vec3));
	}
	
	m_tInfo.vPos += m_tInfo.vDir * 30.f * fDeltaTime;
	
	return OBJ_NOEVENT;
}

void CRemilia_Arm::Late_Update_GameObject()
{
	UpdateBodyCollision();
}

void CRemilia_Arm::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_pObjectInfo->wstrObjectImage_ObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, m_tInfo.vSize.z);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);
	matWorld = matScale * matTrans;
	const RECT& rect = m_pObjectInfo->tRect;
	float 	fCenterX = float(((rect.right - rect.left)*0.5f));
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	RenderCollision();

	if (m_bDead)
	{
		_vec3 vRandom(m_tInfo.vPos);
		vRandom.x += rand() % 50 - 25.f;
		vRandom.y += rand() % 50 - 25.f;
		RenderDieEffect(vRandom);
	}
}

void CRemilia_Arm::Shoot()
{
	{
		const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Remilia_BigBullet");
		INFO tInfo;
		tInfo.vPos = m_tInfo.vPos;
		_vec3 vDiff = CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info().vPos - m_tInfo.vPos;
		D3DXVec3Normalize(&tInfo.vDir, &vDiff);
		tInfo.vSize = { 1.5f,1.5f,0.0f };
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, CRemiliaBullet::Create(pObjectInfo, tInfo, 170.f));
	}
	for(int i=0; i<2; ++i)
	{
		const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Remilia_MiddleBullet");
		INFO tInfo;
		tInfo.vPos = m_tInfo.vPos + _vec3{0.f,(rand() % 10 - 5.f) ,0.f};
		_vec3 vDiff = CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info().vPos - m_tInfo.vPos;
		D3DXVec3Normalize(&tInfo.vDir, &vDiff);
		tInfo.vSize = { 1.0f,1.0f,0.0f };
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, CRemiliaBullet::Create(pObjectInfo, tInfo, 150.f - i));
	}
	for (int i = 0; i<6; ++i)
	{
		const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Remilia_SmallBullet");
		INFO tInfo;
		tInfo.vPos = m_tInfo.vPos + _vec3{ 0.f,(rand() % 10 - 5.f) ,0.f };
		_vec3 vDiff = CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info().vPos - m_tInfo.vPos;
		D3DXVec3Normalize(&tInfo.vDir, &vDiff);
		tInfo.vSize = { 1.0f,1.0f,0.0f };
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, CRemiliaBullet::Create(pObjectInfo, tInfo, 140.f - i * 2.f));
	}

}

void CRemilia_Arm::UpdateBodyCollision()
{
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
	m_vecBodyCollision[0].tFRect =
	{
		(float)(m_tInfo.vPos.x - v2Radius.x * m_tInfo.vSize.x * fReduceSizeLeft),
		(float)(m_tInfo.vPos.y - v2Radius.y * m_tInfo.vSize.y * fReduceSizeUp),
		(float)(m_tInfo.vPos.x + v2Radius.x * m_tInfo.vSize.x * fReduceSizeRight),
		(float)(m_tInfo.vPos.y + v2Radius.y * m_tInfo.vSize.y * fReduceSizeDown)
	};
}

void CRemilia_Arm::UpdateDie()
{
	float fDieRotateSpeed = 100.f;
	m_tInfo.vDir.y += 27.8f * CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_tInfo.fAngle += m_fDieRandomSign * fDieRotateSpeed * CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_fRemainedDieTime -= CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_fRemainedDieTime < 0)
	{
		m_bDestroyed = true;
	}
}

void CRemilia_Arm::Set_IsLeft(bool _bLeft)
{
	m_bLeft = _bLeft;
}

void CRemilia_Arm::Set_Remilia(CBossRemilia * _pRemilia)
{
	m_pRemilia = _pRemilia;
}
