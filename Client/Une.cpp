#include "stdafx.h"
#include "Une.h"


CUne::CUne()
	:m_iMaxAtkMount(12)
{
}


CUne::~CUne()
{
}

CGameObject * CUne::Create(const ACTORINFO * _pActorInfo, const OBJECTINFO * _pObjectInfo)
{
	return nullptr;
}

HRESULT CUne::Ready_GameObject()
{
	return E_NOTIMPL;
}

int CUne::Update_GameObject()
{
	return 0;
}

void CUne::Late_Update_GameObject()
{
}

void CUne::Render_GameObject()
{
}

void CUne::UpdateBodyCollision()
{
}

void CUne::UpdatePattern()
{
}

void CUne::UpdateState()
{
}

void CUne::UpdateAnimation()
{
}

void CUne::UpdateDie()
{
}
