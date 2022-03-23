#pragma once


// CamAnimation 대화 상자
#include <map>
class GameObject;
struct KeyNode 
{
	int index	= 0;
	float Time	= 0;
	float PosX  = 0;
	float PosY  = 0;
	float PosZ  = 0;
	float RotX	= 0;
	float RotY	= 0;
	float RotZ	= 0;
};

class CamAnimation : public CDialogEx
{
	DECLARE_DYNAMIC(CamAnimation)

public:
	CamAnimation(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CamAnimation();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAM_ANIMATION };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
public:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CListCtrl mListCtrl;
	CSliderCtrl CamAnime_slider;
	CEdit NowTime_Eidt;
	float SliderPos;

	GameObject* MainCam;
	std::map<float, KeyNode> KeyList;


	afx_msg void OnAddKey();
	afx_msg void OnDeleteKey();
	afx_msg void OnPlay();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLvnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton7();
};
