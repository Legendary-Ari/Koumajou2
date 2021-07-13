#include "stdafx.h"
#include "Effect.h"


CEffect::CEffect()
	:m_fTimeStack(0.f)
{
}


CEffect::~CEffect()
{
}

CGameObject * CEffect::Create(const ANIMATION * _tAnimationInfo, D3DXVECTOR3 _vPos, D3DXVECTOR3 _vDir)
{
	CGameObject* pInstance = new CEffect;
	static_cast<CEffect*>(pInstance)->Set_Prefab(_tAnimationInfo);
	pInstance->Set_Pos(_vPos);
	static_cast<CEffect*>(pInstance)->Set_Dir(_vDir);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

void CEffect::Set_Prefab(const ANIMATION * _pPrefab)
{
	m_pAnimationInfo = _pPrefab;
}

void CEffect::Set_Dir(_vec3 _vDir)
{
	m_tInfo.vDir = _vDir;
}

HRESULT CEffect::Ready_GameObject()
{


	return S_OK;
}

int CEffect::Update_GameObject()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	m_fTimeStack += CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_pAnimationInfo->fPlay_Speed <= m_fTimeStack)
	{
		m_fTimeStack = 0.f;
		++m_uiAnimationFrame;

		if (m_pAnimationInfo->vecRect.size() <= m_uiAnimationFrame)
		{
			m_uiAnimationFrame = 0;
			if (!m_pAnimationInfo->bLoop)
				return OBJ_DESTROYED;
		}
			
	}
	m_tInfo.vPos += m_tInfo.vDir;

	return OBJ_NOEVENT;
}

void CEffect::InitUpdate_GameObject()
{
}

void CEffect::Late_Update_GameObject()
{

}

void CEffect::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_pAnimationInfo->wstrObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();
	if (m_tInfo.vDir.x < 0)
		m_bFliped = true;
	else
		m_bFliped = false;
	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, (m_bFliped ? -1.0f : 1.0f), 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_tInfo.fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);
	matWorld = matScale * matRotZ * matTrans;
	const RECT& rect = m_pAnimationInfo->vecRect[0];
	float 	fCenterX = float(((rect.right - rect.left) * 0.5f));
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
	const RECT& tRenderRect = m_pAnimationInfo->vecRect[m_uiAnimationFrame];
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &tRenderRect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CEffect::Release_GameObject()
{
}

const RENDERID::ID & CEffect::Get_RenderId() const
{
	return RENDERID::EFFECT;
}
