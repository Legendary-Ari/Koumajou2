#include "stdafx.h"
#include "AluraUneGEffrect.h"


CAluraUneGEffrect::CAluraUneGEffrect()
{
}


CAluraUneGEffrect::~CAluraUneGEffrect()
{
	Release_GameObject();
}

CGameObject * CAluraUneGEffrect::Create(const ANIMATION * _tAnimationInfo, D3DXVECTOR3 _vPos)
{
	CGameObject* pInstance = new CAluraUneGEffrect;
	static_cast<CEffect*>(pInstance)->Set_Animation(_tAnimationInfo);
	pInstance->Set_Pos(_vPos);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CAluraUneGEffrect::Ready_GameObject()
{
	m_vecAnimation.resize(1);

	m_vecAnimation[0] = m_pAnimationInfo;
	
	return S_OK;
}

int CAluraUneGEffrect::Update_GameObject()
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

			return OBJ_DESTROYED;
			
		}
	}
	return OBJ_NOEVENT;
}

void CAluraUneGEffrect::InitUpdate_GameObject()
{
}

void CAluraUneGEffrect::Late_Update_GameObject()
{
}

void CAluraUneGEffrect::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_pAnimationInfo->wstrObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();
	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
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

void CAluraUneGEffrect::Release_GameObject()
{
}
