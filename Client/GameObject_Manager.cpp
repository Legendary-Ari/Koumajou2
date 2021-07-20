#include "stdafx.h"
#include "GameObject_Manager.h"
#include "GameObject.h"
#include "CollisionMgr.h"

IMPLEMENT_SINGLETON(CGameObject_Manager)
CGameObject_Manager::CGameObject_Manager()
	:m_bStageChanging(false)
{
}


CGameObject_Manager::~CGameObject_Manager()
{
	Release_GameObject_Manager(); 
}

void CGameObject_Manager::Add_GameObject_Manager(OBJECTINFO::OBJID eID, CGameObject * pObject)
{
	if (OBJECTINFO::OBJID_END <= eID || nullptr == pObject || 0 > eID)
		return; 

	m_listGameObject[eID].emplace_back(pObject); 
}

void CGameObject_Manager::InitUpdate_GameObject_Manager()
{
	for (int i = 0; i < OBJECTINFO::OBJID_END; ++i)
	{
		for (auto& iter = m_listGameObject[i].begin(); iter != m_listGameObject[i].end(); ++iter)
		{
			(*iter)->InitUpdate_GameObject();

		}
	}
}

void CGameObject_Manager::Update_GameObject_Manager()
{
	CCollisionMgr::PlayerCheckBottom();
	for (int i = 0 ; i < OBJECTINFO::OBJID_END; ++i)
	{
		for (auto& iter = m_listGameObject[i].begin() ; iter != m_listGameObject[i].end() ; )
		{
			int iEvent = (*iter)->Update_GameObject(); 
			if (OBJ_DESTROYED == iEvent)
			{
				Safe_Delete(*iter); 
				iter = m_listGameObject[i].erase(iter); 
			}
			else ++iter; 
		}
	}

	CCollisionMgr::Collision_BackGroundEx(m_listGameObject[OBJECTINFO::PLAYER]);
	CCollisionMgr::Collision_BackGroundEx(m_listGameObject[OBJECTINFO::PLAYER_BULLET]);
	CCollisionMgr::Collision_BackGroundEx(m_listGameObject[OBJECTINFO::ENEMY]);
	CCollisionMgr::Collision_BackGroundEx(m_listGameObject[OBJECTINFO::ENEMY_BULLET]);
	CCollisionMgr::Collision_BackGroundEx(m_listGameObject[OBJECTINFO::BOSS]);

	CCollisionMgr::Collision(m_listGameObject[OBJECTINFO::ENEMY], m_listGameObject[OBJECTINFO::PLAYER]);
	CCollisionMgr::Collision(m_listGameObject[OBJECTINFO::BOSS], m_listGameObject[OBJECTINFO::PLAYER]);
	CCollisionMgr::Collision(m_listGameObject[OBJECTINFO::ENEMY], m_listGameObject[OBJECTINFO::PLAYER_BULLET]);
	CCollisionMgr::Collision(m_listGameObject[OBJECTINFO::BOSS], m_listGameObject[OBJECTINFO::PLAYER_BULLET]);
	CCollisionMgr::Collision(m_listGameObject[OBJECTINFO::ENEMY_BULLET], m_listGameObject[OBJECTINFO::PLAYER]);
	CCollisionMgr::Collision(m_listGameObject[OBJECTINFO::COLLISION], m_listGameObject[OBJECTINFO::PLAYER]);
	CCollisionMgr::Collision(m_listGameObject[OBJECTINFO::ITEM], m_listGameObject[OBJECTINFO::PLAYER]);
	CCollisionMgr::Collision_Ex(m_listGameObject[OBJECTINFO::ENEMY], m_listGameObject[OBJECTINFO::PLAYER_BULLET]);
	CCollisionMgr::Collision_Ex(m_listGameObject[OBJECTINFO::ENEMY], m_listGameObject[OBJECTINFO::PLAYER]);
	CCollisionMgr::Collision_Ex(m_listGameObject[OBJECTINFO::BOSS], m_listGameObject[OBJECTINFO::PLAYER]);

	

	for (int i = 0; i < OBJECTINFO::OBJID_END; ++i)
	{
		for (auto& pGameObject : m_listGameObject[i])
		{
			pGameObject->Late_Update_GameObject();

			if (m_bStageChanging)
			{
				for (auto& list : m_listGameObjectRender)
					list.clear();
				return;
			}

			m_listGameObjectRender[pGameObject->Get_RenderId()].emplace_back(pGameObject);
		}
	}
}

void CGameObject_Manager::Render_GameObject_Manager()
{
	if (m_bStageChanging)
	{
		m_bStageChanging = false;
		return;
	}
	for (int i = 0; i < RENDERID::END; ++i)
	{
		for (auto& pGameObject : m_listGameObjectRender[i])
		{
			pGameObject->Render_GameObject();
		}
		m_listGameObjectRender[i].clear();
	}
}

void CGameObject_Manager::Release_GameObject_Manager()
{
	for (int i = 0; i < OBJECTINFO::OBJID_END; ++i)
	{
		for (auto& pObject : m_listGameObject[i])
		{
			Safe_Delete(pObject);
		}
		m_listGameObject[i].clear();
	}
}

void CGameObject_Manager::Release_StageObject()
{
	for (int i = 0; i < OBJECTINFO::OBJID_END; ++i)
	{
		if (i == OBJECTINFO::PLAYER || i == OBJECTINFO::VSKILL)
			continue;
		for (auto& pObject : m_listGameObject[i])
		{
			Safe_Delete(pObject);
		}
		m_listGameObject[i].clear();
	}
	m_bStageChanging = true;
}

CGameObject * CGameObject_Manager::Get_Player()
{
	if (m_listGameObject[OBJECTINFO::PLAYER].empty())
		return nullptr;
	return m_listGameObject[OBJECTINFO::PLAYER].front();
}

CGameObject * CGameObject_Manager::Get_Boss()
{
	if (m_listGameObject[OBJECTINFO::BOSS].empty())
		return nullptr;
	return m_listGameObject[OBJECTINFO::BOSS].front();
}

list<CGameObject*>& CGameObject_Manager::Get_EnemyList()
{
	return m_listGameObject[OBJECTINFO::ENEMY];
}
