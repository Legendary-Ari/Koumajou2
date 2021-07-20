#include "stdafx.h"
#include "Chi.h"
#include "Player.h"

CChi::CChi()
	:m_fSpeed(30.f)
{
}


CChi::~CChi()
{
}

CGameObject * CChi::Create(bool _bIsDeca, _vec3 _vPos)
{
	CGameObject* pInstance = new CChi;
	static_cast<CChi*>(pInstance)->Set_Chi(_bIsDeca ? 10 : 1);
	pInstance->Set_Pos(_vPos);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CChi::Ready_GameObject()
{
	if (m_uiChiMount == 10)
	{
		m_tInfo.vSize = {1.f,1.f,0.f};
	}
	else
	{
		m_tInfo.vSize = { 0.5f,0.5f,0.f };
	}

	m_vecAnimation.resize(1);
	m_vecAnimation[0] = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"Common_EffectsChi");
	if (!m_vecAnimation[0])
	{
		ERR_MSG(L"CChi 이미지 로드 실패");
		return E_FAIL;
	}
		
	return S_OK;
}

void CChi::InitUpdate_GameObject()
{
}

int CChi::Update_GameObject()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;
	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	_vec3 vDiff = CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info().vPos - m_tInfo.vPos;
	
	_vec3 vNormalizedDiff;
	D3DXVec3Normalize(&vNormalizedDiff, &vDiff);
	m_fSpeed += 7.f;
	m_tInfo.vDir = vNormalizedDiff * m_fSpeed * fDeltaTime;
	m_tInfo.vPos += m_tInfo.vDir;

	UpdateAnimation();
	return OBJ_NOEVENT;
}

void CChi::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_vecAnimation[0]->wstrObjectKey);
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

	D3DXMATRIX matScale, matTrans, matWorld, matParentTransl;
	D3DXMatrixScaling(&matScale, (m_bFliped ? -m_tInfo.vSize.x : m_tInfo.vSize.x), m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);
	matWorld = matScale * matTrans;
	const RECT& rect = m_vecAnimation[0]->vecRect[0];
	float 	fCenterX = float(((rect.right - rect.left) * 0.5f));
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
	const RECT& tRenderRect = m_vecAnimation[0]->vecRect[m_uiAnimationFrame];
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	D3DCOLOR tColor;
	tColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &tRenderRect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, tColor);

}

void CChi::Release_GameObject()
{
}

void CChi::Give_ThisItem()
{
	static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Add_Chi(m_uiChiMount);
	m_bDestroyed = true;
}

void CChi::Set_Chi(UINT _uiChi)
{
	m_uiChiMount = _uiChi;
}
