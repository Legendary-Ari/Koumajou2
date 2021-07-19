#include "stdafx.h"
#include "StageUi.h"
#include "Player.h"

CStageUi::CStageUi()
	:m_uiIconMount(3)
{
}


CStageUi::~CStageUi()
{
}

CGameObject * CStageUi::Create()
{
	CGameObject* pInstance = new CStageUi;
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;

}

HRESULT CStageUi::Ready_GameObject()
{
	m_tInfo.vPos = { 154.f, 36.f, 0.f };
	m_tHpBarInfo.vPos = { 115.f, 20.0f, 0.f };
	m_tHpBarInfo.vSize = { 6.f,0.75f,0.f };
	m_tMPBarInfo.vPos = { 117.f, 28.0f, 0.f };
	m_tMPBarInfo.vSize = { 0.75f,0.75f,0.f };
	m_vRingInfo.vPos = { 0.f, 0.f, 0.f };
	m_vRingInfo.vSize = { 0.75f, 0.75f, 0.f };
	for (int i = 0; i < 3; ++i)
	{
		m_vIconInfo[i].vPos = { 40.f, 0.f, 0.f };
		m_vIconInfo[i].fAngle = 0.f + (45.f * i);
		m_vIconInfo[i].vSize = { 0.5f, 0.5f, 1.f };
	}
	
	
	m_pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"StageUi");
	if (!m_pObjectInfo)
	{
		ERR_MSG(L"StageUi 못찾았습니다");
		return E_FAIL;
	}
	return S_OK;
}
void CStageUi::InitUpdate_GameObject()
{
	m_pCurHp = static_cast<const CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_HpPointer();
	m_pCurMp = static_cast<const CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_MpPointer();
}

int CStageUi::Update_GameObject()
{
	return 0;
}

void CStageUi::Late_Update_GameObject()
{
	m_tHpBarInfo.vSize.x = 6.f * 0.01f * (*m_pCurHp);
	if (m_tHpBarInfo.vSize.x < 0)
		m_tHpBarInfo.vSize.x = 0;
	m_tMPBarInfo.vSize.x = 0.75f * 0.01f * (*m_pCurMp);
	if (m_tMPBarInfo.vSize.x < 0)
		m_tMPBarInfo.vSize.x = 0;
}

void CStageUi::Render_GameObject()
{

	
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_pObjectInfo->wstrObjectImage_ObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.75f, 0.75f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	matWorld = matScale * matTrans;
	const RECT& rect = m_pObjectInfo->tRect;
	float 	fCenterX = float(((rect.right - rect.left)*0.5f));
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	RenderHp();
	RenderMp();
	RenderRing();
	RenderIcon();
	RenderChiText();
	RenderLifeText();
}


void CStageUi::Release_GameObject()
{
}

void CStageUi::RenderHp()
{
	const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"HpBar");
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(pObjectInfo->wstrObjectImage_ObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, m_tHpBarInfo.vSize.x, m_tHpBarInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tHpBarInfo.vPos.x, m_tHpBarInfo.vPos.y, 0.f);
	matWorld = matScale * matTrans;
	const RECT& rect = pObjectInfo->tRect;
	float 	fCenterX = 0.f;
	float 	fCenterY = 0.f;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CStageUi::RenderMp()
{

	const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"MpBar");
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(pObjectInfo->wstrObjectImage_ObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, m_tMPBarInfo.vSize.x, m_tMPBarInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tMPBarInfo.vPos.x, m_tMPBarInfo.vPos.y, 0.f);
	matWorld = matScale * matTrans;
	const RECT& rect = pObjectInfo->tRect;
	float 	fCenterX = 0.f;
	float 	fCenterY = 0.f;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CStageUi::RenderRing()
{

	const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"StageWeaponRing");
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(pObjectInfo->wstrObjectImage_ObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, m_vRingInfo.vSize.x, m_vRingInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_vRingInfo.vPos.x, m_vRingInfo.vPos.y, 0.f);
	matWorld = matScale * matTrans;
	const RECT& rect = pObjectInfo->tRect;
	float 	fCenterX = float(((rect.right - rect.left)*0.5f));
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CStageUi::RenderIcon()
{
	for (UINT i = 0; i < m_uiIconMount; ++i)
	{
		const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"StageWeaponIcon");
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(pObjectInfo->wstrObjectImage_ObjectKey);
		if (nullptr == pTexInfo)
			return;
		D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

		D3DXMATRIX matScale, matRotZ, matTrans, matOrbit, matWorld;
		D3DXMatrixScaling(&matScale, m_vIconInfo[i].vSize.x, m_vIconInfo[i].vSize.y, 0.f);
		D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_vIconInfo[i].fAngle));
		D3DXMatrixTranslation(&matTrans, m_vIconInfo[i].vPos.x, m_vIconInfo[i].vPos.y, 0.f);
		D3DXMatrixRotationZ(&matOrbit, D3DXToRadian(m_vIconInfo[i].fAngle));
		matWorld = matScale * matRotZ * matTrans * matOrbit;
		const RECT& rect = pObjectInfo->tRect;
		float 	fCenterX = float(((rect.right - rect.left)*0.5f));
		float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	}

}

void CStageUi::RenderChiText()
{
	//TCHAR str[32];
	//GetPrivateProfileString(L"Pos", L"x", L"", str, sizeof(str), L"../Temp.ini");
	//m_vIconInfo.vPos.x = _ttof(str);
	//GetPrivateProfileString(L"Pos", L"y", L"", str, sizeof(str), L"../Temp.ini");
	//m_vIconInfo.vPos.y = _ttof(str);
	//GetPrivateProfileString(L"Size", L"x", L"", str, sizeof(str), L"../Temp.ini");
	//m_vIconInfo.vSize.x = _ttof(str);
	//GetPrivateProfileString(L"Size", L"y", L"", str, sizeof(str), L"../Temp.ini");
	//m_vIconInfo.vSize.y = _ttof(str);
}

void CStageUi::RenderLifeText()
{
}
