#include "stdafx.h"
#include "BossHpUi.h"
#include "BossAlice.h"
#include "BossRemilia.h"

CBossHpUi::CBossHpUi()
{
}


CBossHpUi::~CBossHpUi()
{
}

CGameObject * CBossHpUi::Create(const OBJECTINFO * _pObjectInfo)
{
	CGameObject* pInstance = new CBossHpUi;
	pInstance->Set_Prefab(_pObjectInfo);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CBossHpUi::Ready_GameObject()
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	m_tInfo.vPos = { 400.f, 290.f, 0.f };
	m_tInfo.vSize = { 0.75f,0.75f,0.f };
	ZeroMemory(&m_tHpBarInfo, sizeof(INFO));
	m_tHpBarInfo.vPos = { 465.f, 286.f, 0.f };
	m_tHpBarInfo.vSize = { 6.f,0.75f,0.f };
	ZeroMemory(&m_tIconInfo, sizeof(INFO));
	m_tIconInfo.vPos = { 494.f, 287.f, 0.f };
	m_tIconInfo.vSize = { 0.75f,0.75f,0.f };
	return S_OK;
}

void CBossHpUi::InitUpdate_GameObject()
{
	m_pCurHp = &CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Hp();
}

int CBossHpUi::Update_GameObject()
{
	if ((*m_pCurHp) <= 0)
		return OBJ_DESTROYED;
	return OBJ_NOEVENT;
}

void CBossHpUi::Late_Update_GameObject()
{
	m_tHpBarInfo.vSize.x = 2.f * 0.01f * (*m_pCurHp);
	if (m_tHpBarInfo.vSize.x < 0)
		m_tHpBarInfo.vSize.x = 0;
}

void CBossHpUi::Render_GameObject()
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
	RenderIcon();
}

void CBossHpUi::Release_GameObject()
{
}

void CBossHpUi::RenderHp()
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
	float 	fCenterX = rect.right;
	float 	fCenterY = 0.f;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CBossHpUi::RenderIcon()
{
	//TCHAR str[32];
	//GetPrivateProfileString(L"Pos", L"x", L"", str, sizeof(str), L"../Temp.ini");
	//m_tIconInfo.vPos.x = _ttof(str);
	//GetPrivateProfileString(L"Pos", L"y", L"", str, sizeof(str), L"../Temp.ini");
	//m_tIconInfo.vPos.y = _ttof(str);
	const OBJECTINFO* pObjectInfo;
	if (dynamic_cast<CBossRemilia*>(CGameObject_Manager::Get_Instance()->Get_Boss()))
	{
		pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"IconRemilia");
	}
	else
	{
		pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"IconAlice");
	}
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(pObjectInfo->wstrObjectImage_ObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

	D3DXMATRIX matScale, matRotZ, matTrans, matOrbit, matWorld;
	D3DXMatrixScaling(&matScale, m_tIconInfo.vSize.x, m_tIconInfo.vSize.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_tIconInfo.fAngle));
	D3DXMatrixTranslation(&matTrans, m_tIconInfo.vPos.x, m_tIconInfo.vPos.y, 0.f);
	D3DXMatrixRotationZ(&matOrbit, D3DXToRadian(m_tIconInfo.fAngle));
	matWorld = matScale * matRotZ * matTrans * matOrbit;
	const RECT& rect = pObjectInfo->tRect;
	float 	fCenterX = float(((rect.right - rect.left)*0.5f));
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

