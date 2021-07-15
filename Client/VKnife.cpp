#include "stdafx.h"
#include "VKnife.h"
#include "Knife.h"

CVKnife::CVKnife()
{
}

CVKnife::~CVKnife()
{
}

CGameObject * CVKnife::Create(OBJECTINFO * _pObjectInfo)
{
	return nullptr;
}

HRESULT CVKnife::Ready_GameObject()
{
	return S_OK;
}

void CVKnife::InitUpdate_GameObject()
{

}

int CVKnife::Update_GameObject()
{
	return 0;
}

void CVKnife::Late_Update_GameObject()
{
}

void CVKnife::Render_GameObject()
{
	
}

void CVKnife::Release_GameObject()
{
}

void CVKnife::Use(const INFO& tInfo)
{
	
}
