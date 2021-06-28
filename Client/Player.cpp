#include "stdafx.h"
#include "Player.h"
#include "Prefab_Manager.h"
#include "Bullet.h"

CPlayer::CPlayer()
{
}


CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject()
{
	m_tInfo.vPos.x = 400.f; 
	m_tInfo.vPos.y = 300.f; 
	m_tInfo.vDir = { 0.f, 0.f, 0.f }; 
	m_tInfo.vSize = D3DXVECTOR3(100.f, 100.f, 0.f); 

	CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::SINGLE_TEX, m_pObjectInfo->wstrObjectImage_Path, m_pObjectInfo->wstrObjectImage_ObjectKey);

	m_eRenderId = RENDERID::OBJECT;

	return S_OK;
}

int CPlayer::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.vDir = {0.f, 0.f, 0.f};

	if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_LEFT))
		m_tInfo.vDir.x = -m_pObjectInfo->fMoveSpeed;
	if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_RIGHT))
		m_tInfo.vDir.x = +m_pObjectInfo->fMoveSpeed;
	if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_DOWN))
		m_tInfo.vDir.y = m_pObjectInfo->fMoveSpeed;
	if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_UP))
		m_tInfo.vDir.y = -m_pObjectInfo->fMoveSpeed;
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_SPACE))
	{
		const OBJECTINFO* pPrefab = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Player_Bullet");
		CGameObject * pObject = CBullet::Create(pPrefab, m_tInfo.vPos, m_fAngle);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pPrefab->eObjId, pObject);
		pObject = nullptr;
	}

	m_tInfo.vPos += m_tInfo.vDir;

	CScroll_Manager::Set_Scroll(-m_tInfo.vDir * 0.5f);

	return OBJ_NOEVENT;
}

void CPlayer::Late_Update_GameObject()
{
	
}

void CPlayer::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_pObjectInfo->wstrObjectImage_ObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);
	matWorld = matScale * matRotZ * matTrans;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CPlayer::Release_GameObject()
{
}

CGameObject * CPlayer::Create(const OBJECTINFO* _pPrefab, const PLACEMENT* _pPlacement)
{
	CGameObject* pInstance = new CPlayer;
	pInstance->Set_Prefab(_pPrefab);
	pInstance->Set_Placement(_pPlacement);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}
