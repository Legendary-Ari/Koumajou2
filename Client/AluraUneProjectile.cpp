#include "stdafx.h"
#include "AluraUneProjectile.h"


CAluraUneProjectile::CAluraUneProjectile()
{
}


CAluraUneProjectile::~CAluraUneProjectile()
{
	Release_GameObject();
}

CGameObject * CAluraUneProjectile::Create(const OBJECTINFO * _tObjectInfo, D3DXVECTOR3 _vPos)
{
	CGameObject* pInstance = new CAluraUneProjectile;
	pInstance->Set_Prefab(_tObjectInfo);
	pInstance->Set_Pos(_vPos);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CAluraUneProjectile::Ready_GameObject()
{

	m_vecAnimation.resize(1);

	const ANIMATION* pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + m_pObjectInfo->wstrIdleAnimImage_StateKey);
	if (!pAnim)
	{
		ERR_MSG(L"CAluraUneProjectile의 이미지를 찾지 못했습니다.");
		return E_FAIL;
	}
	m_vecAnimation[0] = pAnim;

	m_bFlying = true;
	m_tInfo.vSize = { 1.0f,1.0f,0.f };

	m_vecAttackCollision.resize(1);
	m_vecAttackCollision[0].eId = COLLISION::C_RECT;
	return S_OK;
}

int CAluraUneProjectile::Update_GameObject()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	UpdateAnimation();


	return OBJ_NOEVENT;
}

void CAluraUneProjectile::InitUpdate_GameObject()
{
}

void CAluraUneProjectile::Late_Update_GameObject()
{
	UpdateAttackCollision();
}

void CAluraUneProjectile::Render_GameObject()
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
	const RECT& rect = m_vecAnimation[0]->vecRect[0];
	float 	fCenterX = float(((rect.right - rect.left) * 0.5f));
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
	const RECT& tRenderRect = m_vecAnimation[0]->vecRect[m_uiAnimationFrame];
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &tRenderRect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	RenderCollision();
}

void CAluraUneProjectile::Release_GameObject()
{
}

void CAluraUneProjectile::UpdateAnimation()
{
	m_fAnimationCumulatedTime += CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_fAnimationCumulatedTime >= m_vecAnimation[0]->fPlay_Speed)
	{
		++m_uiAnimationFrame;
		if (m_uiAnimationFrame >= m_vecAnimation[0]->vecRect.size())
		{
			m_uiAnimationFrame = 0;
			m_bDestroyed = true;
		}
		m_fAnimationCumulatedTime = 0.f;
	}
}

void CAluraUneProjectile::UpdateAttackCollision()
{
	float fSizeX = m_tInfo.vSize.x;
	float fSizeY = m_tInfo.vSize.y;
	float fReduceSizeLeft = 0.25f;
	float fReduceSizeRight = 0.25f;
	float fReduceSizeUp = 1.0f;
	if (m_uiAnimationFrame < 3)
	{
		fReduceSizeUp = -1.0f;
	}
	else if (m_uiAnimationFrame == 3)
	{
		fReduceSizeUp = -0.5f;
	}
	else if (m_uiAnimationFrame == 4)
	{
		fReduceSizeUp = 0.0f;
	}
	else if (m_uiAnimationFrame == 5 || m_uiAnimationFrame == 6 || m_uiAnimationFrame == 7)
	{
		fReduceSizeUp = 1.0f;
	}
	else
	{
		fReduceSizeUp = -1.0f;
	}
	float fReduceSizeDown = 0.75f;

	RECT rect = m_vecAnimation[0]->vecRect[m_uiAnimationFrame];

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
