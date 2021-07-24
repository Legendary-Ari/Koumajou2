#include "stdafx.h"
#include "RemiliaBullet.h"

CRemiliaBullet::CRemiliaBullet()
{
}


CRemiliaBullet::~CRemiliaBullet()
{
}

CGameObject * CRemiliaBullet::Create(const OBJECTINFO * _tObjectInfo, const INFO & tInfo, float _fSpeed)
{
	CGameObject* pInstance = new CRemiliaBullet;
	pInstance->Set_Prefab(_tObjectInfo);
	pInstance->Set_Info(tInfo);
	static_cast<CRemiliaBullet*>(pInstance)->Set_Speed(_fSpeed);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

void CRemiliaBullet::OnBlockedTile(CGameObject * pHitObject, DIRECTION::ID _eId)
{
	m_bDestroyed = true;
}

void CRemiliaBullet::OnBlocked(CGameObject * pHitObject, DIRECTION::ID _eId)
{
}

void CRemiliaBullet::OnOverlaped(CGameObject * _pHitObject, _vec3 vHitPos)
{
}

HRESULT CRemiliaBullet::Ready_GameObject()
{
	m_vecTileCollision.resize(1);
	m_vecTileCollision[0].eId = COLLISION::C_SPHERE;
	m_vecAttackCollision = m_vecTileCollision;
	m_vecBodyCollision = m_vecAttackCollision;
	return S_OK;
}

void CRemiliaBullet::InitUpdate_GameObject()
{
}

int CRemiliaBullet::Update_GameObject()
{
	if (m_bDestroyed)
	{

		return OBJ_DESTROYED;
	}

	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();

	float fMapX = (float)CScroll_Manager::GetMapSizeX();
	float fMapY = (float)CScroll_Manager::GetMapSizeY();
	if (m_tInfo.vPos.x > fMapX + 50.f || m_tInfo.vPos.x < -50.f ||
		m_tInfo.vPos.y > fMapY + 50.f || m_tInfo.vPos.y < -50.f)
		return OBJ_DESTROYED;
	m_tInfo.vPos += m_tInfo.vDir * fDeltaTime * m_fSpeed;
	return OBJ_NOEVENT;
}

void CRemiliaBullet::Late_Update_GameObject()
{
	UpdateTileCollision();
	UpdateAttackCollision();
	UpdateBodyCollision();
}

void CRemiliaBullet::Render_GameObject()
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

void CRemiliaBullet::Release_GameObject()
{
}

void CRemiliaBullet::Set_Speed(float _fSpeed)
{
	m_fSpeed = _fSpeed;
}

void CRemiliaBullet::UpdateAttackCollision()
{
	m_vecAttackCollision = m_vecTileCollision;
}

void CRemiliaBullet::UpdateBodyCollision()
{
	m_vecBodyCollision = m_vecTileCollision;
}

void CRemiliaBullet::UpdateTileCollision()
{
	float fSizeX = m_tInfo.vSize.x;
	float fSizeY = m_tInfo.vSize.y;
	float fReduceSizeLeft;
	float fReduceSizeRight;
	float fReduceSizeUp;
	float fReduceSizeDown;
	if (m_pObjectInfo->wstrPrefabName == L"Remilia_SmallBullet")
	{
		fReduceSizeLeft = 0.1f;
		fReduceSizeRight = 0.1f;
		fReduceSizeUp = 0.1f;
		fReduceSizeDown = 0.1f;
	}
	else
	{
		fReduceSizeLeft = 0.5f;
		fReduceSizeRight = 0.5f;
		fReduceSizeUp = 0.5f;
		fReduceSizeDown = 0.5f;
	}


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
