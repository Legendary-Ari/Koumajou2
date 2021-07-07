// AnimationTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "AnimationTool.h"
#include "afxdialogex.h"


// CAnimationTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAnimationTool, CDialog)

CAnimationTool::CAnimationTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ANIMATIONTOOL, pParent)
	, m_wstrObject_Key(_T("")), m_wstrState_Key(_T(""))
	,m_fPlay_Speed(0.f)
	, m_iDrawID(0), m_iAnimationCount(0)
	, m_iPlay_Speed(0)
	, m_lTop(0)
	, m_lRight(0)
	, m_lBottom(0)
	, m_lLeft(0)
	, m_TIMERHANDLE(10001)
	, m_pSelectedAnimation(nullptr)
	, m_iRectIndex(0)
	, m_bPlay(false)
	, m_iAnimSize(0)
	, m_iWidth(0)
	, m_iHeight(0)
	, m_iHMount(0)
	, m_iStartX(0)
	, m_iStartY(0)
{

}

CAnimationTool::~CAnimationTool()
{

	for (pair<CString, ANIMATION*> rPair : m_mapAnima)
		Safe_Delete(rPair.second);
	m_mapAnima.clear();

}

void CAnimationTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_wstrObject_Key);
	DDX_Text(pDX, IDC_EDIT2, m_wstrState_Key);
	DDX_Text(pDX, IDC_EDIT3, m_fPlay_Speed);

	DDX_Control(pDX, IDC_START_IMAGE, m_Start_Image);
	DDX_Control(pDX, IDC_INDEX_TO_SELECTED_IMAGE, m_Pic_Rect);
	DDX_Control(pDX, IDC_LIST_TO_SELECTED_IMAGE, m_Pic_Loaded);

	DDX_Control(pDX, IDC_LIST2, m_ListBox_Image);
	DDX_Control(pDX, IDC_ANIMATION_LIST, m_Animation_ListBox);

	DDX_Control(pDX, IDC_CHECK1, m_Loop);
	DDX_Text(pDX, IDC_EDIT6, m_iPlay_Speed);
	DDX_Text(pDX, IDC_ANIMATION_TOP, m_lTop);
	DDX_Text(pDX, IDC_ANIMATION_RIGHT, m_lRight);
	DDX_Text(pDX, IDC_ANIMATION_BOTTOM, m_lBottom);
	DDX_Text(pDX, IDC_ANIMATION_LEFT, m_lLeft);
	DDX_Control(pDX, IDC_LIST_ANIMATION_RECT, m_ListBoxRect);
	DDX_Text(pDX, IDC_EDIT_ANIMATION_INDEX, m_iRectIndex);
	DDX_Text(pDX, IDC_ANIM_EDIT_SIZE, m_iAnimSize);
	DDX_Text(pDX, IDC_ANIM_EDIT_WIDTH, m_iWidth);
	DDX_Text(pDX, IDC_ANIM_EDIT_HIEGHT, m_iHeight);
	DDX_Text(pDX, IDC_ANIM_EDIT_HMOUNT, m_iHMount);
	DDX_Text(pDX, IDC_ANIM_EDIT_STARTX, m_iStartX);
	DDX_Text(pDX, IDC_ANIM_EDIT_STARTY, m_iStartY);
}


BEGIN_MESSAGE_MAP(CAnimationTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON3, &CAnimationTool::OnBnClickedAddRect)
	ON_BN_CLICKED(IDC_BUTTON1, &CAnimationTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CAnimationTool::OnBnClickedLoad)
	ON_LBN_SELCHANGE(IDC_LIST2, &CAnimationTool::OnLbnSelchangeImageList)

	ON_LBN_SELCHANGE(IDC_ANIMATION_LIST, &CAnimationTool::OnLbnSelchangeAnimation_List)
	ON_WM_DROPFILES()

	ON_BN_CLICKED(IDC_BUTTON10_ANIM, &CAnimationTool::OnBnClickedAddInfo)
	ON_BN_CLICKED(IDC_BUTTON11, &CAnimationTool::OnBnClickedDeleteRect)
	ON_BN_CLICKED(IDC_BUTTON12, &CAnimationTool::OnBnClickedDeleteAnimationList)

	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON16, &CAnimationTool::OnBnClickedPlay)
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(IDC_LIST_ANIMATION_RECT, &CAnimationTool::OnLbnSelchangeListAnimationRect)
	ON_EN_KILLFOCUS(IDC_ANIMATION_TOP, &CAnimationTool::OnEnKillfocusAnimationTop)
	ON_EN_KILLFOCUS(IDC_ANIMATION_RIGHT, &CAnimationTool::OnEnKillfocusAnimationRight)
	ON_EN_KILLFOCUS(IDC_ANIMATION_LEFT, &CAnimationTool::OnEnKillfocusAnimationLeft)
	ON_EN_KILLFOCUS(IDC_ANIMATION_BOTTOM, &CAnimationTool::OnEnKillfocusAnimationBottom)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_INFO, &CAnimationTool::OnBnClickedButtonEditInfo)
END_MESSAGE_MAP()


// addinfo 버튼 클릭
void CAnimationTool::OnBnClickedAddInfo()//여기 진행중..
{
	UpdateData(TRUE);
	//==========animation 정보 입력=======================

	 

	int iImage = m_ListBox_Image.GetCurSel();
	if (iImage == LB_ERR)
	{
		ERR_MSG(L"이미지 선택해주세요");
		return;
	}
	CString wstrObject_Key;
	m_ListBox_Image.GetText(iImage, wstrObject_Key);
	CString AnimationName = m_wstrObject_Key + m_wstrState_Key; //두개objkey와 statekey를 더한것이 키값
	ANIMATION* pAnima = nullptr;
	auto& iter_Animation_find = m_mapAnima.find(AnimationName);
	int iResult = -1;
	if (iter_Animation_find == m_mapAnima.end())
	{
		pAnima = new ANIMATION{};
		m_pSelectedAnimation = pAnima;
	}
	else
	{
		iResult = AfxMessageBox(_T("수정하시겠습니까?"), MB_YESNO);
		if (iResult == IDYES)
			pAnima = iter_Animation_find->second;
		else
			return;
	}

	CString cstrPath_Key;
	m_ListBox_Image.GetText(iImage, cstrPath_Key);

	pAnima->wstrObjectKey = cstrPath_Key;
	pAnima->wstrStateKey = m_wstrState_Key;
	pAnima->fPlay_Speed = m_fPlay_Speed;
	

	auto& iter_Anim_find = m_mapObjectKeyToPath.find(cstrPath_Key);
	CString cstrPath = iter_Anim_find->second;
	CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::MULTI_TEX, vector<RECT>(), cstrPath.GetString(), m_wstrObject_Key.GetString(), m_wstrState_Key.GetString());
	pAnima->wstrFilePath = iter_Anim_find->second;

	if (m_Loop.GetCheck())
		pAnima->bLoop = true;
	else
		pAnima->bLoop = false;
	//=====================================================
	if (iResult != IDYES)
	{
		m_mapAnima.emplace(AnimationName, pAnima);
		m_Animation_ListBox.AddString(m_wstrObject_Key + L"->" + m_wstrState_Key);
	}

	//------------------Reset Rect--------------------
	m_pSelectedAnimation->vecRect.clear();
	m_pSelectedAnimation->vecRect.reserve(m_iAnimSize);
	m_ListBoxRect.ResetContent();

	//------------------ReAdd Rect---------------------
	for (int i = 0; i < m_iAnimSize; ++i)
	{

		RECT rect =
		{
			m_iStartX + m_iWidth * (i % (m_iHMount )),
			m_iStartY + m_iHeight * (i / (m_iHMount)),
			m_iStartX + m_iWidth * ((i % (m_iHMount)) + 1)-1,
			m_iStartY + m_iHeight * ((i / (m_iHMount)) + 1)-1
		};
		m_pSelectedAnimation->vecRect.emplace_back(rect);
		CString cstrRectName;
		cstrRectName.Format(L"%02d RECT{%d,%d // %d,%d }", i, rect);
		m_ListBoxRect.AddString(cstrRectName);
	}
	
	UpdateData(FALSE);
}


void CAnimationTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateData(TRUE);
	int iFileCount = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);
	TCHAR szFileFullPath[MAX_PATH]{};

	m_Drop = hDropInfo;


	for (int i = 0; i < iFileCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szFileFullPath, MAX_PATH);

		CString wstrRelativePath = CFileInfo::ConvertRelativePath(szFileFullPath);
		CString wstrFileNameAndEx = PathFindFileName(wstrRelativePath);

		TCHAR szFileName[MAX_PATH]{};
		lstrcpy(szFileName, wstrFileNameAndEx.GetString());
		PathRemoveExtension(szFileName);
		if (m_ListBox_Image.FindString(-1, szFileName) != LB_ERR)
			continue;
		m_ListBox_Image.AddString(szFileName);
		CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::SINGLE_TEX, { RECT{} }, wstrRelativePath.GetString(), szFileName);
		m_mapObjectKeyToPath.emplace(szFileName, wstrRelativePath);
	}
	//m_ListBox_Image.SetHorizontalExtent(500);
	UpdateData(FALSE);
	CDialog::OnDropFiles(hDropInfo);

}


//끌어넣은 이미지 리스트 선택 후 확인
void CAnimationTool::OnLbnSelchangeImageList() // 
{
	UpdateData(TRUE);
	int iListCursor = m_ListBox_Image.GetCurSel();
	int iRectCursor = m_ListBoxRect.GetCurSel();
	if (iRectCursor == LB_ERR || iListCursor == LB_ERR)
		return;
	
	CTexture_Manager::Get_Instance()->DrawPic(m_wstrObject_Key.GetString(), RECT(),-1, m_Pic_Loaded, m_wstrState_Key.GetString());
	
}

int CAnimationTool::DeleteListRectItemAndReName(UINT _index)
{
	int iReturn = m_ListBoxRect.DeleteString(_index);
	m_pSelectedAnimation->vecRect.erase(m_pSelectedAnimation->vecRect.begin() + _index);
	OnLbnSelchangeAnimation_List();

	return iReturn;
}

void CAnimationTool::OnBnClickedAddRect()
{
	UpdateData(TRUE);

	int iIndex = m_Animation_ListBox.GetCurSel();
	CString wstrFindName;
	m_Animation_ListBox.GetText(iIndex, wstrFindName);

	// 키에서 -> 문자 삭제 
	int i = 0;
	for (; i < wstrFindName.GetLength(); ++i)
	{
		if (wstrFindName[i] == L'>')
			break;
	}

	wstrFindName.Delete(i - 1, 2);

	auto& iter_find = m_mapAnima.find(wstrFindName);
	//맵에 키가 없을 때 리턴 
	if (iter_find == m_mapAnima.end())
	{
		ERR_MSG(L"맵에 일치하는 키가 없다");
		return;
	}
	RECT rc{m_lLeft, m_lTop, m_lRight, m_lBottom};
	iter_find->second->vecRect.emplace_back(rc);
	CString cstrRectName;
	cstrRectName.Format(L"%02d RECT{%d,%d // %d,%d }", iter_find->second->vecRect.size()-1, m_lLeft, m_lTop, m_lRight, m_lBottom);
	m_ListBoxRect.AddString(cstrRectName);
	m_ListBoxRect.SetCurSel(m_ListBoxRect.GetTopIndex());
	OnLbnSelchangeListAnimationRect();
	m_lLeft = 0;
	m_lTop = 0;
	m_lRight = 0;
	m_lBottom = 0;
	UpdateData(FALSE);
}

void CAnimationTool::OnBnClickedDeleteRect()
{
	int iIndex = m_ListBoxRect.GetCurSel();
	if (iIndex == LB_ERR)
	{
		return;
	}
	DeleteListRectItemAndReName(iIndex);
	ZeroMemory(&m_tRect, sizeof(RECT));
	UpdateData(FALSE);
}


void CAnimationTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strFilePath = L"../Data/AnimationData.dat";
	HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		CFileDialog Dlg(FALSE,// FALSE가 다른이름으로 저장. 
			L"dat",
			L"*.dat",
			OFN_OVERWRITEPROMPT);
		TCHAR szFilePath[MAX_PATH]{};

		GetCurrentDirectory(MAX_PATH, szFilePath);
		PathRemoveFileSpec(szFilePath);

		lstrcat(szFilePath, L"\\Data");
		Dlg.m_ofn.lpstrInitialDir = szFilePath;
		if (IDOK != Dlg.DoModal())
		{
			ERR_MSG(L"파일열기 실패");
			return;
		}
		strFilePath = Dlg.GetPathName();
		hFile = CreateFile(strFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	}
	
	//하나씩 저장 해야지. 폴문 돌면서 . 
	DWORD dwbyte = 0;
	DWORD dwStringSize = 0;
	// 난 쓸수 있어. 너넨 쓰지마 . 
	for (auto& rPair : m_mapAnima)
	{

		dwStringSize = (rPair.second->wstrObjectKey.GetLength() + 1) * sizeof(TCHAR);
		WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
		WriteFile(hFile, rPair.second->wstrObjectKey.GetString(), dwStringSize, &dwbyte, nullptr);

		dwStringSize = (rPair.second->wstrStateKey.GetLength() + 1) * sizeof(TCHAR);
		WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
		WriteFile(hFile, rPair.second->wstrStateKey.GetString(), dwStringSize, &dwbyte, nullptr);

		dwStringSize = (rPair.second->wstrFilePath.GetLength() + 1) * sizeof(TCHAR);
		WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
		WriteFile(hFile, rPair.second->wstrFilePath.GetString(), dwStringSize, &dwbyte, nullptr);


		WriteFile(hFile, &rPair.second->fPlay_Speed, sizeof(float), &dwbyte, nullptr);
		WriteFile(hFile, &rPair.second->bLoop, sizeof(bool), &dwbyte, nullptr);

		UINT iRectSize = rPair.second->vecRect.size();
		WriteFile(hFile, &iRectSize, sizeof(UINT), &dwbyte, nullptr);
		for (auto& rect : rPair.second->vecRect)
		{
			WriteFile(hFile, &rect, sizeof(RECT), &dwbyte, nullptr);
		}
	}
	CloseHandle(hFile);

}


void CAnimationTool::OnBnClickedLoad()
{

	CString strFilePath = _T("../Data/AnimationData.dat");
	HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		CFileDialog Dlg(TRUE,// TRUE면 열기. 
			L"dat",
			L"*.dat",
			OFN_OVERWRITEPROMPT);
		
		TCHAR szFilePath[MAX_PATH]{};
		GetCurrentDirectory(MAX_PATH, szFilePath);
		PathRemoveFileSpec(szFilePath);
		lstrcat(szFilePath, L"\\Data");
		Dlg.m_ofn.lpstrInitialDir = szFilePath;

		if (IDOK != Dlg.DoModal())
			return;
		
		strFilePath = Dlg.GetPathName();
		hFile = CreateFile(strFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	}
	for (auto& rPair : m_mapAnima)
		Safe_Delete(rPair.second);
	m_mapAnima.clear();
	m_Animation_ListBox.ResetContent(); 

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
		ReadFile(hFile, &pAnimaInfo->bLoop, sizeof(bool), &dwbyte, nullptr);

		UINT iRectSize = 0;
		ReadFile(hFile, &iRectSize, sizeof(UINT), &dwbyte, nullptr);
		pAnimaInfo->vecRect.reserve(iRectSize);
		for (UINT i=0; i<iRectSize; ++i)
		{
			RECT rect;
			ReadFile(hFile, &rect, sizeof(RECT), &dwbyte, nullptr);
			pAnimaInfo->vecRect.emplace_back(rect);
		}

		m_mapAnima.emplace(pAnimaInfo->wstrObjectKey + pAnimaInfo->wstrStateKey, pAnimaInfo);
		m_Animation_ListBox.AddString(pAnimaInfo->wstrObjectKey + "->" + pAnimaInfo->wstrStateKey);
		m_mapObjectKeyToPath.emplace(pAnimaInfo->wstrObjectKey, pAnimaInfo->wstrFilePath);
		CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::MULTI_TEX, pAnimaInfo->vecRect, pAnimaInfo->wstrFilePath.GetString(), pAnimaInfo->wstrObjectKey.GetString(), pAnimaInfo->wstrStateKey.GetString());
	}

	//	//==============싱글 텍스쳐 파일 이름 셋팅===============
	//	CString wstrSingleFilePath = pAnimaInfo->wstrFilePath;

	//	//===============멀티 텍스쳐 파일 이름 셋팅==============
	//	CString wstrFileNameAndEx = PathFindFileName(wstrSingleFilePath);
	//	CString wstrMultiFilePath = wstrSingleFilePath;

	//	TCHAR szFileName[MAX_PATH]{};
	//	lstrcpy(szFileName, wstrFileNameAndEx.GetString());
	//	int iPathLength = wstrMultiFilePath.GetLength() - lstrlen(szFileName);
	//	wstrMultiFilePath.Delete(iPathLength, lstrlen(szFileName));
	//	PathRemoveExtension(szFileName);

	//	lstrcat(szFileName, L".png");

	//	wstrMultiFilePath.Insert(iPathLength, szFileName);


	//	//===============이미지 Insert==============


	//		if (pAnimaInfo->wstrObjectKey.GetString() != L""&& pAnimaInfo->wstrStateKey.GetString() != L"")
	//		{
	//			if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::MULTI_TEX, wstrMultiFilePath.GetString(), pAnimaInfo->wstrObjectKey.GetString(), pAnimaInfo->wstrStateKey.GetString(), pAnimaInfo->iMax_Index)))
	//			{
	//				ERR_MSG(L"멀티 텍스쳐 실패");
	//				return;
	//			}
	//		}

	//}
	CloseHandle(hFile);
}


//완성된 애니메이션 리스트박스 확인
void CAnimationTool::OnLbnSelchangeAnimation_List()
{
	UpdateData(TRUE);

	int iIndex = m_Animation_ListBox.GetCurSel();
	CString wstrFindName;
	m_Animation_ListBox.GetText(iIndex, wstrFindName);

	// 키에서 -> 문자 삭제 
	int i = 0;
	for (; i < wstrFindName.GetLength(); ++i)
	{
		if (wstrFindName[i] == L'>')
			break;
	}

	wstrFindName.Delete(i-1, 2);
	auto& iter_find = m_mapAnima.find(wstrFindName);
	m_pSelectedAnimation = iter_find->second;
	m_wstrObject_Key = m_pSelectedAnimation->wstrObjectKey;
	m_wstrState_Key = m_pSelectedAnimation->wstrStateKey;
	m_fPlay_Speed = m_pSelectedAnimation->fPlay_Speed;

	
	if (m_pSelectedAnimation->bLoop)
		m_Loop.SetCheck(true);
	else
		m_Loop.SetCheck(false);

	int iImage = m_ListBox_Image.FindString(-1, m_pSelectedAnimation->wstrObjectKey);
	if (iImage == LB_ERR)
	{
		CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::MULTI_TEX, m_pSelectedAnimation->vecRect, m_pSelectedAnimation->wstrFilePath.GetString(), m_pSelectedAnimation->wstrObjectKey.GetString(), m_pSelectedAnimation->wstrStateKey.GetString());
		m_mapObjectKeyToPath.emplace(m_pSelectedAnimation->wstrObjectKey, m_pSelectedAnimation->wstrFilePath);
		m_ListBox_Image.AddString(m_pSelectedAnimation->wstrObjectKey);
		iImage = m_ListBox_Image.GetTopIndex();
	}
	m_ListBox_Image.SetCurSel(iImage);

	m_ListBoxRect.ResetContent();
	int iRect = 0;
	for (auto& rect : m_pSelectedAnimation->vecRect)
	{
		CString cstrRectName;
		cstrRectName.Format(L"%02d RECT{%d,%d // %d,%d }", iRect++, rect.left, rect.top, rect.right, rect.bottom);
		m_ListBoxRect.AddString(cstrRectName);
	}
	if(m_ListBoxRect.GetCount() != 0)
		m_ListBoxRect.SetCurSel(0);

	CTexture_Manager::Get_Instance()->DrawPic(m_pSelectedAnimation->wstrObjectKey.GetString(), RECT(), -1, m_Pic_Loaded, m_pSelectedAnimation->wstrStateKey.GetString());
	if(m_ListBoxRect.GetCount() != 0)
		CTexture_Manager::Get_Instance()->DrawPic(m_pSelectedAnimation->wstrObjectKey.GetString(), RECT(), -1, m_Start_Image, m_pSelectedAnimation->wstrStateKey.GetString());
	else
		CTexture_Manager::Get_Instance()->DrawPic(m_pSelectedAnimation->wstrObjectKey.GetString(), m_pSelectedAnimation->vecRect[0], 0, m_Start_Image, m_pSelectedAnimation->wstrStateKey.GetString());
	OnLbnSelchangeListAnimationRect();

	UpdateData(FALSE);
}

void CAnimationTool::OnBnClickedDeleteAnimationList()
{
	UpdateData(TRUE);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iIndex = m_Animation_ListBox.GetCurSel();
	if (iIndex == LB_ERR)
	{
		ERR_MSG(L"리스트 먼저 선택해");
		return;
	}
	CString wstrFindName;
	m_Animation_ListBox.GetText(iIndex, wstrFindName);
	int i = 0;
	// "->" 문자 삭제
	for (; i < wstrFindName.GetLength(); ++i)
	{
		if (wstrFindName[i] == L'>')
			break;
	}
	wstrFindName.Delete(i - 1, 2);

	auto& iter_find = m_mapAnima.find(wstrFindName);

	Safe_Delete(iter_find->second);
	m_mapAnima.erase(iter_find);

	m_Animation_ListBox.DeleteString(iIndex);
	m_pSelectedAnimation = iter_find->second;

	UpdateData(FALSE);
}

void CAnimationTool::OnTimer(UINT_PTR nIDEvent)
{
	if (m_TIMERHANDLE == nIDEvent)
	{
		if (++m_iAnimationCount >= m_pSelectedAnimation->vecRect.size())
			m_iAnimationCount = 0;
		CTexture_Manager::Get_Instance()->DrawPic(m_pSelectedAnimation->wstrObjectKey, m_pSelectedAnimation->vecRect[m_iAnimationCount], m_iAnimationCount, m_Pic_Rect, m_pSelectedAnimation->wstrStateKey);
		SetTimer(m_TIMERHANDLE, m_pSelectedAnimation->fPlay_Speed * 1000.f, NULL);
	}
	CDialog::OnTimer(nIDEvent);
}


void CAnimationTool::OnBnClickedPlay()
{
	UpdateData(TRUE);
	if (m_bPlay)
	{
		KillTimer(m_TIMERHANDLE);
		GetDlgItem(IDC_BUTTON16)->SetWindowText(L"Play");
	}		
	else
	{
		SetTimer(m_TIMERHANDLE, m_pSelectedAnimation->fPlay_Speed * 1000.f, NULL);
		GetDlgItem(IDC_BUTTON16)->SetWindowText(L"Stop");
	}
		
	m_bPlay = !m_bPlay;
	UpdateData(FALSE);
}


BOOL CAnimationTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnBnClickedLoad();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CAnimationTool::OnDestroy()
{
	int iResult = AfxMessageBox(L"애니메이션을 저장하시겠습니까?", MB_YESNO);
	if (iResult == IDYES)
		OnBnClickedSave();

	CDialog::OnDestroy();
}


void CAnimationTool::OnLbnSelchangeListAnimationRect()
{
	int iIndex = m_ListBoxRect.GetCurSel();
	if (iIndex == LB_ERR)
	{
		return;
	}

	m_tRect = m_pSelectedAnimation->vecRect[iIndex];

	CTexture_Manager::Get_Instance()->DrawPic(m_pSelectedAnimation->wstrObjectKey.GetString(), m_pSelectedAnimation->vecRect[iIndex], iIndex, m_Pic_Rect, m_pSelectedAnimation->wstrStateKey.GetString());


	m_iRectIndex = iIndex;

	UpdateData(FALSE);
}

void CAnimationTool::OnBnClickedButtonEditrect()
{
	UpdateData(TRUE);
	int iIndex = m_ListBoxRect.GetCurSel();
	if (iIndex == LB_ERR)
	{
		return;
	}
	CString cstrRectName;
	m_pSelectedAnimation->vecRect[iIndex] = m_tRect;
	m_ListBoxRect.ResetContent();
	int iRect = 0;
	for (auto& rect : m_pSelectedAnimation->vecRect)
	{
		CString cstrRectName;
		cstrRectName.Format(L"%02d RECT{%d,%d // %d,%d }", iRect++, rect.left, rect.top, rect.right, rect.bottom);
		m_ListBoxRect.AddString(cstrRectName);
	}
	m_ListBoxRect.SetCurSel(iIndex);
}


void CAnimationTool::OnEnKillfocusAnimationTop()
{
	OnBnClickedButtonEditrect();
}


void CAnimationTool::OnEnKillfocusAnimationRight()
{
	OnBnClickedButtonEditrect();
}


void CAnimationTool::OnEnKillfocusAnimationLeft()
{
	OnBnClickedButtonEditrect();
}


void CAnimationTool::OnEnKillfocusAnimationBottom()
{
	OnBnClickedButtonEditrect();
}


void CAnimationTool::OnBnClickedButtonEditInfo()
{
	int iAnimationSelected = m_Animation_ListBox.GetCurSel();
	if (iAnimationSelected == LB_ERR)
	{
		ERR_MSG(L"수정할 anim을 선택해 주세요");
	}

	UpdateData(TRUE);
	m_pSelectedAnimation->wstrStateKey = m_wstrState_Key;
	m_pSelectedAnimation->fPlay_Speed = m_fPlay_Speed;
	m_pSelectedAnimation->bLoop = m_Loop.GetCheck();
	UpdateData(FALSE);
}
