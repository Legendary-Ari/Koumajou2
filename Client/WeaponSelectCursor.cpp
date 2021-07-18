#include "stdafx.h"
#include "WeaponSelectCursor.h"
#include "VKnife.h"
#include "VS_Clock.h"
#include "VS_Meling.h"
#include "VS_Patchuri.h"
#include "VS_Reimu.h"
#include "VS_Flandre.h"
#include "VS_Alice.h"

CWeaponSelectCursor::CWeaponSelectCursor()
	:m_uiCurSel(0)
	, m_iSelectedIdx(0)
{
	ZeroMemory(m_pVSkill, sizeof(m_pVSkill));
}


CWeaponSelectCursor::~CWeaponSelectCursor()
{
	Release_GameObject();
}

CGameObject * CWeaponSelectCursor::Create(const OBJECTINFO * _pObjectInfo)
{
	CGameObject* pInstance = new CWeaponSelectCursor;
	pInstance->Set_Prefab(_pObjectInfo);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CWeaponSelectCursor::Ready_GameObject()
{
	m_iSelected[0] = -1;
	m_iSelected[1] = -1;
	m_iSelected[2] = -1;
	m_bSceneChange = false;
	m_tInfo.vPos = { (float)(CLIENTCX >> 1), (float)(CLIENTCY >> 1), 0.f };
	m_tInfo.vSize = { 0.6f,0.6f,0.0f };
	
	{	
		m_pVSkill[CVSkill::KNIFE] = static_cast<CVSkill*>(CVKnife::Create());
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::UI, m_pVSkill[CVSkill::KNIFE]);
		INFO tInfo{
			m_vRingPos[CVSkill::KNIFE],
			{ 0.f,0.f,0.f },
			m_tInfo.vSize,
			0.f
		};
		m_pVSkill[CVSkill::KNIFE]->Set_Info(tInfo);
	}

	{	
		m_pVSkill[CVSkill::CLOCK] = static_cast<CVSkill*>(CVS_Clock::Create());
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::UI, m_pVSkill[CVSkill::CLOCK]);
		INFO tInfo{
			m_vRingPos[CVSkill::CLOCK],
			{ 0.f,0.f,0.f },
			m_tInfo.vSize,
			0.f
		};
		m_pVSkill[CVSkill::CLOCK]->Set_Info(tInfo); 
	}

	{
		m_pVSkill[CVSkill::MEILING] = static_cast<CVSkill*>(CVS_Meling::Create());
		if (m_pVSkill[CVSkill::MEILING])
		{
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::UI, m_pVSkill[CVSkill::MEILING]);
			INFO tInfo{
				m_vRingPos[CVSkill::MEILING],
				{ 0.f,0.f,0.f },
				m_tInfo.vSize,
				0.f
			};
			m_pVSkill[CVSkill::MEILING]->Set_Info(tInfo);
		}
	}

	{
		m_pVSkill[CVSkill::ALICE] = static_cast<CVSkill*>(CVS_Alice::Create());
		if (m_pVSkill[CVSkill::ALICE])
		{
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::UI, m_pVSkill[CVSkill::ALICE]);
			INFO tInfo{
				m_vRingPos[CVSkill::ALICE],
				{ 0.f,0.f,0.f },
				m_tInfo.vSize,
				0.f
			};
			m_pVSkill[CVSkill::ALICE]->Set_Info(tInfo);
		}
		
	}

	{
		m_pVSkill[CVSkill::PATCHURI] = static_cast<CVSkill*>(CVS_Patchuri::Create());
		if (m_pVSkill[CVSkill::PATCHURI])
		{
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::UI, m_pVSkill[CVSkill::PATCHURI]);
			INFO tInfo{
				m_vRingPos[CVSkill::PATCHURI],
				{ 0.f,0.f,0.f },
				m_tInfo.vSize,
				0.f
			};
			m_pVSkill[CVSkill::PATCHURI]->Set_Info(tInfo);
		}
	}

	{
		m_pVSkill[CVSkill::REIMU] = static_cast<CVSkill*>(CVS_Reimu::Create());
		if (m_pVSkill[CVSkill::REIMU])
		{
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::UI, m_pVSkill[CVSkill::REIMU]);
			INFO tInfo{
				m_vRingPos[CVSkill::REIMU],
				{ 0.f,0.f,0.f },
				m_tInfo.vSize,
				0.f
			};
			m_pVSkill[CVSkill::REIMU]->Set_Info(tInfo);
		}
	}
	{
		m_pVSkill[CVSkill::FLANDRE] = static_cast<CVSkill*>(CVS_Flandre::Create());
		if (m_pVSkill[CVSkill::FLANDRE])
		{
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::UI, m_pVSkill[CVSkill::FLANDRE]);
			INFO tInfo{
				m_vRingPos[CVSkill::FLANDRE],
				{ 0.f,0.f,0.f },
				m_tInfo.vSize,
				0.f
			};
			m_pVSkill[CVSkill::FLANDRE]->Set_Info(tInfo);
		}
	}
	return S_OK;
}

void CWeaponSelectCursor::InitUpdate_GameObject()
{
	m_tInfo.vPos = m_vRingPos[m_uiCurSel];
}

int CWeaponSelectCursor::Update_GameObject()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	if (CKey_Manager::Get_Instance()->Key_Down(KEY_LEFT))
	{
		do 
		{
			if (m_uiCurSel == 0)
				m_uiCurSel = (UINT)CVSkill::SKILL_END;
			else
				--m_uiCurSel;
			if (m_uiCurSel == (UINT)CVSkill::SKILL_END)
				break;
			
		} while (!m_pVSkill[m_uiCurSel] || IsSelected(m_uiCurSel));
		
		m_tInfo.vPos = m_vRingPos[m_uiCurSel];
	}
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_RIGHT))
	{
		do{
			if (m_uiCurSel == (UINT)CVSkill::SKILL_END)
				m_uiCurSel = 0;
			else
				++m_uiCurSel;
			if (m_uiCurSel == (UINT)CVSkill::SKILL_END)
				break;
		} while (!m_pVSkill[m_uiCurSel] || IsSelected(m_uiCurSel));
		m_tInfo.vPos = m_vRingPos[m_uiCurSel];
	}

	if (CKey_Manager::Get_Instance()->Key_Down(KEY_X))
	{
		if (m_iSelectedIdx > 0)
		{
			UINT uiTarget = m_iSelected[m_iSelectedIdx - 1];
			m_pVSkill[uiTarget]->Set_Pos(m_vRingPos[uiTarget]);
			m_iSelected[m_iSelectedIdx - 1] = -1;
			--m_iSelectedIdx;

			m_uiCurSel = uiTarget;
			m_tInfo.vPos = m_vRingPos[m_uiCurSel];
		}
	}
	
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_Z))
	{

		if (m_uiCurSel == CVSkill::SKILL_END)
		{
			m_bSceneChange = true;
			for (UINT i = 0; i < 3; ++i)
			{
				CGame_Manager::Get_Instance()->m_uiSelectedSubWeapon[i] = (UINT)m_iSelected[i];
			}
		}			
		else
		{
			
			m_iSelected[m_iSelectedIdx] = m_uiCurSel;
			m_pVSkill[m_uiCurSel]->Set_Pos(m_vSelectedPos[m_iSelectedIdx]);

			do {
				if (m_uiCurSel == (UINT)CVSkill::SKILL_END)
					m_uiCurSel = 0;
				else
					++m_uiCurSel;
			} while (m_uiCurSel != CVSkill::SKILL_END && !m_pVSkill[m_uiCurSel]);
			m_tInfo.vPos = m_vRingPos[m_uiCurSel];
			++m_iSelectedIdx;
		}
	}
	if (m_iSelectedIdx == 3)
	{
		m_uiCurSel = CVSkill::SKILL_END;
		m_tInfo.vPos = m_vRingPos[m_uiCurSel];
	}


	return OBJ_NOEVENT;
}

void CWeaponSelectCursor::Late_Update_GameObject()
{
	if(m_bSceneChange)
		CScene_Manager::Get_Instance()->Change_Scene_Manager(CScene_Manager::STAGE_1_1);
}

void CWeaponSelectCursor::Render_GameObject()
{
	//TCHAR str[32];
	//GetPrivateProfileString(L"Pos", L"x", L"", str, sizeof(str), L"../Temp.ini");
	//m_tInfo.vPos.x = _ttof(str);
	//GetPrivateProfileString(L"Pos", L"y", L"", str, sizeof(str), L"../Temp.ini");
	//m_tInfo.vPos.y = _ttof(str);
	{
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_pObjectInfo->wstrObjectImage_ObjectKey);
		if (nullptr == pTexInfo)
			return;
		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();
		//초기화
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
		matWorld = matScale * matTrans;
		float fCenterX;
		float fCenterY;
		RECT rect = m_pObjectInfo->tRect;
		fCenterX = float(((rect.right - rect.left)*0.5f));
		fCenterY = float(((rect.bottom - rect.top) * 0.5f));
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	{
		const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"WeaponSelectCursor");
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(pObjectInfo->wstrObjectImage_ObjectKey);
		if (nullptr == pTexInfo)
			return;
		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();
		//초기화
		D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y - 50.f, 0.f);
		matWorld = matScale* matTrans;
		float fCenterX;
		float fCenterY;
		RECT rect = pObjectInfo->tRect;
		fCenterX = float(((rect.right - rect.left)*0.5f));
		fCenterY = float(((rect.bottom - rect.top) * 0.5f));
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
}

void CWeaponSelectCursor::Release_GameObject()
{
}

bool CWeaponSelectCursor::IsSelected(UINT _uiCurSel)
{
	if (m_iSelectedIdx == 0)
		return false;
	for (UINT i = 0; i <= m_iSelectedIdx; ++i)
	{
		if (m_iSelected[i] == _uiCurSel)
			return true;
	}
	return false;
}
