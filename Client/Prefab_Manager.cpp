#include "stdafx.h"
#include "Prefab_Manager.h"
#include "Spawn_Manager.h"

IMPLEMENT_SINGLETON(CPrefab_Manager)
CPrefab_Manager::CPrefab_Manager()
	:m_wstrPlacementPath(L"../Data/Placement%d.dat")
{
}


CPrefab_Manager::~CPrefab_Manager()
{
	for (auto & rPair : m_mapAnimationPrefab)
	{
		Safe_Delete(rPair.second);
	}
	m_mapAnimationPrefab.clear();
	for (auto & rPair : m_mapObjectPrefab)
	{
		Safe_Delete(rPair.second);
	}
	m_mapObjectPrefab.clear();
	for (auto & rPair : m_mapPlacementPrefab)
	{
		Safe_Delete(rPair.second);
	}
	m_mapPlacementPrefab.clear();
}

HRESULT CPrefab_Manager::Ready_Prefab_Manager()
{

	if (FAILED(LoadAnimationPrefab()))
		return E_FAIL;
	if (FAILED(LoadObjectPrefab()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPrefab_Manager::LoadObjectPrefab()
{
	HANDLE hFile = CreateFile(L"../Data/ObjectData.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return E_FAIL;
	}
		
	for (auto& rPair : m_mapObjectPrefab)
	{
		Safe_Delete(rPair.second);
	}
	m_mapObjectPrefab.clear();

	DWORD dwbyte = 0;
	DWORD strLen;
	OBJECTINFO* pObject;
	TCHAR *pBuff;
	// 난 쓸수 있어. 너넨 쓰지마 . // 메롱 쓸거지롱
	while (true)
	{

		ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);

		if (dwbyte == 0)
			break;

		pObject = new OBJECTINFO;

		pBuff = new TCHAR[strLen]{};
		ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
		pObject->wstrPrefabName = pBuff;
		Safe_Delete(pBuff);

		ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
		pBuff = new TCHAR[strLen]{};
		ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
		pObject->wstrObjectImage_ObjectKey = pBuff;
		Safe_Delete(pBuff);

		ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
		pBuff = new TCHAR[strLen]{};
		ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
		pObject->wstrObjectImage_Path = pBuff;
		Safe_Delete(pBuff);

		ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
		pBuff = new TCHAR[strLen]{};
		ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
		pObject->wstrDeathAnimImage_ObjectKey = pBuff;
		Safe_Delete(pBuff);

		ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
		pBuff = new TCHAR[strLen]{};
		ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
		pObject->wstrDeathAnimImage_StateKey = pBuff;
		Safe_Delete(pBuff);

		ReadFile(hFile, &pObject->fMaxHp, sizeof(float), &dwbyte, nullptr);
		ReadFile(hFile, &pObject->fAtk, sizeof(float), &dwbyte, nullptr);
		ReadFile(hFile, &pObject->fAtkRatio, sizeof(float), &dwbyte, nullptr);
		ReadFile(hFile, &pObject->fMoveSpeed, sizeof(float), &dwbyte, nullptr);
		ReadFile(hFile, &pObject->eObjId, sizeof(BYTE), &dwbyte, nullptr);
		ReadFile(hFile, &pObject->bDestructable, sizeof(bool), &dwbyte, nullptr);
		ReadFile(hFile, &pObject->eBulletType, sizeof(BYTE), &dwbyte, nullptr);
		ReadFile(hFile, &pObject->fShotGunAngle, sizeof(float), &dwbyte, nullptr);
		ReadFile(hFile, &pObject->iShotGunCount, sizeof(int), &dwbyte, nullptr);

		m_mapObjectPrefab.emplace(pObject->wstrPrefabName, pObject);

		if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::SINGLE_TEX, pObject->wstrObjectImage_Path, pObject->wstrObjectImage_ObjectKey)))
		{
			ERR_MSG(L"싱글 텍스쳐 실패");
			return E_FAIL;
		}
	}
	CloseHandle(hFile);
	return S_OK;
}

HRESULT CPrefab_Manager::LoadAnimationPrefab()
{

	HANDLE hFile = CreateFile(L"../Data/Effect.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;
	for (auto& rPair : m_mapAnimationPrefab)
		Safe_Delete(rPair.second);
	m_mapAnimationPrefab.clear();

	DWORD dwbyte = 0;
	DWORD dwStringSize = 0;
	ANIMATION* pAnimaInfo = 0;
	TCHAR* pBuf = nullptr;

	while (true)
	{
		ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
		if (0 == dwbyte)
			break;
		pBuf = new TCHAR[dwStringSize];
		pAnimaInfo = new ANIMATION;
		ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
		pAnimaInfo->wstrObjectKey = pBuf;
		Safe_Delete(pBuf);


		ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
		pBuf = new TCHAR[dwStringSize];
		ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
		pAnimaInfo->wstrStateKey = pBuf;
		Safe_Delete(pBuf);

		ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
		pBuf = new TCHAR[dwStringSize];
		ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
		pAnimaInfo->wstrFilePath = pBuf;
		Safe_Delete(pBuf);


		ReadFile(hFile, &pAnimaInfo->fPlay_Speed, sizeof(float), &dwbyte, nullptr);
		ReadFile(hFile, &pAnimaInfo->iMax_Index, sizeof(int), &dwbyte, nullptr);
		ReadFile(hFile, &pAnimaInfo->bLoop, sizeof(bool), &dwbyte, nullptr);
		ReadFile(hFile, &pAnimaInfo->bIsSingle, sizeof(bool), &dwbyte, nullptr);

		m_mapAnimationPrefab.emplace(pAnimaInfo->wstrObjectKey + pAnimaInfo->wstrStateKey, pAnimaInfo);

		//===============이미지 Insert==============
		if (pAnimaInfo->bIsSingle)
		{
			if (pAnimaInfo->wstrObjectKey != L"")
			{
				if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::SINGLE_TEX, pAnimaInfo->wstrFilePath, pAnimaInfo->wstrObjectKey)))
				{
					ERR_MSG(L"싱글 텍스쳐 실패");
					return E_FAIL;
				}
			}
		}
		else
		{
			if (pAnimaInfo->wstrObjectKey != L"" && pAnimaInfo->wstrStateKey != L"")
			{
				if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::MULTI_TEX, pAnimaInfo->wstrFilePath, pAnimaInfo->wstrObjectKey, pAnimaInfo->wstrStateKey, pAnimaInfo->iMax_Index)))
				{
					ERR_MSG(L"멀티 텍스쳐 실패");
					return E_FAIL;
				}
			}
		}
	}
	CloseHandle(hFile);

	return S_OK;
}

HRESULT CPrefab_Manager::LoadPlacementPrefab(const wstring & _path)
{
	HANDLE hFile = CreateFile(_path.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"Placement 파일이 없습니다");
		return E_FAIL;
	}

	for (auto& rPair : m_mapPlacementPrefab)
		Safe_Delete(rPair.second);
	m_mapPlacementPrefab.clear();

	DWORD dwbyte = 0;
	DWORD dwStringSize = 0;

	TCHAR* pBuf = nullptr;
	ACTORINFO* pPlacement = nullptr;

	while (true)
	{
		pPlacement = new ACTORINFO;
		ReadFile(hFile, &pPlacement->eRenderID, sizeof(int), &dwbyte, nullptr);
		if (0 == dwbyte)
		{
			Safe_Delete(pPlacement);
			break;
		}
		ReadFile(hFile, &pPlacement->m_tMatInfo.mat[MATID::TRANS], sizeof(D3DXVECTOR3), &dwbyte, nullptr);
		ReadFile(hFile, &pPlacement->m_tMatInfo.mat[MATID::ROT], sizeof(D3DXVECTOR3), &dwbyte, nullptr);
		ReadFile(hFile, &pPlacement->m_tMatInfo.mat[MATID::SCALE], sizeof(D3DXVECTOR3), &dwbyte, nullptr);

		ReadFile(hFile, &pPlacement->m_bRender, sizeof(bool), &dwbyte, nullptr);


		ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
		pBuf = new TCHAR[dwStringSize];
		ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
		pPlacement->wstrActorName = pBuf;

		Safe_Delete(pBuf);

		//	}

		ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
		pBuf = new TCHAR[dwStringSize];
		ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
		pPlacement->wstrPrefabName = pBuf;
		Safe_Delete(pBuf);

		ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
		pBuf = new TCHAR[dwStringSize];
		ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
		pPlacement->wstrObjectKey = pBuf;
		Safe_Delete(pBuf);

		ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
		pBuf = new TCHAR[dwStringSize];
		ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
		pPlacement->wstrFilePath = pBuf;
		Safe_Delete(pBuf);

		m_mapPlacementPrefab.emplace(pPlacement->wstrPrefabName, pPlacement);
		//===============이미지 Insert==============

		if (pPlacement->wstrObjectKey != L"")
		{
			if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::SINGLE_TEX, pPlacement->wstrFilePath, pPlacement->wstrPrefabName)))
			{
				ERR_MSG(L"싱글 텍스쳐 실패");
				return E_FAIL;
			}
		}
	}
	CloseHandle(hFile);

	return S_OK;
}

const OBJECTINFO * CPrefab_Manager::Get_ObjectPrefab(const wstring & _key) const
{
	auto& iter_find = m_mapObjectPrefab.find(_key);
	if(iter_find == m_mapObjectPrefab.end())
		return nullptr;
	return iter_find->second;
}

const ANIMATION * CPrefab_Manager::Get_AnimationPrefab(const wstring & _key) const
{
	auto& iter_find = m_mapAnimationPrefab.find(_key);
	if (iter_find == m_mapAnimationPrefab.end())
		return nullptr;
	return iter_find->second;
}

const ACTORINFO * CPrefab_Manager::Get_PlacementPrefab(const wstring & _key) const
{
	auto& iter_find = m_mapPlacementPrefab.find(_key);
	if (iter_find == m_mapPlacementPrefab.end())
		return nullptr;
	return iter_find->second;
}

HRESULT CPrefab_Manager::SpawnObjectbyScene(const CScene_Manager::ID & _id)
{
	TCHAR szFilePath[MAX_PATH] = L"";
	swprintf_s(szFilePath, m_wstrPlacementPath.c_str(), (int)_id);
	LoadPlacementPrefab(szFilePath);

	ACTORINFO* pPlacement = nullptr;
	for (auto& rPair : m_mapPlacementPrefab)
	{
		pPlacement = rPair.second;
		auto& iter_find = m_mapObjectPrefab.find(pPlacement->wstrActorName);
		if (iter_find == m_mapObjectPrefab.end())
		{
			//ERR_MSG(L"키값 오류");
			CSpawn_Manager::Spawn(L"", pPlacement, nullptr);
		}
		else
		{
			CSpawn_Manager::Spawn(iter_find->second->wstrPrefabName, pPlacement, iter_find->second);
		}
		
		
	}

	return S_OK;
}
