#include "stdafx.h"
#include "Remilia_Eye.h"
#include "BossRemilia.h"


CRemilia_Eye::CRemilia_Eye()
	:m_fMaxDieTime(1.f)
	, m_fDistanceFromRemilia(70.f)
{
}


CRemilia_Eye::~CRemilia_Eye()
{
}

CGameObject * CRemilia_Eye::Create(int idx, CBossRemilia * _pRemilia)
{
	CGameObject* pInstance = new CRemilia_Eye;
	static_cast<CRemilia_Eye*>(pInstance)->Set_Index( idx);
	switch (idx)
	{
	case 0:
		pInstance->Set_Angle(90.f);
		break;
	case 1:
		pInstance->Set_Angle(90.f +120.f);
		break;
	case 2:
		pInstance->Set_Angle(90.f -120.f);
		break;
	default:
		Safe_Delete(pInstance);
		return nullptr;
	}
	static_cast<CRemilia_Eye*>(pInstance)->Set_Remilia(_pRemilia);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Delete(pInstance);
	}
	return pInstance;
}

HRESULT CRemilia_Eye::Ready_GameObject()
{
	m_pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Remilia_Eye");
	m_tInfo.vSize = { 1.f,1.f,0.f };
	m_vecAnimation.emplace_back(CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->wstrObjectImage_ObjectKey + m_pObjectInfo->wstrIdleAnimImage_StateKey));
	m_vecBodyCollision.resize(1);
	m_fCurHp = m_pObjectInfo->fMaxHp;
	return S_OK;
}

int CRemilia_Eye::Update_GameObject()
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
			m_pRemilia->Set_EyeNullptr(m_iIdx);
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
	m_tInfo.fAngle += fDeltaTime * 30.f;
	m_tInfo.vPos = m_pRemilia->Get_Info().vPos + m_fDistanceFromRemilia * _vec3{ cosf(D3DXToRadian(-m_tInfo.fAngle)), sinf(D3DXToRadian(-m_tInfo.fAngle)), 0.f };

	
	return OBJ_NOEVENT;
}

void CRemilia_Eye::Late_Update_GameObject()
{
	UpdateBodyCollision();
	m_fAnimationCumulatedTime += CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_fAnimationCumulatedTime >= m_vecAnimation[0]->fPlay_Speed)
	{
		m_fAnimationCumulatedTime = 0;
		++m_uiAnimationFrame;
		if (m_uiAnimationFrame >= m_vecAnimation[0]->vecRect.size())
			m_uiAnimationFrame = 0;
	}

}

void CRemilia_Eye::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_vecAnimation[0]->wstrObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, m_tInfo.vSize.z);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_tInfo.fAngle + 90.f));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);
	matWorld = matScale * matRotZ * matTrans;
	const RECT& rect = m_vecAnimation[0]->vecRect[m_uiAnimationFrame];
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

void CRemilia_Eye::Set_Index(int idx)
{
	m_iIdx = idx;
}

void CRemilia_Eye::UpdateBodyCollision()
{
	float fSizeX = m_tInfo.vSize.x;
	float fSizeY = m_tInfo.vSize.y;
	float fReduceSizeLeft = 0.5f;
	float fReduceSizeRight = 0.5f;
	float fReduceSizeUp = 0.7f;
	float fReduceSizeDown = 0.7f;

	RECT rect = m_vecAnimation[0]->vecRect[0];

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

void CRemilia_Eye::UpdateDie()
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

void CRemilia_Eye::Set_Remilia(CBossRemilia * _pRemilia)
{
	m_pRemilia = _pRemilia;
}
