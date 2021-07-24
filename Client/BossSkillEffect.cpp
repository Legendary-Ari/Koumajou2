#include "stdafx.h"
#include "BossSkillEffect.h"


CBossSkillEffect::CBossSkillEffect()
	:m_fKillTime(2.5f)
	, m_fRemainKillTime(2.5f)
{
}


CBossSkillEffect::~CBossSkillEffect()
{
}

CGameObject * CBossSkillEffect::Create(const OBJECTINFO * _tObejctInfo, const _vec3 & _vPos)
{
	CGameObject* pInstance = new CBossSkillEffect;
	pInstance->Set_Prefab(_tObejctInfo);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	pInstance->Set_Pos(_vPos);
	return pInstance;
}

HRESULT CBossSkillEffect::Ready_GameObject()
{
	ZeroMemory(&m_tInfo,sizeof(INFO));
	m_tInfo.vSize = { 1.f,1.f,0.f };
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSS_SPELL);
	CSoundMgr::Get_Instance()->PlaySound(L"018.wav", CSoundMgr::BOSS_SPELL);
	return S_OK;
}

int CBossSkillEffect::Update_GameObject()
{
	float fDeltaTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_tInfo.fAngle += fDeltaTime * 100.f;

	m_fRemainKillTime -= fDeltaTime;
	if (m_fRemainKillTime <= 0)
		return OBJ_DESTROYED;

	return OBJ_NOEVENT;
}

void CBossSkillEffect::InitUpdate_GameObject()
{
}

void CBossSkillEffect::Late_Update_GameObject()
{
}

void CBossSkillEffect::Render_GameObject()
{
	for (int i = 0; i < 3; ++i)
	{
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_pObjectInfo->wstrObjectImage_ObjectKey);
		if (nullptr == pTexInfo)
			return;
		D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

		D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_tInfo.fAngle + i * 120.f));
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);
		matWorld = matScale * matRotZ * matTrans;
		const RECT& rect = m_pObjectInfo->tRect;
		float 	fCenterX = float(((rect.right - rect.left)*0.5f));
		float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(100, 255, 255, 255));

	}
	
}

void CBossSkillEffect::Release_GameObject()
{
}

const RENDERID::ID & CBossSkillEffect::Get_RenderId() const
{
	return RENDERID::BACKGROUND;
}
