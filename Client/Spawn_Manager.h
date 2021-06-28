#pragma once
class CSpawn_Manager
{
private:
	CSpawn_Manager();
	~CSpawn_Manager();
public:
	static HRESULT Spawn(const wstring _wstrObjName, const PLACEMENT* _pPlacement, const OBJECTINFO* _pObjectInfo);
};

