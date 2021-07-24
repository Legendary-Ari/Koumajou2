#include "stdafx.h"
#include "Fade.h"


CFade::CFade()
{
}


CFade::~CFade()
{
}

CGameObject * CFade::Create(bool _bIn)
{
	CGameObject* pInstance = new CFade;
	static_cast<CFade*>(pInstance)->Set_FadeInOrOut(_bIn);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

void CFade::Set_FadeInOrOut(bool _bIn)
{
	m_bFade = _bIn;
}

HRESULT CFade::Ready_GameObject()
{
	CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::SINGLE_TEX, {{ 0,0,0,0 }}, L"../Image/black.png", L"Black");
	m_iAlpha = m_bFade ? 255 : 0;
	return S_OK;
}

int CFade::Update_GameObject()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	m_bFade ? (m_iAlpha -= 10) : (m_iAlpha += 10);
	if(m_iAlpha < 0 || m_iAlpha > 255)
		return OBJ_DESTROYED;

	return OBJ_NOEVENT;
}

void CFade::InitUpdate_GameObject()
{
}

void CFade::Late_Update_GameObject()
{
}

void CFade::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"Black");
	if (nullptr == pTexInfo)
		return;

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixTranslation(&matTrans, float(CLIENTCX>>1), float(CLIENTCY >> 1), 0.f);
	matWorld = matTrans;
	float 	fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float 	fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

}

void CFade::Release_GameObject()
{
}

const RENDERID::ID & CFade::Get_RenderId() const
{
	return RENDERID::UI2;
}
