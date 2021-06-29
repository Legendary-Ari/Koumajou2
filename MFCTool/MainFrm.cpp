
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "MFCTool.h"

#include "MainFrm.h"
#include "MFCToolView.h"
#include "Miniview.h"
#include "Form.h"
#include "HierarchyView.h"
#include "OptionView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
// 
// 	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
// 		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
// 	{
// 		TRACE0("도구 모음을 만들지 못했습니다.\n");
// 		return -1;      // 만들지 못했습니다.
// 	}
// 
// 	if (!m_wndStatusBar.Create(this))
// 	{
// 		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
// 		return -1;      // 만들지 못했습니다.
// 	}
// 	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
// 
// 	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
// 	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
// 	EnableDocking(CBRS_ALIGN_ANY);
// 	DockControlBar(&m_wndToolBar);
// 

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_tMainSplitter.CreateStatic(this, 1, 3, WS_CHILD | WS_VISIBLE);
	m_tSecondSplitter.CreateStatic(&m_tMainSplitter,2,1, WS_CHILD | WS_VISIBLE, m_tMainSplitter.IdFromRowCol(0, 0)); 
	m_tRightSplitter.CreateStatic(&m_tMainSplitter, 2, 1, WS_CHILD | WS_VISIBLE, m_tMainSplitter.IdFromRowCol(0, 2));

	/*
	0, 0	|	0, 1
	ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
	1, 0	|	1, 1
	*/
	
	m_tMainSplitter.CreateView(0, 1, RUNTIME_CLASS(CMFCToolView), CSize(CLIENTCX, CLIENTCY), pContext); 
	m_tSecondSplitter.CreateView(0,0, RUNTIME_CLASS(CMiniview), CSize(300, 300), pContext);
	m_tSecondSplitter.CreateView(1, 0, RUNTIME_CLASS(CForm), CSize(300, 300), pContext);
	m_tMainSplitter.SetColumnInfo(0, 300, 10);
	m_tRightSplitter.CreateView(0, 0, RUNTIME_CLASS(CHierarchyView), CSize(400, 300), pContext);
	m_tRightSplitter.CreateView(1, 0, RUNTIME_CLASS(COptionView), CSize(200, 300), pContext);
	//return CFrameWnd::OnCreateClient(lpcs, pContext);
	return TRUE;
}
