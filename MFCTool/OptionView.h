#pragma once
#include "afxwin.h"



// COptionView 폼 뷰입니다.
class CHierarchyView;
class CMFCToolView;
class COptionView : public CFormView
{
	DECLARE_DYNCREATE(COptionView)

protected:
	COptionView();           // 동적 만들기에 사용되는 protected 생성자입니다.
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
public:
	void	OnHirerachyTreeCtrlSelectChanged(CString  _cstrActorName);
	void	UpdatePrefabComboBox();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	
	DECLARE_MESSAGE_MAP()
private:
	CHierarchyView*	m_pHierarchyView;
	CMFCToolView* m_pView;
private:
	map<CString, OBJECTINFO*>* m_pmapPrefab;
	map<CString, ACTORINFO*>* m_pmapActorInfo;
public:
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
	CStatic m_PicturePrefab;
	CString m_cstrActorName;
	afx_msg void OnEnKillfocusOptionActorname();
	afx_msg void OnEnKillfocusEditPosy();
	afx_msg void OnEnKillfocusEditScalex();
	afx_msg void OnEnKillfocusEditScaley();
	afx_msg void OnEnKillfocusEditOptionAngle();
};


