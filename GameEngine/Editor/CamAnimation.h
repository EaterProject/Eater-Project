#pragma once


// CamAnimation 대화 상자
#include <vector>
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
	bool isPlay = false;
	int PlayIndex = 0;
	GameObject* MainCam;
	std::vector<KeyNode> KeyList;
	std::vector<KeyNode> AddKeyList;
	void AddItem(int index,KeyNode& Node);

	afx_msg void OnAddKey();
	afx_msg void OnDeleteKey();
	afx_msg void OnPlay();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButton7();
	afx_msg void OnAllDelete();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
