#include "stdafx.h"
#include "Knife.h"
#include "Effect.h"

CKnife::CKnife()
{
}


CKnife::~CKnife()
{
}

CGameObject * CKnife::Create(const OBJECTINFO * _tObjectInfo, D3DXVECTOR3 _vPos, float _fAngle)
{
	CGameObject* pInstance = new CKnife;
	pInstance->Set_Prefab(_tObjectInfo);
	pInstance->Set_Pos(_vPos);
	pInstance->Set_Angle(_fAngle);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

void CKnife::OnBlockedTile(CGameObject * pHitObject, DIRECTION::ID _eId)
{
	m_bDestroyed = true;
}

void CKnife::OnBlocked(CGameObject * pHitObject, DIRECTION::ID _eId)
{
	m_bDestroyed = true;
}

void CKnife::OnOverlaped(CGameObject* _pHitObject, _vec3 vHitPos)
{
	m_bDestroyed = true;
}

HRESULT CKnife::Ready_GameObject()
{
	m_tInfo.vDir = { cosf(D3DXToRadian(-m_tInfo.fAngle)),sinf(D3DXToRadian(-m_tInfo.fAngle)),0.f };
	m_tInfo.vSize = {1.f,1.f,0.f};

	m_vecAttackCollision.resize(1);
	m_vecBodyCollision.resize(1);
	m_vecTileCollision.resize(1);

	return S_OK;
}

void CKnife::InitUpdate_GameObject()
{
}

int CKnife::Update_GameObject()
{
	if (m_bDestroyed)
	{
		const ANIMATION* pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"SakuyaMelee");
		INFO tInfo;
		ZeroMemory(&tInfo, sizeof(INFO));
		tInfo.vPos = m_tInfo.vPos;
		if (m_tInfo.vDir.x < 0)
			tInfo.vDir.x = -5.f;
		tInfo.vSize = { 1.0f,1.0f,0.f };
		CGameObject * pObject = CEffect::Create(pAnim, tInfo);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::EFFECT, pObject);
		pObject = nullptr;
		return OBJ_DESTROYED;
	}

	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();

	if (abs(CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info().vPos.x - m_tInfo.vPos.x) > (CLIENTCX << 1))
		return OBJ_DESTROYED;

	m_tInfo.vPos += m_tInfo.vDir * m_pObjectInfo->fMoveSpeed * fDeltaTime;
	return OBJ_NOEVENT;
}

void CKnife::Late_Update_GameObject()
{
	UpdateAttackCollision();
	UpdateBodyCollision();
	UpdateTileCollision();
}

void CKnife::Render_GameObject()
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
	const RECT& rect = m_pObjectInfo->tRect;
	float 	fCenterX = float(((rect.right - rect.left)*0.5f));
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	RenderCollision();
}

void CKnife::Release_GameObject()
{
}

void CKnife::UpdateAttackCollision()
{

	float fSizeX = m_tInfo.vSize.x;
	float fSizeY = m_tInfo.vSize.y;
	float fReduceSizeLeft = 1.f;
	float fReduceSizeRight = 1.f;
	float fReduceSizeUp = 1.f;
	float fReduceSizeDown = 1.f;

	RECT rect = m_pObjectInfo->tRect;

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

void CKnife::UpdateBodyCollision()
{
	m_vecBodyCollision = m_vecAttackCollision;
}

void CKnife::UpdateTileCollision()
{
	m_vecTileCollision = m_vecBodyCollision;
}
