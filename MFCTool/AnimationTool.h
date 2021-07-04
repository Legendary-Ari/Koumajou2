#pragma once
#include "afxwin.h"


// CAnimationTool 대화 상자입니다.

class CAnimationTool : public CDialog
{
	DECLARE_DYNAMIC(CAnimationTool)

public:
	CAnimationTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAnimationTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMATIONTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public :
	void SetImageView(CString Objectkey, const RECT& _rect, int Index, const CStatic& PictureBox, CString StateKey = 0 );
protected:
	int DeleteListRectItemAndReName(UINT _index);
public:
	CString m_wstrObject_Key;
	CString m_wstrState_Key;
	float m_fPlay_Speed;
	int m_iDrawID;
	HDROP m_Drop;
	int m_iAnimationCount;
	const int m_TIMERHANDLE;
	ANIMATION* m_pSelectedAnimation;

	afx_msg void OnBnClickedAddRect();
	afx_msg void OnBnClickedSave();
	afx_msg void OnLbnSelchangeAnimation_List();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedAddInfo();
	afx_msg void OnBnClickedDeleteRect();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnLbnSelchangeImageList();


	CStatic m_Start_Image;
	CStatic m_Pic_Rect;
	CStatic m_Pic_Loaded;
	CListBox m_ListBox_Image;
	CListBox m_Animation_ListBox;
	CButton m_Loop;

	map<CString, ANIMATION*> m_mapAnima;
	map<CString, CString> m_mapObjectKeyToPath;

	afx_msg void OnBnClickedDeleteAnimationList();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedStop();
	int m_iPlay_Speed;
	
	virtual BOOL OnInitDialog();
	union {
		struct {
			LONG m_lLeft;
			LONG m_lTop;
			LONG m_lRight;
			LONG m_lBottom;
		};
		RECT m_tRect;
	};
	
	afx_msg void OnDestroy();
	CListBox m_ListBoxRect;
	afx_msg void OnLbnSelchangeListAnimationRect();
	afx_msg void OnBnClickedButtonEditrect();
	
	afx_msg void OnEnKillfocusAnimationTop();
	afx_msg void OnEnKillfocusAnimationRight();
	afx_msg void OnEnKillfocusAnimationLeft();
	afx_msg void OnEnKillfocusAnimationBottom();
	int m_iRectIndex;
};
