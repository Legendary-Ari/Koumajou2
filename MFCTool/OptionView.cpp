// OptionView.cpp : ���� �����Դϴ�.
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


// COptionView �����Դϴ�.

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


// COptionView �޽��� ó�����Դϴ�.


void COptionView::OnBnClickedButton1Place()
{
	
}
