#pragma once



// COptionView �� ���Դϴ�.

class COptionView : public CFormView
{
	DECLARE_DYNCREATE(COptionView)

protected:
	COptionView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~COptionView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPTIONVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1Place();
};


