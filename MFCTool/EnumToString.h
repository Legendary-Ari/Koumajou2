#pragma once
class CEnumToString
{
private:
	CEnumToString();
	~CEnumToString();
public:
	static const CString szRenderId[RENDERID::END];
	static const CString szObjId[tagOBJECTINFO::OBJID_END];
	static const CString szBulletType[tagOBJECTINFO::TYPE_END];
};

