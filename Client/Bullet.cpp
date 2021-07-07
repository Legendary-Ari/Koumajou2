#include "stdafx.h"
#include "Bullet.h"
#include "Effect.h"
#include "Prefab_Manager.h"

CBullet::CBullet()
{
}


CBullet::~CBullet()
{
	Release_GameObject();
}

CGameObject * CBullet::Create(const OBJECTINFO * _tObjectInfo, D3DXVECTOR3 _vPos, float _fAngle)
{
	CGameObject* pInstance = new CBullet;
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

HRESULT CBullet::Ready_GameObject()
{
	m_tInfo.vDir.z = 0.f;



	return S_OK;
}

int CBullet::Update_GameObject()
{
	if (m_bDestroyed)
	{
		const ANIMATION* pPrefab = nullptr; //CPrefab_Manager::Get_Instance()->Get_AnimationPrefab();
		CGameObject * pObject = CEffect::Create(pPrefab, m_tInfo.vPos);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::UI, pObject);
		pObject = nullptr;
		return OBJ_DESTROYED;
	}
		

	m_tInfo.vDir.x = cosf(D3DXToRadian(-m_tInfo.fAngle));
	m_tInfo.vDir.y = sinf(D3DXToRadian(-m_tInfo.fAngle));

	m_tInfo.vPos += m_tInfo.vDir * m_pObjectInfo->fMoveSpeed;

	return OBJ_NOEVENT;
}

void CBullet::Late_Update_GameObject()
{
	if (m_tInfo.vPos.x > WINCX - 100)
		m_bDestroyed = true;
}

void CBullet::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_pObjectInfo->wstrObjectImage_ObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_tInfo.fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	matWorld = matScale * matRotZ * matTrans;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CBullet::Release_GameObject()
{
}
