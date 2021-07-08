#pragma once
class CSpawn_Manager
{
private:
	CSpawn_Manager();
	~CSpawn_Manager();
public:
	static HRESULT Spawn(const wstring _wstrObjName, const ACTORINFO* _pActorInfo, const OBJECTINFO* _pObjectInfo);
	static ACTORINFO CreateEmptyInfo(const INFO& _tInfo, wstring _wstrPrefabName);
};

