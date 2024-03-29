#pragma once
#include "Scene_Manager.h"
class CPrefab_Manager
{
	DECLARE_SINGLETON(CPrefab_Manager)
public:
	enum ID { ID_OBJECT, ID_ANIMATION, ID_PLACEMENT };
private:
	CPrefab_Manager();
	~CPrefab_Manager();
public:
	HRESULT Ready_Prefab_Manager();

	const OBJECTINFO* Get_ObjectPrefab(const wstring& _key) const;
	const ANIMATION* Get_AnimationPrefab(const wstring& _key) const;
	const ACTORINFO* Get_PlacementPrefab(const wstring& _key) const;
	HRESULT SpawnObjectbyScene(const CScene_Manager::ID& _id);

private:
	HRESULT LoadObjectPrefab();
	HRESULT LoadAnimationPrefab();
	HRESULT LoadActorPrefab(const wstring& _path);
private:
	map<wstring, OBJECTINFO*> m_mapObjectPrefab;
	map<wstring, ANIMATION*> m_mapAnimationPrefab;
	map<wstring, ACTORINFO*> m_mapActorPrefab;

	const wstring m_wstrActorPath;
};

