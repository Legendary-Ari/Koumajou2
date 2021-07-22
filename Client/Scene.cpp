#include "stdafx.h"
#include "Scene.h"

BYTE CScene::m_ePlaying = CScene_Manager::END;
CScene::CScene()
{
}


CScene::~CScene()
{
}

const _vec3 & CScene::Get_StartPos() const
{
	return m_vStartPos;
}
