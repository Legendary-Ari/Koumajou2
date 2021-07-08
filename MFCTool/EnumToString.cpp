#include "stdafx.h"
#include "EnumToString.h"

const CString CEnumToString::szRenderId[RENDERID::END] =
{
	_T("BACKGROUND"),
	_T("BACKCOLLISION"),
	_T("OBJECT"),
	_T("PAWN"),
	_T("EFFECT"),
	_T("UI")
};
const CString CEnumToString::szObjId[tagOBJECTINFO::OBJID_END] =
{
	_T("BACKGROUND"),
	_T("PLAYER"),
	_T("ENEMY"),
	_T("BOSS"),
	_T("PLAYER_BULLET"),
	_T("ENEMY_BULLET"),
	_T("EFFECT"),
	_T("UI"),
};
const CString CEnumToString::szBulletType[tagOBJECTINFO::TYPE_END] =
{
	_T("NORMAL"),
	_T("HOMING")
};

CEnumToString::CEnumToString()
{
}


CEnumToString::~CEnumToString()
{
}
