#include "stdafx.h"
#include "SubMeling.h"
#include "Effect.h"

CSubMeling::CSubMeling()
	:m_fRemainUpTime(0.3f)
	, m_bIsGoingUp(true)
{
}


CSubMeling::~CSubMeling()
{
}

CGameObject * CSubMeling::Create(const INFO& tInfo)
{
	CGameObject* pInstance = new CSubMeling;
	pInstance->Set_Info(tInfo);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

void CSubMeling::OnBlockedTile(CGameObject * pHitObject, DIRECTION::ID _eId)
{
	m_uiAnimationFrame = 3;
	m_bDead = true;
}

void CSubMeling::OnBlocked(CGameObject * pHitObject, DIRECTION::ID _eId)
{
	if (_eId == DIRECTION::N && m_bIsGoingUp)
		m_bIsGoingUp = false;
	else
		m_bDestroyed = true;
}

void CSubMeling::OnOverlaped(CGameObject * _pHitObject, _vec3 vHitPos)
{

}

HRESULT CSubMeling::Ready_GameObject()
{
	m_tInfo.vDir = { cosf(D3DXToRadian(-m_tInfo.fAngle)),sinf(D3DXToRadian(-m_tInfo.fAngle)),0.f };
	m_tInfo.vSize = { 1.f,1.f,0.f };

	const ANIMATION* pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"MeilingJump");
	m_vecAnimation.emplace_back(pAnim);

	m_pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"SubMeiling");

	if (m_tInfo.fAngle == 180.f)
		m_bFliped = true;
	else
		m_bFliped = false;
	
	m_vecAttackCollision.resize(1);
	m_vecBodyCollision.resize(1);
	m_vecTileCollision.resize(1);
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::VSKILL);
	{
		int i = rand() % 3;
		if (i == 0)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"sak_A033.wav", CSoundMgr::VSKILL);
		}
		else if (i == 1)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"sak_A035.wav", CSoundMgr::VSKILL);
		}
		else if (i == 2)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"sak_A037.wav", CSoundMgr::VSKILL);
		}
	}
	return S_OK;
}

void CSubMeling::InitUpdate_GameObject()
{
}

int CSubMeling::Update_GameObject()
{
	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_bDestroyed)
	{
		const ANIMATION* pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"SakuyaMelee");
		INFO tInfo;
		ZeroMemory(&tInfo, sizeof(INFO));
		tInfo.vPos = m_tInfo.vPos;
		tInfo.vSize = { 1.0f,1.0f,0.f };
		CGameObject * pObject = CEffect::Create(pAnim, tInfo);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::EFFECT, pObject);
		pObject = nullptr;
		return OBJ_DESTROYED;
	}
	if (m_bDead)
	{
		m_uiAnimationFrame = 3;
		m_tInfo.vSize.x -= 10.f * fDeltaTime;
		m_tInfo.vSize.y += 5.f * fDeltaTime;
		if (m_tInfo.vSize.x <= 0.f)
			m_bDestroyed = true;
		return OBJ_NOEVENT;
	}



	if (m_bIsGoingUp)
	{
		m_fRemainUpTime -= fDeltaTime;
		if (abs(m_fRemainUpTime) < 0.05f)
		{
			m_uiAnimationFrame = 1;
		}
			
		if (m_fRemainUpTime <= 0)
		{
			m_bIsGoingUp = false;
		}
	}
	else
	{
		m_uiAnimationFrame = 2;
	}

	m_tInfo.vDir.x = cosf(D3DXToRadian(-m_tInfo.fAngle)) * fDeltaTime * m_pObjectInfo->fMoveSpeed * 0.5f;

	if (m_bIsGoingUp)
	{
		m_tInfo.vDir.y = -fDeltaTime * m_pObjectInfo->fMoveSpeed;
	}
	else
	{
		m_tInfo.vDir.y = fDeltaTime * m_pObjectInfo->fMoveSpeed;
	}

	m_tInfo.vPos += m_tInfo.vDir * m_pObjectInfo->fMoveSpeed * fDeltaTime;
	return OBJ_NOEVENT;
}

void CSubMeling::Late_Update_GameObject()
{
	if (m_bOnGround && !m_bIsGoingUp)
		m_uiAnimationFrame = 3;
	UpdateAttackCollision();
	UpdateTileCollision();
}

void CSubMeling::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_vecAnimation[0]->wstrObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, (m_bFliped ? -m_tInfo.vSize.x : m_tInfo.vSize.x), m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);
	matWorld = matScale  * matTrans;
	const RECT& rect = m_vecAnimation[0]->vecRect[m_uiAnimationFrame];
	float 	fCenterX = float(((rect.right - rect.left)*0.5f));
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	RenderCollision();
}

void CSubMeling::Release_GameObject()
{
}

void CSubMeling::UpdateAttackCollision()
{
	float fSizeX = m_tInfo.vSize.x;
	float fSizeY = m_tInfo.vSize.y;
	float fReduceSizeLeft = 0.f;
	float fReduceSizeRight = 1.f;
	float fReduceSizeUp = 0.5f;
	float fReduceSizeDown = 1.f;

	RECT rect = m_vecAnimation[0]->vecRect[m_uiAnimationFrame];

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

void CSubMeling::UpdateTileCollision()
{
	m_vecTileCollision = m_vecAttackCollision;
}