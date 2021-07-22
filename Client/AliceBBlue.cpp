#include "stdafx.h"
#include "AliceBBlue.h"
#include "BossAlice.h"

int CAliceBBlue::m_pSoundIdx = 0;
CAliceBBlue::CAliceBBlue()
	:m_fAcceleration(0.f)
{
}


CAliceBBlue::~CAliceBBlue()
{
}

CGameObject * CAliceBBlue::Create(const OBJECTINFO * _tObjectInfo, const INFO& tInfo)
{
	CGameObject* pInstance = new CAliceBBlue;
	pInstance->Set_Prefab(_tObjectInfo);
	pInstance->Set_Info(tInfo);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

void CAliceBBlue::OnBlockedTile(CGameObject * pHitObject, DIRECTION::ID _eId)
{
	m_bDestroyed = true;
}

void CAliceBBlue::OnBlocked(CGameObject * pHitObject, DIRECTION::ID _eId)
{
	
}

void CAliceBBlue::OnOverlaped(CGameObject* _pHitObject, _vec3 vHitPos)
{
	
}

HRESULT CAliceBBlue::Ready_GameObject()
{

	m_vecTileCollision.resize(1);
	m_vecTileCollision[0].eId = COLLISION::C_SPHERE;
	m_vecAttackCollision = m_vecTileCollision;
	m_vecBodyCollision = m_vecAttackCollision;
	m_fAcceleration = 0.f;
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNELID(CSoundMgr::BOSS_BULLET1 + m_pSoundIdx));
		CSoundMgr::Get_Instance()->PlaySound(L"060.wav", CSoundMgr::CHANNELID(CSoundMgr::BOSS_BULLET1 + m_pSoundIdx));
		++m_pSoundIdx;
		if (m_pSoundIdx >= 3)
			m_pSoundIdx = 0;
	}
	
	return S_OK;
}

void CAliceBBlue::InitUpdate_GameObject()
{
}

int CAliceBBlue::Update_GameObject()
{
	if (m_bDestroyed)
	{
		
		return OBJ_DESTROYED;
	}

	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();

	float fMapX = (float)CScroll_Manager::GetMapSizeX();
	float fMapY = (float)CScroll_Manager::GetMapSizeY();
	if(m_tInfo.vPos.x > fMapX || m_tInfo.vPos.x < 0 ||
		m_tInfo.vPos.y > fMapY || m_tInfo.vPos.y < 0)
		return OBJ_DESTROYED;
	m_fAcceleration += fDeltaTime * 200.f;
	m_tInfo.vPos += m_tInfo.vDir * fDeltaTime * m_fAcceleration;
	return OBJ_NOEVENT;
}

void CAliceBBlue::Late_Update_GameObject()
{
	UpdateTileCollision();
	UpdateAttackCollision();
	UpdateBodyCollision();
}

void CAliceBBlue::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_pObjectInfo->wstrObjectImage_ObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_tInfo.fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);
	matWorld = matScale * matRotZ * matTrans;
	const RECT& rect = m_pObjectInfo->tRect;
	float 	fCenterX = float(((rect.right - rect.left)*0.5f));
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	RenderCollision();
}

void CAliceBBlue::Release_GameObject()
{
}

void CAliceBBlue::UpdateAttackCollision()
{
	m_vecAttackCollision = m_vecTileCollision;
}

void CAliceBBlue::UpdateBodyCollision()
{
	m_vecBodyCollision = m_vecTileCollision;
}

void CAliceBBlue::UpdateTileCollision()
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
	m_vecTileCollision[0].tFRect =
	{
		(float)(m_tInfo.vPos.x - v2Radius.x * m_tInfo.vSize.x * fReduceSizeLeft),
		(float)(m_tInfo.vPos.y - v2Radius.y * m_tInfo.vSize.y * fReduceSizeUp),
		(float)(m_tInfo.vPos.x + v2Radius.x * m_tInfo.vSize.x * fReduceSizeRight),
		(float)(m_tInfo.vPos.y + v2Radius.y * m_tInfo.vSize.y * fReduceSizeDown)
	};
}
