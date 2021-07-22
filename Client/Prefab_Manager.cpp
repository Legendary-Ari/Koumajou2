#include "stdafx.h"
#include "Prefab_Manager.h"
#include "Spawn_Manager.h"

IMPLEMENT_SINGLETON(CPrefab_Manager)
CPrefab_Manager::CPrefab_Manager()
	:m_wstrActorPath(L"../Data/TreeItem %02d-%02d.dat")
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
	for (auto & rPair : m_mapActorPrefab)
	{
		Safe_Delete(rPair.second);
	}
	m_mapActorPrefab.clear();
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

		pObject = new OBJECTINFO{};

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
		pObject->cstrIdleAnimImage_ObjectKey = pBuff;
		Safe_Delete(pBuff);

		ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
		pBuff = new TCHAR[strLen]{};
		ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
		pObject->wstrIdleAnimImage_StateKey = pBuff;
		Safe_Delete(pBuff);

		ReadFile(hFile, &pObject->bIsSingle, sizeof(bool), &dwbyte, nullptr);
		ReadFile(hFile, &pObject->fMaxHp, sizeof(float), &dwbyte, nullptr);
		ReadFile(hFile, &pObject->fAtk, sizeof(float), &dwbyte, nullptr);
		ReadFile(hFile, &pObject->fAtkRatio, sizeof(float), &dwbyte, nullptr);
		ReadFile(hFile, &pObject->fMoveSpeed, sizeof(float), &dwbyte, nullptr);
		ReadFile(hFile, &pObject->eObjId, sizeof(BYTE), &dwbyte, nullptr);
		ReadFile(hFile, &pObject->eRenderId, sizeof(BYTE), &dwbyte, nullptr);
		ReadFile(hFile, &pObject->tRect, sizeof(RECT), &dwbyte, nullptr);
		ReadFile(hFile, &pObject->bDestructable, sizeof(bool), &dwbyte, nullptr);
		ReadFile(hFile, &pObject->eBulletType, sizeof(BYTE), &dwbyte, nullptr);

		m_mapObjectPrefab.emplace(pObject->wstrPrefabName, pObject);

		if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::SINGLE_TEX, { pObject->tRect }, pObject->wstrObjectImage_Path, pObject->wstrObjectImage_ObjectKey)))
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

	HANDLE hFile = CreateFile(L"../Data/AnimationData.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;
	for (auto& rPair : m_mapAnimationPrefab)
		Safe_Delete(rPair.second);
	m_mapAnimationPrefab.clear();

	DWORD dwbyte = 0;
	DWORD dwStringSize = 0;
	ANIMATION* pAnimaInfo = 0;
	TCHAR* pBuff = nullptr;

	while (true)
	{
		ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
		if (0 == dwbyte)
			break;
		pBuff = new TCHAR[dwStringSize];
		pAnimaInfo = new ANIMATION;
		ReadFile(hFile, pBuff, dwStringSize, &dwbyte, nullptr);
		pAnimaInfo->wstrObjectKey = pBuff;
		Safe_Delete(pBuff);


		ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
		pBuff = new TCHAR[dwStringSize];
		ReadFile(hFile, pBuff, dwStringSize, &dwbyte, nullptr);
		pAnimaInfo->wstrStateKey = pBuff;
		Safe_Delete(pBuff);

		ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
		pBuff = new TCHAR[dwStringSize];
		ReadFile(hFile, pBuff, dwStringSize, &dwbyte, nullptr);
		pAnimaInfo->wstrFilePath = pBuff;
		Safe_Delete(pBuff);


		ReadFile(hFile, &pAnimaInfo->fPlay_Speed, sizeof(float), &dwbyte, nullptr);
		ReadFile(hFile, &pAnimaInfo->bLoop, sizeof(bool), &dwbyte, nullptr);

		UINT iRectSize = 0;
		ReadFile(hFile, &iRectSize, sizeof(UINT), &dwbyte, nullptr);
		pAnimaInfo->vecRect.reserve(iRectSize);
		for (UINT i = 0; i<iRectSize; ++i)
		{
			RECT rect;
			ReadFile(hFile, &rect, sizeof(RECT), &dwbyte, nullptr);
			pAnimaInfo->vecRect.emplace_back(rect);
		}
		m_mapAnimationPrefab.emplace(pAnimaInfo->wstrObjectKey + pAnimaInfo->wstrStateKey, pAnimaInfo);
		CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::MULTI_TEX, pAnimaInfo->vecRect, pAnimaInfo->wstrFilePath, pAnimaInfo->wstrObjectKey, pAnimaInfo->wstrStateKey);
	}
	CloseHandle(hFile);

	return S_OK;
}

HRESULT CPrefab_Manager::LoadActorPrefab(const wstring & _path)
{
	HANDLE hFile = CreateFile(_path.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"Actor 파일이 없습니다");
		return E_FAIL;
	}

	for (auto& rPair : m_mapActorPrefab)
		Safe_Delete(rPair.second);
	m_mapActorPrefab.clear();

	DWORD dwbyte = 0;
	DWORD strLen = 0;
	UINT mIDX;
	TCHAR* pBuff = nullptr;
	ACTORINFO* pActorInfo = nullptr;
	UINT uiMapX;
	UINT uiMapY;
	ReadFile(hFile, &mIDX, sizeof(UINT), &dwbyte, nullptr);
	if (dwbyte == 0)
	{
		ERR_MSG(L"Actor 읽을거리 없음");
		return E_FAIL;
	}
	ReadFile(hFile, &mIDX, sizeof(UINT), &dwbyte, nullptr);
	ReadFile(hFile, &uiMapX, sizeof(UINT), &dwbyte, nullptr);
	ReadFile(hFile, &uiMapY, sizeof(UINT), &dwbyte, nullptr);
	CScroll_Manager::SetMapSize((int)uiMapX, (int)uiMapY);
	while (true)
	{
		ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);

		if (dwbyte == 0)
			break;

		pActorInfo = new ACTORINFO{};
		pBuff = new TCHAR[strLen]{};
		ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
		pActorInfo->wstrActorName = pBuff;
		Safe_Delete(pBuff);

		ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
		pBuff = new TCHAR[strLen]{};
		ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
		pActorInfo->wstrPrefabName = pBuff;
		Safe_Delete(pBuff);

		ReadFile(hFile, &pActorInfo->bIsFolder, sizeof(bool), &dwbyte, nullptr);
		ReadFile(hFile, &pActorInfo->tInfo, sizeof(INFO), &dwbyte, nullptr);
		if (!pActorInfo->bIsFolder)
			m_mapActorPrefab.emplace(pActorInfo->wstrActorName, pActorInfo);
		else
			Safe_Delete(pActorInfo);
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
	auto& iter_find = m_mapActorPrefab.find(_key);
	if (iter_find == m_mapActorPrefab.end())
		return nullptr;
	return iter_find->second;
}

HRESULT CPrefab_Manager::SpawnObjectbyScene(const CScene_Manager::ID & _id)
{
	UINT uiFirst = 0, uiSecond = 0;
	switch (_id)
	{
	case CScene_Manager::STAGE_1_1:
		uiFirst = 1;
		uiSecond = 1;
		break;
	case CScene_Manager::STAGE_1_4:
		uiFirst = 1;
		uiSecond = 4;
		break;
	case CScene_Manager::STAGE_2_4:
		uiFirst = 2;
		uiSecond = 4;
		break;
	default:
		break;
	}
	TCHAR szFilePath[MAX_PATH] = L"";
	swprintf_s(szFilePath, m_wstrActorPath.c_str(), uiFirst,uiSecond);
	LoadActorPrefab(szFilePath);

	ACTORINFO* pActor = nullptr;
	for (auto& rPair : m_mapActorPrefab)
	{
		pActor = rPair.second;
		auto& iter_find = m_mapObjectPrefab.find(pActor->wstrPrefabName);
		if (iter_find == m_mapObjectPrefab.end())
		{
			//ERR_MSG(L"키값 오류");
			CSpawn_Manager::Spawn(L"", pActor, nullptr);
		}
		else
		{
			CSpawn_Manager::Spawn(iter_find->second->wstrPrefabName, pActor, iter_find->second);
		}
		
		
	}

	return S_OK;
}

