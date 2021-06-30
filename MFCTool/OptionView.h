#pragma once
#include "afxwin.h"



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
private:
	
private:
	map<CString, OBJECTINFO*>* m_pmapPrefab;
	map<CString, ACTORINFO*>* m_pmapActorInfo;
public:
	//afx_msg void OnBnClickedButtonApply();
	afx_msg void OnLbnSelchangeListOptionPrefab();
	afx_msg void OnEnKillfocusEditPosx();
	CListBox m_LB_Prefab;
	float m_fPosX;
	float m_fPosY;
	float m_fScaleX;
	float m_fScaleY;
	float m_fAngle;
	virtual void OnInitialUpdate();
	afx_msg void OnCbnSelchangeComboOptionPrefab();
	CComboBox m_ComboBox_PrefabList;
};


