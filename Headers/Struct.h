#pragma once
typedef struct tagTEXINFO
{
	LPDIRECT3DTEXTURE9 pTexture; // HBITMAP ?? 
	D3DXIMAGE_INFO tImageInfo; // 이미지에 대한 정보를 가지고 있을 변수 . 
}TEXINFO;
typedef struct tagTile
{
	D3DXVECTOR3 vPos; 
	D3DXVECTOR3 vSize; 
	BYTE byDrawID; 
	BYTE byOption; 
}TILE;

typedef struct tagUnitInfo
{
	// 다시한번 강조하지만 저장할 안에 포인터 있으면 어떻게 될지 다시 생각해봐. 
#ifdef _AFX
	CString wstrPrefabName; 
#else
	wstring wstrPrefabName; 
#endif // _AFX
	int		iAtt; 
	int		iDef; 
	BYTE	byJob; 
	BYTE	byItem; 
}UNITINFO;

typedef struct tagInfo
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vSize;

	float		fAngle;
	//D3DXVECTOR2; 
	//D3DXVECTOR4; 

}INFO;

typedef struct tagFRect
{
	float	left;
	float	top;
	float	right;
	float	bottom;
}FRECT;

typedef struct tagOBJECTINFO

{
	enum OBJID { BACKGROUND, PLAYER, ENEMY, BOSS, PLAYER_BULLET, ENEMY_BULLET, EFFECT, UI, OBJID_END };
	enum BULLET_TYPE { NORMAL, HOMING, TYPE_END };
#ifdef _AFX
	CString cstrName;
	CString cstrObjectImage_ObjectKey;
	CString cstrObjectImage_Path;
	CString cstrIdleAnimImage_ObjectKey;
	CString cstrIdleAnimImage_StateKey;
#else
	wstring wstrPrefabName;
	wstring wstrObjectImage_ObjectKey;
	wstring wstrObjectImage_Path;
	wstring cstrIdleAnimImage_ObjectKey;
	wstring wstrIdleAnimImage_StateKey;
#endif
	float	fMaxHp;
	float	fAtk;
	float	fAtkRatio;
	float	fMoveSpeed;
	BYTE	eObjId;
	BYTE	eRenderId;
	bool	bIsSingle;
// IfSingle
	RECT	tFRect;
//Bullet
	bool	bDestructable;
	BYTE	eBulletType;

}OBJECTINFO;




typedef struct tagAnimationInfo
{
#ifdef _AFX
	CString wstrObjectKey;
	CString wstrStateKey;
	CString	wstrFilePath;
#else
	wstring wstrObjectKey;
	wstring wstrStateKey;
	wstring wstrFilePath;
#endif

	vector<RECT> vecRect;
	float	fPlay_Speed;
	bool	bLoop;
	
}ANIMATION;

typedef struct tagActorInfo
{	

	INFO	tInfo;
	bool	bIsFolder;
#ifdef _AFX
	CString wstrPrefabName;
	CString wstrActorName;

#else
	wstring wstrPrefabName;
	wstring wstrActorName;
#endif
}ACTORINFO;

typedef struct tagCollision
{
	enum ID {C_RECT, C_SPHERE};
	FRECT	tFRect;
	ID		eId;
}COLLISION;