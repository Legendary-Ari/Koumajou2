#include "stdafx.h"
#include "Effect.h"


CEffect::CEffect()
	:m_iCurFrame(-1)
	,m_fTimeStack(0.f)
{
}


CEffect::~CEffect()
{
}

CGameObject * CEffect::Create(const ANIMATION * _tAnimationInfo, D3DXVECTOR3 _vPos)
{
	CGameObject* pInstance = new CEffect;
	static_cast<CEffect*>(pInstance)->Set_Prefab(_tAnimationInfo);
	pInstance->Set_Pos(_vPos);
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

HRESULT CEffect::Ready_GameObject()
{
	m_eRenderId = RENDERID::EFFECT;

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
		++m_iCurFrame;

		if (m_pAnimationInfo->vecRect.size() < m_iCurFrame)
		{
			m_iCurFrame = 0;
			if (!m_pAnimationInfo->bLoop)
				return OBJ_DESTROYED;
		}
			
	}
	

	return OBJ_NOEVENT;
}

void CEffect::Late_Update_GameObject()
{

}

void CEffect::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_pAnimationInfo->wstrObjectKey,m_pAnimationInfo->wstrStateKey,m_iCurFrame);
	if (nullptr == pTexInfo)
		return;
	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	matWorld = matScale * matRotZ * matTrans;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &m_pAnimationInfo->vecRect[m_iCurFrame], &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CEffect::Release_GameObject()
{
}
