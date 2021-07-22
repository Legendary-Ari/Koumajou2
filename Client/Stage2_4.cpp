#include "stdafx.h"
#include "Stage2_4.h"
#include "Player.h"
#include "BossHpUi.h"
#include "InvisibleBlock.h"
#include "StageUi.h"
#include "BossRemilia.h"

CStage2_4::CStage2_4()
{
}


CStage2_4::~CStage2_4()
{
}

HRESULT CStage2_4::Ready_Scene()
{
	if (m_ePlaying != CScene_Manager::STAGE_2_4)
	{
		CSoundMgr::Get_Instance()->StopAll();
		CSoundMgr::Get_Instance()->PlayBGM(L"Stage 2-4.mp3");
		m_ePlaying = CScene_Manager::STAGE_2_4;
	}

	m_vStartPos = { 30.f,270.f,0.f };
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();
	D3DXVECTOR3 vDiff = m_vStartPos - D3DXVECTOR3{ float(CLIENTCX >> 1), float(CLIENTCY >> 1), 0.f };
	CScroll_Manager::Force_Set_Scroll(-vDiff);

	{
		const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Remilia");
		assert(pObjectInfo);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::BOSS, CBossRemilia::Create(pObjectInfo));

	}

	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::UI, CStageUi::Create());

	if (!CGameObject_Manager::Get_Instance()->Get_Player())
	{
		const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Player");
		const ACTORINFO* pActorInfo = CPrefab_Manager::Get_Instance()->Get_PlacementPrefab(L"Player");
		if (pActorInfo)
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, CPlayer::Create(pObjectInfo, pActorInfo->tInfo));
		else
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::OBJID)pObjectInfo->eObjId, CPlayer::Create(pObjectInfo, { { 50.f,500.f,0.f },{ 0.f,0.f,0.f },{ 1.f,1.f,0.f },0.f }));
	}
	{
		INFO tInfo;
		ZeroMemory(&tInfo, sizeof(tInfo));
		tInfo.vPos = { float(WINCX >> 1),float(CLIENTCY + 20.f),0.f };
		tInfo.vSize = { float(WINCX), 64.f, 0.f };
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJECTINFO::BACKGROUND), CInvisibleBlock::Create(tInfo));
	}
	//{
	//	const OBJECTINFO* pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"BossHpBar");
	//	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::UI, CBossHpUi::Create(pObjectInfo));
	//}
	return S_OK;
}

void CStage2_4::InitUpdate_Scene()
{
	CGameObject_Manager::Get_Instance()->InitUpdate_GameObject_Manager();
}

void CStage2_4::Update_Scene()
{
	CGameObject_Manager::Get_Instance()->Update_GameObject_Manager();
}

void CStage2_4::Render_Scene()
{
	CGameObject_Manager::Get_Instance()->Render_GameObject_Manager();
}

void CStage2_4::Release_Scene()
{
	CGameObject_Manager::Get_Instance()->Release_StageObject();
}

CScene * CStage2_4::Create()
{
	CScene* pInstance = new CStage2_4;
	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Delete(pInstance);
		return pInstance;
	}
	return pInstance;
}