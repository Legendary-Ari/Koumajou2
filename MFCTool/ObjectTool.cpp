// ObjectTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "ObjectTool.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "Form.h"


// CObjectTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CObjectTool, CDialog)

CObjectTool::CObjectTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_OBJECTTOOL, pParent)
	, m_cstrName(_T(""))
	, m_iShotGunCount(0)
	, m_fShotGunAngle(0.f)
	, m_fMaxHp(0.f)
	, m_fObjAtk(0.f)
	, m_fObjAtkRatio(0.f)
	, m_fObjMoveSpeed(0.f)
	, m_bIsPlaying_DeathAnimation(false)
	, m_iDeathAnimationIndex(0)
	, m_lLeft(0)
	, m_lTop(0)
	, m_lRight(0)
	, m_lBottom(0)
	, m_pObjectInfoSelected(nullptr)
{

}

CObjectTool::~CObjectTool()
{
	for (auto& rPair : m_mapObject)
		Safe_Delete(rPair.second);
	m_mapObject.clear();
}

void CObjectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OBJNAME, m_cstrName);
	DDX_Control(pDX, IDC_CHECK1_BULLET, m_CheckBoxDestructable);
	DDX_Control(pDX, IDC_LIST2_OBJ, m_ListBox_ObjectList);
	DDX_Control(pDX, IDC_LIST3_OBJ, m_ListBox_AnimList);
	DDX_Control(pDX, IDC_ANIM_PIC, m_PictureAnimation);
	DDX_Text(pDX, IDC_EDIT_OBJHP, m_fMaxHp);
	DDX_Text(pDX, IDC_EDIT_OBJATK, m_fObjAtk);
	DDX_Text(pDX, IDC_EDIT_OBJATKSPD, m_fObjAtkRatio);
	DDX_Text(pDX, IDC_EDIT_OBJMOVSPD, m_fObjMoveSpeed);
	DDX_Control(pDX, IDC_COMBO_OBJID, m_ComboOBJID);
	DDX_Control(pDX, IDC_BUTTON_OBJ_PLAYSTOP, m_ButtonAnimationPlay);
	DDX_Control(pDX, IDC_PIC_OBJECT, m_PictureObject);
	DDX_Control(pDX, IDC_COMBO_BULLET_TYPE, m_BulletTypeSelectControl);
	DDX_Control(pDX, IDC_LIST_OBJPIC, m_ListBox_ObjImage);
	DDX_Control(pDX, IDC_COMBO_RENDERID, m_ComboRenderId);
	DDX_Control(pDX, IDC_RADIO_STATIC, m_RadioImageType[0]);
	DDX_Control(pDX, IDC_RADIO_ANIMATION, m_RadioImageType[1]);
	DDX_Text(pDX, IDC_EDIT_OBJECT_LEFT, m_lLeft);
	DDX_Text(pDX, IDC_EDIT_OBJECT_TOP, m_lTop);
	DDX_Text(pDX, IDC_EDIT_OBJECT_RIGHT, m_lRight);
	DDX_Text(pDX, IDC_EDIT_OBJECT_BOTTOM, m_lBottom);
	DDX_Control(pDX, IDC_PIC_RECT, m_Pic_Rect);
}

void CObjectTool::Update_Group_Bullet()
{
	int iIndex = m_ComboOBJID.GetCurSel();

	if (iIndex == OBJECTINFO::ENEMY_BULLET || iIndex == OBJECTINFO::PLAYER_BULLET)
	{
		GetDlgItem(IDC_GROUP_BULLET)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_COMBO_BULLET_TYPE)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_CHECK1_BULLET)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_TEXT_BULLET_TYPE)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_TEXT_DESTRUCTABLE)->ShowWindow(SW_NORMAL);
	}
	else
	{
		GetDlgItem(IDC_GROUP_BULLET)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_BULLET_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK1_BULLET)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TEXT_BULLET_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TEXT_DESTRUCTABLE)->ShowWindow(SW_HIDE);
	}

}

BEGIN_MESSAGE_MAP(CObjectTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_OBJADD, &CObjectTool::OnBnClickedAdd)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST2_OBJ, &CObjectTool::OnLbnSelchangeObjectList)
	ON_BN_CLICKED(IDC_BUTTON_OBJDEL, &CObjectTool::OnBnClickedButtonObjectDelete)
	ON_BN_CLICKED(IDC_BUTTON_OBJSAVE, &CObjectTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON_OBJLOAD, &CObjectTool::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_OBJLOADANIM, &CObjectTool::OnBnClickedButtonLoadAnimation)
	ON_LBN_SELCHANGE(IDC_LIST3_OBJ, &CObjectTool::OnLbnSelchangeAnimation)
	ON_BN_CLICKED(IDC_BUTTON_OBJ_PLAYSTOP, &CObjectTool::OnBnClickedButtonObjPlaystop)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_OBJID, &CObjectTool::OnCbnSelchangeComboObjid)
	ON_LBN_SELCHANGE(IDC_LIST_OBJPIC, &CObjectTool::OnLbnSelchangeListObjpic)
	ON_BN_CLICKED(IDC_RADIO_STATIC, &CObjectTool::OnBnClickedRadioStatic)
	ON_BN_CLICKED(IDC_RADIO_ANIMATION, &CObjectTool::OnBnClickedRadioAnimation)
	ON_EN_KILLFOCUS(IDC_EDIT_OBJECT_TOP, &CObjectTool::OnEnKillfocusEditObjectTop)
	ON_EN_KILLFOCUS(IDC_EDIT_OBJECT_LEFT, &CObjectTool::OnEnKillfocusEditObjectLeft)
	ON_EN_KILLFOCUS(IDC_EDIT_OBJECT_RIGHT, &CObjectTool::OnEnKillfocusEditObjectRight)
	ON_EN_KILLFOCUS(IDC_EDIT_OBJECT_BOTTOM, &CObjectTool::OnEnKillfocusEditObjectBottom)
END_MESSAGE_MAP()


// CObjectTool 메시지 처리기입니다.

//Bullet Add
void CObjectTool::OnBnClickedAdd()
{
	UpdateData(TRUE);
	if (m_cstrName.IsEmpty())
	{
		ERR_MSG(L"이름을 채워넣으세요");
		return;
	}

	int iObjID = m_ComboOBJID.GetCurSel();
	if (iObjID == CB_ERR)
	{
		ERR_MSG(L"오브젝트 타입을 선택해주세요");
		return;
	}

	int iImage = m_ListBox_ObjImage.GetCurSel();


	int iDeathAnimationIndex = m_ListBox_AnimList.GetCurSel();
	if (iDeathAnimationIndex == LB_ERR)
	{
		//ERR_MSG(L"사망 애니메이션을 선택해주세요");
		//return;
	}

	int iRenderId = m_ComboRenderId.GetCurSel();
	if (iRenderId == CB_ERR)
	{
		ERR_MSG(L"RenderId를 선택해 주세요");
		return;
	}

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
	const map< CString, ANIMATION*>& map = pForm->m_tAnimationTool.m_mapAnima;

	CString cstrName;
	if(iDeathAnimationIndex != -1)
		m_ListBox_AnimList.GetText(iDeathAnimationIndex, cstrName);
	int i = 0;
	// "->" 문자 삭제
	for (; i < cstrName.GetLength(); ++i)
	{
		if (cstrName[i] == L'>')
			break;
	}
	cstrName.Delete(i - 1, 2);
	auto& iter_find = map.find(cstrName);
	if (iter_find == map.end())
	{
		//ERR_MSG(L"애니메이션을 찾지 못했습니다.");
		//return;
	}
	cstrName = L"";

	int iBulletTypeIndex = m_BulletTypeSelectControl.GetCurSel();

	if (iBulletTypeIndex == CB_ERR )
	{
		iBulletTypeIndex = 2;
	}

	OBJECTINFO*	pObjectData = new OBJECTINFO{};
	pObjectData->cstrName = m_cstrName;
	pObjectData->fMaxHp = m_fMaxHp;
	pObjectData->fAtk = m_fObjAtk;
	pObjectData->fAtkRatio = m_fObjAtkRatio;
	pObjectData->fMoveSpeed = m_fObjMoveSpeed;
	pObjectData->eObjId = (OBJECTINFO::OBJID)iObjID;
	pObjectData->eRenderId = (RENDERID::ID)iRenderId;
	if (iImage == LB_ERR)
	{
		pObjectData->cstrObjectImage_ObjectKey = L"";
		pObjectData->cstrObjectImage_Path = L"";
	}
	else
	{
		m_ListBox_ObjImage.GetText(iImage, cstrName);
		auto& iter_find = m_mapKeyToPath.find(cstrName);
		if (iter_find == m_mapKeyToPath.end())
		{
			pObjectData->cstrObjectImage_ObjectKey = L"";
			pObjectData->cstrObjectImage_Path = L"";
		}
		else
		{
			pObjectData->cstrObjectImage_ObjectKey = cstrName;
			pObjectData->cstrObjectImage_Path = iter_find->second;
		}
		cstrName = L"";
	}
	if (iter_find != map.end())
	{
		pObjectData->cstrIdleAnimImage_ObjectKey = iter_find->second->wstrObjectKey;
		pObjectData->cstrIdleAnimImage_StateKey = iter_find->second->wstrStateKey;
	}
	else
	{
		pObjectData->cstrIdleAnimImage_ObjectKey = L"";
		pObjectData->cstrIdleAnimImage_StateKey = L"";
	}
	
	pObjectData->bIsSingle = m_RadioImageType[0].GetCheck();

	pObjectData->bDestructable = (bool)m_CheckBoxDestructable.GetCheck();
	pObjectData->eBulletType = (OBJECTINFO::BULLET_TYPE)iBulletTypeIndex;
	pObjectData->tRect = m_tRect;

	auto& iter = m_mapObject.find(m_cstrName);
	if (iter != m_mapObject.end())
	{
		Safe_Delete(iter->second);
		m_mapObject.erase(iter);
	}
	else
	{
		m_ListBox_ObjectList.AddString(m_cstrName);
	}
		
	m_mapObject.emplace(m_cstrName, pObjectData); // Add는 수정도 가능

}


void CObjectTool::OnDropFiles(HDROP hDropInfo)
{
	UpdateData(TRUE);
	int iDropCount = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);
	TCHAR szFileFullPath[MAX_PATH]{};
	
	for (int i = 0; i < iDropCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szFileFullPath, MAX_PATH);
		CString wstrRelativePath = CFileInfo::ConvertRelativePath(szFileFullPath);
		CString wstrFileNameAndEx = PathFindFileName(wstrRelativePath);
		TCHAR szFileName[MAX_PATH]{};
		lstrcpy(szFileName, wstrFileNameAndEx.GetString());
		PathRemoveExtension(szFileName);
		auto& iter_find = m_mapKeyToPath.find(szFileName);
		if (iter_find != m_mapKeyToPath.end())
			continue;
		m_ListBox_ObjImage.AddString(szFileName);
		CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::SINGLE_TEX, {RECT()}, wstrRelativePath.GetString(), szFileName);
		m_mapKeyToPath.emplace(szFileName, wstrRelativePath);
	}
	UpdateData(FALSE);
	CDialog::OnDropFiles(hDropInfo);
}

void CObjectTool::OnLbnSelchangeObjectList()
{
	OnBnClickedButtonLoadAnimation();

	int iObjectIndex = m_ListBox_ObjectList.GetCurSel();

	CString cstrName;
	m_ListBox_ObjectList.GetText(iObjectIndex, cstrName);

	auto& iter = m_mapObject.find(cstrName);
	if (iter == m_mapObject.end())
		return;

	m_pObjectInfoSelected = iter->second;
		
	m_cstrName = m_pObjectInfoSelected->cstrName;
	m_fMaxHp = m_pObjectInfoSelected->fMaxHp;
	m_fObjAtk = m_pObjectInfoSelected->fAtk;
	m_fObjAtkRatio = m_pObjectInfoSelected->fAtkRatio;
	m_fObjMoveSpeed = m_pObjectInfoSelected->fMoveSpeed;
	m_ComboOBJID.SetCurSel(m_pObjectInfoSelected->eObjId);
	m_ComboRenderId.SetCurSel(m_pObjectInfoSelected->eRenderId);
	m_tRect = m_pObjectInfoSelected->tRect;

	int findIndex = m_ListBox_AnimList.FindStringExact(-1, m_pObjectInfoSelected->cstrIdleAnimImage_ObjectKey + L"->" + m_pObjectInfoSelected->cstrIdleAnimImage_StateKey);
	if (findIndex != -1)
	{
		m_ListBox_AnimList.SetCurSel(findIndex);
		OnLbnSelchangeAnimation();
	}
		

	findIndex = m_ListBox_ObjImage.FindStringExact(-1, m_pObjectInfoSelected->cstrObjectImage_ObjectKey);
	if (findIndex != -1)
	{
		m_ListBox_ObjImage.SetCurSel(findIndex);
		OnLbnSelchangeListObjpic();
	}
		

	m_BulletTypeSelectControl.SetCurSel(m_pObjectInfoSelected->eBulletType);
	
	m_CheckBoxDestructable.SetCheck(m_pObjectInfoSelected->bDestructable);
	m_RadioImageType[0].SetCheck(m_pObjectInfoSelected->bIsSingle);
	m_RadioImageType[1].SetCheck(!m_pObjectInfoSelected->bIsSingle);
	if (m_pObjectInfoSelected->bIsSingle)
		OnBnClickedRadioStatic();
	else
		OnBnClickedRadioAnimation();
	Update_Group_Bullet();


	CTexture_Manager::Get_Instance()->DrawPic(m_pObjectInfoSelected->cstrObjectImage_ObjectKey, m_pObjectInfoSelected->tRect, 0, m_PictureObject);

	UpdateData(FALSE);
}


void CObjectTool::OnBnClickedButtonObjectDelete()
{
	int iIndex = m_ListBox_ObjectList.GetCurSel();
	
	if (iIndex == LB_ERR)
	{
		ERR_MSG(L"삭제할 대상을 선택해 주세요");
		return;
	}

	CString cstrName;
	m_ListBox_ObjectList.GetText(iIndex, cstrName);
	
	auto& iter = m_mapObject.find(cstrName);
	if (iter == m_mapObject.end())
	{
		ERR_MSG(L"ListBox에는 존재하지만 Map에는 존재하지 않는 키값입니다");
		return;
	}
	
	Safe_Delete(iter->second);
	m_mapObject.erase(iter);

	m_ListBox_ObjectList.DeleteString(iIndex);
}


void CObjectTool::OnBnClickedSave()
{
		//CString strPath = Dlg.GetFileName(); // 파일의 이름만!
		//strPath = Dlg.GetPathName();// 파일의 경로 + 이름까지 
	CString strFilePath = _T("../Data/ObjectData.dat");
	HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		CFileDialog Dlg(FALSE,// FALSE가 다른이름으로 저장. 
			L"dat",
			L"ObjectData.dat",
			OFN_OVERWRITEPROMPT);
		
		TCHAR szFilePath[MAX_PATH]{};

		GetCurrentDirectory(MAX_PATH, szFilePath);
		PathRemoveFileSpec(szFilePath);

		lstrcat(szFilePath, L"\\Data");
		//PathCombine(szFilePath, szFilePath, L"Data"); 
		Dlg.m_ofn.lpstrInitialDir = szFilePath;
		if (IDOK != Dlg.DoModal())
			return;
		strFilePath = Dlg.GetPathName();
		hFile = CreateFile(strFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	}

	DWORD dwbyte = 0;
	DWORD strLen;
	// 난 쓸수 있어. 너넨 쓰지마 . // 메롱 쓸거지롱
	for (auto& rPair : m_mapObject)
	{
		OBJECTINFO* pObject = rPair.second;
		strLen = pObject->cstrName.GetLength() + 1;
		WriteFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
		WriteFile(hFile, pObject->cstrName.GetString(), sizeof(TCHAR) * strLen, &dwbyte, nullptr);

		strLen = pObject->cstrObjectImage_ObjectKey.GetLength() + 1;
		WriteFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
		WriteFile(hFile, pObject->cstrObjectImage_ObjectKey.GetString(), sizeof(TCHAR) * strLen, &dwbyte, nullptr);

		strLen = pObject->cstrObjectImage_Path.GetLength() + 1;
		WriteFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
		WriteFile(hFile, pObject->cstrObjectImage_Path.GetString(), sizeof(TCHAR) * strLen, &dwbyte, nullptr);

		strLen = pObject->cstrIdleAnimImage_ObjectKey.GetLength() + 1;
		WriteFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
		WriteFile(hFile, pObject->cstrIdleAnimImage_ObjectKey.GetString(), sizeof(TCHAR) * strLen, &dwbyte, nullptr);

		strLen = pObject->cstrIdleAnimImage_StateKey.GetLength() + 1;
		WriteFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
		WriteFile(hFile, pObject->cstrIdleAnimImage_StateKey.GetString(), sizeof(TCHAR) * strLen, &dwbyte, nullptr);

		WriteFile(hFile, &pObject->bIsSingle, sizeof(bool), &dwbyte, nullptr);
		WriteFile(hFile, &pObject->fMaxHp, sizeof(float), &dwbyte, nullptr);
		WriteFile(hFile, &pObject->fAtk, sizeof(float), &dwbyte, nullptr);
		WriteFile(hFile, &pObject->fAtkRatio, sizeof(float), &dwbyte, nullptr);
		WriteFile(hFile, &pObject->fMoveSpeed, sizeof(float), &dwbyte, nullptr);
		WriteFile(hFile, &pObject->eObjId, sizeof(BYTE), &dwbyte, nullptr);
		WriteFile(hFile, &pObject->eRenderId, sizeof(BYTE), &dwbyte, nullptr);
		WriteFile(hFile, &pObject->tRect, sizeof(RECT), &dwbyte, nullptr);
		WriteFile(hFile, &pObject->bDestructable, sizeof(bool), &dwbyte, nullptr);
		WriteFile(hFile, &pObject->eBulletType, sizeof(BYTE), &dwbyte, nullptr);
	}
	CloseHandle(hFile);

}


void CObjectTool::OnBnClickedButtonLoad()
{
	
	OnBnClickedButtonLoadAnimation();
	if (m_ListBox_AnimList.GetCount() <= 0)
	{
		return;
	}

	//CString strPath = Dlg.GetFileName(); // 파일의 이름만!
	//strPath = Dlg.GetPathName();// 파일의 경로 + 이름까지 
	CString strFilePath = _T("../Data/ObjectData.dat");
	HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		CFileDialog Dlg(TRUE,// FALSE가 다른이름으로 저장. 
			L"dat",
			L"*.dat",
			OFN_OVERWRITEPROMPT);
		
		TCHAR szFilePath[MAX_PATH]{};

		GetCurrentDirectory(MAX_PATH, szFilePath);
		PathRemoveFileSpec(szFilePath);

		lstrcat(szFilePath, L"\\Data");
		//PathCombine(szFilePath, szFilePath, L"Data"); 
		Dlg.m_ofn.lpstrInitialDir = szFilePath;
		if (IDOK != Dlg.DoModal())
			return;
		strFilePath = Dlg.GetPathName();
		hFile = CreateFile(strFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	}

	for (auto& rPair : m_mapObject)
	{
		Safe_Delete(rPair.second);
	}
	m_mapObject.clear();
	m_mapKeyToPath.clear();
	m_ListBox_ObjectList.ResetContent();
	m_ListBox_ObjImage.ResetContent();

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
		pObject->cstrName = pBuff;
		Safe_Delete(pBuff);

		ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
		pBuff = new TCHAR[strLen]{};
		ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
		pObject->cstrObjectImage_ObjectKey = pBuff;
		Safe_Delete(pBuff);

		ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
		pBuff = new TCHAR[strLen]{};
		ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
		pObject->cstrObjectImage_Path = pBuff;
		Safe_Delete(pBuff);

		ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
		pBuff = new TCHAR[strLen]{};
		ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
		pObject->cstrIdleAnimImage_ObjectKey = pBuff;
		Safe_Delete(pBuff);

		ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
		pBuff = new TCHAR[strLen]{};
		ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
		pObject->cstrIdleAnimImage_StateKey = pBuff;
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

		m_mapObject.emplace(pObject->cstrName, pObject);
		m_ListBox_ObjectList.AddString(pObject->cstrName);
		m_mapKeyToPath.emplace(pObject->cstrObjectImage_ObjectKey, pObject->cstrObjectImage_Path);
		m_ListBox_ObjImage.AddString(pObject->cstrObjectImage_ObjectKey);
			
		if(pObject->bIsSingle)
			CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::SINGLE_TEX, {RECT()}, pObject->cstrObjectImage_Path.GetString(), pObject->cstrObjectImage_ObjectKey.GetString());
	}
	CloseHandle(hFile);
}


void CObjectTool::OnBnClickedButtonLoadAnimation()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
	const map< CString,ANIMATION*>& map = pForm->m_tAnimationTool.m_mapAnima;
	
	for (auto& rPair : map)
	{
		CString cstrTemp = rPair.second->wstrObjectKey + L"->" + rPair.second->wstrStateKey;
		if(m_ListBox_AnimList.FindStringExact(-1,cstrTemp.GetString()) == -1)
			m_ListBox_AnimList.AddString(cstrTemp);
	}

	//Animation Map 로드코드 구간
}


void CObjectTool::OnLbnSelchangeAnimation()
{
	int iIndex = m_ListBox_AnimList.GetCurSel();

	CString wstrFindName;
	m_ListBox_AnimList.GetText(iIndex, wstrFindName);
	int i = 0;
	// "->" 문자 삭제
	for (; i < wstrFindName.GetLength(); ++i)
	{
		if (wstrFindName[i] == L'>')
			break;
	}
	wstrFindName.Delete(i - 1, 2);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
	const map< CString, ANIMATION*>& map = pForm->m_tAnimationTool.m_mapAnima;

	auto& iter_find = map.find(wstrFindName);

	CGraphic_Device::Get_Instance()->Render_Begin();
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(iter_find->second->wstrObjectKey.GetString(), iter_find->second->wstrStateKey.GetString(), m_iDeathAnimationIndex);
	if (nullptr == pTexInfo)
		return;
	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, WINCX / TILECX, WINCX / TILECX, 0.f);
	D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f);
	matWorld = matScale * matTrans;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CGraphic_Device::Get_Instance()->Render_End(m_PictureAnimation.m_hWnd);
}

void CObjectTool::OnBnClickedButtonObjPlaystop()
{
	int iIndex = m_ListBox_AnimList.GetCurSel();

	if (iIndex == LB_ERR)
		return;

	if (!m_bIsPlaying_DeathAnimation)
	{
		m_bIsPlaying_DeathAnimation = true;
		SetTimer(1000, 0 ,NULL);
		m_ButtonAnimationPlay.SetWindowText(L"Stop");
	}
	else
	{
		m_bIsPlaying_DeathAnimation = false;
		KillTimer(1000);
		m_ButtonAnimationPlay.SetWindowText(L"Play");
	}
}

void CObjectTool::SetSingleVisibility(bool _bVisible)
{
	if (_bVisible)
	{
		GetDlgItem(IDC_GROUP_SINGLE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PIC_RECT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_OBJECT_TOP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_OBJECT_RIGHT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_OBJECT_LEFT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_OBJECT_BOTTOM)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TEXT_TOP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TEXT_LEFT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TEXT_BOTTOM)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TEXT_RIGHT)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_GROUP_SINGLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_PIC_RECT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_OBJECT_TOP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_OBJECT_RIGHT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_OBJECT_LEFT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_OBJECT_BOTTOM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TEXT_TOP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TEXT_LEFT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TEXT_BOTTOM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TEXT_RIGHT)->ShowWindow(SW_HIDE);
	}

}

void CObjectTool::SetMultiVisibility(bool _bVisible)
{
	if (_bVisible)
	{
		GetDlgItem(IDC_GROUP_MULTI)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ANIM_PIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TEXT_IDLEANIM)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_LIST3_OBJ)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_OBJLOADANIM)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_OBJ_PLAYSTOP)->ShowWindow(SW_SHOW);

	}
	else
	{
		GetDlgItem(IDC_GROUP_MULTI)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ANIM_PIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TEXT_IDLEANIM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LIST3_OBJ)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_OBJLOADANIM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_OBJ_PLAYSTOP)->ShowWindow(SW_HIDE);

	}
}


void CObjectTool::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent != 1000)
		return;

	int iIndex = m_ListBox_AnimList.GetCurSel();

	CString wstrFindName;
	m_ListBox_AnimList.GetText(iIndex, wstrFindName);
	int i = 0;
	// "->" 문자 삭제
	for (; i < wstrFindName.GetLength(); ++i)
	{
		if (wstrFindName[i] == L'>')
			break;
	}
	wstrFindName.Delete(i - 1, 2);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
	const map< CString, ANIMATION*>& map = pForm->m_tAnimationTool.m_mapAnima;

	auto& iter_find = map.find(wstrFindName);

	iter_find->second->fPlay_Speed;
	++m_iDeathAnimationIndex;
	if (m_iDeathAnimationIndex >= iter_find->second->vecRect.size())
		m_iDeathAnimationIndex = 0;

	OnLbnSelchangeAnimation();
	SetTimer(1000, iter_find->second->fPlay_Speed * 1000.f, NULL);
	CDialog::OnTimer(nIDEvent);
}


void CObjectTool::OnCbnSelchangeComboObjid()
{
	Update_Group_Bullet();
}


void CObjectTool::OnLbnSelchangeListObjpic()
{

}


BOOL CObjectTool::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	CString cstrFormat;

	m_ComboOBJID.ResetContent();
	for (BYTE i = 0; i < OBJECTINFO::OBJID_END; ++i)
	{
		cstrFormat.Format(_T("%d "), i);
		CString cstrEnum = CEnumToString::szObjId[i];
		m_ComboOBJID.AddString(cstrFormat + cstrEnum);
		cstrFormat.Empty();
	}
	m_ComboRenderId.ResetContent();
	for (BYTE i = 0; i < RENDERID::END; ++i)
	{
		cstrFormat.Format(_T("%d "), i);
		CString cstrEnum = CEnumToString::szRenderId[i];
		m_ComboRenderId.AddString(cstrFormat + cstrEnum);
		cstrFormat.Empty();
	}


	OnBnClickedButtonLoad();

	m_RadioImageType[0].SetCheck(true);
	m_ListBox_ObjectList.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CObjectTool::OnBnClickedRadioStatic()
{
	if (m_RadioImageType[0].GetCheck())
	{
		SetSingleVisibility(true);
		SetMultiVisibility(false);
		if (m_pObjectInfoSelected)
			m_pObjectInfoSelected->bIsSingle = true;
	}
	else if (m_RadioImageType[1].GetCheck())
	{
		SetSingleVisibility(false);
		SetMultiVisibility(true);
		if (m_pObjectInfoSelected)
			m_pObjectInfoSelected->bIsSingle = false;
	}
}


void CObjectTool::OnBnClickedRadioAnimation()
{
	if (m_RadioImageType[0].GetCheck())
	{
		SetSingleVisibility(true);
		SetMultiVisibility(false);
		if (m_pObjectInfoSelected)
			m_pObjectInfoSelected->bIsSingle = true;
	}
	else if (m_RadioImageType[1].GetCheck())
	{
		SetSingleVisibility(false);
		SetMultiVisibility(true);
		if(m_pObjectInfoSelected)
			m_pObjectInfoSelected->bIsSingle = false;
	}
}


void CObjectTool::OnEnKillfocusEditObjectTop()
{
	int iImage = m_ListBox_ObjImage.GetCurSel();
	if (iImage == LB_ERR)
	{
		ERR_MSG(L"Image를 선택해주세요");
		return;
	}
	CString cstrObjectKey;
	m_ListBox_ObjImage.GetText(iImage, cstrObjectKey);

	if (!m_pObjectInfoSelected)
		return;
	UpdateData(TRUE);
	m_pObjectInfoSelected->tRect.top = m_tRect.top;
	UpdateData(FALSE);
	CTexture_Manager::Get_Instance()->DrawPic(cstrObjectKey, m_pObjectInfoSelected->tRect, 0, m_Pic_Rect);
}


void CObjectTool::OnEnKillfocusEditObjectLeft()
{
	int iImage = m_ListBox_ObjImage.GetCurSel();
	if (iImage == LB_ERR)
	{
		ERR_MSG(L"Image를 선택해주세요");
		return;
	}
	CString cstrObjectKey;
	m_ListBox_ObjImage.GetText(iImage, cstrObjectKey);

	if (!m_pObjectInfoSelected)
		return;
	UpdateData(TRUE);
	m_pObjectInfoSelected->tRect.left = m_tRect.left;
	UpdateData(FALSE);
	CTexture_Manager::Get_Instance()->DrawPic(cstrObjectKey, m_pObjectInfoSelected->tRect, 0, m_Pic_Rect);
}


void CObjectTool::OnEnKillfocusEditObjectRight()
{
	int iImage = m_ListBox_ObjImage.GetCurSel();
	if (iImage == LB_ERR)
	{
		ERR_MSG(L"Image를 선택해주세요");
		return;
	}
	CString cstrObjectKey;
	m_ListBox_ObjImage.GetText(iImage, cstrObjectKey);

	if (!m_pObjectInfoSelected)
		return;
	UpdateData(TRUE);
	m_pObjectInfoSelected->tRect.right = m_tRect.right;
	UpdateData(FALSE);
	CTexture_Manager::Get_Instance()->DrawPic(cstrObjectKey, m_pObjectInfoSelected->tRect, 0, m_Pic_Rect);
}


void CObjectTool::OnEnKillfocusEditObjectBottom()
{
	int iImage = m_ListBox_ObjImage.GetCurSel();
	if (iImage == LB_ERR)
	{
		ERR_MSG(L"Image를 선택해주세요");
		return;
	}
	CString cstrObjectKey;
	m_ListBox_ObjImage.GetText(iImage, cstrObjectKey);

	if (!m_pObjectInfoSelected)
		return;
	UpdateData(TRUE);
	m_pObjectInfoSelected->tRect.bottom = m_tRect.bottom;
	UpdateData(FALSE);
	CTexture_Manager::Get_Instance()->DrawPic(cstrObjectKey, m_pObjectInfoSelected->tRect, 0, m_Pic_Rect);
}
