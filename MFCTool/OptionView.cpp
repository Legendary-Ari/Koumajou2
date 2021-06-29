// OptionView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "OptionView.h"


// COptionView

IMPLEMENT_DYNCREATE(COptionView, CFormView)

COptionView::COptionView()
	: CFormView(IDD_OPTIONVIEW)
{

}

COptionView::~COptionView()
{
}

void COptionView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COptionView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_OPTION_APPLY, &COptionView::OnBnClickedButton1Place)
END_MESSAGE_MAP()


// COptionView 진단입니다.

#ifdef _DEBUG
void COptionView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void COptionView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// COptionView 메시지 처리기입니다.


void COptionView::OnBnClickedButton1Place()
{
	
}
