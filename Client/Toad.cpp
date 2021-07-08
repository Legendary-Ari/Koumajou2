#include "stdafx.h"
#include "Toad.h"
CToad::CToad()
{
}


CToad::~CToad()
{
}

CGameObject * CToad::Create(const ACTORINFO * _pActorInfo, const OBJECTINFO * _pObjectInfo)
{
	CGameObject* pInstance = new CToad;
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

HRESULT CToad::Ready_GameObject()
{
	if (FAILED(CEnemy::Ready_GameObject()))
		return E_FAIL;
	m_vecAnimation.resize(STATE_END);

	const ANIMATION* pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->cstrIdleAnimImage_ObjectKey + m_pObjectInfo->wstrIdleAnimImage_StateKey);
	if (!pAnim)
	{
		ERR_MSG(L"플레이어의 이미지를 찾지 못했습니다.1");
		return E_FAIL;
	}
	m_vecAnimation[IDLE] = pAnim;

	m_vecBodyCollision.resize(1);
	m_vecBodyCollision[0].eId = COLLISION::C_RECT;
	return S_OK;
}

int CToad::Update_GameObject()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;


	UpdateBodyCollision();
	return OBJ_NOEVENT;
}

void CToad::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_vecAnimation[0]->wstrObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, (m_bIsFliped ? -1.0f : 1.0f), 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_tInfo.fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);
	matWorld = matScale * matRotZ * matTrans;
	const RECT& rect = m_vecAnimation[IDLE]->vecRect[0];
	float 	fCenterX = float(((rect.right - rect.left) * 0.25f));
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
	const RECT& tRenderRect = m_vecAnimation[IDLE]->vecRect[m_uiAnimationFrame];
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &tRenderRect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	RenderCollision();
}

void CToad::UpdateBodyCollision()
{
	float fSize = m_tInfo.vSize.x;
	float fReduceSizeLeft = -0.2f;
	float fReduceSizeRight = 0.7f;
	float fReduceSizeUp = 0.f;
	float fReduceSizeDown = 1.f;

	RECT rect = m_vecAnimation[0]->vecRect[0];

	_vec2 v2Radius = { (float)((rect.right - rect.left) * 0.5f), (float)((rect.bottom - rect.top) * 0.5f) };
	v2Radius *= fSize;
	m_vecBodyCollision[0].tRect =
	{
		(LONG)(m_tInfo.vPos.x - v2Radius.x * fReduceSizeLeft),
		(LONG)(m_tInfo.vPos.y - v2Radius.y * fReduceSizeUp),
		(LONG)(m_tInfo.vPos.x + v2Radius.x * fReduceSizeRight),
		(LONG)(m_tInfo.vPos.y + v2Radius.y * fReduceSizeDown)
	};
}
