#pragma once


// CTAP_2 대화 상자
#include "EditorData.h"
#include "CustomDialog.h"
class AnimationController;
class CTAP_Animation : public CustomDialog
{
	DECLARE_DYNAMIC(CTAP_Animation)

public:
	CTAP_Animation(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTAP_Animation();

	void SetGameObject(AnimationController* Data);
	

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAP_ANIMATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	AnimationController* AC;

	CSliderCtrl AnimationBar_Slider;
	CComboBox AnimationList;
	CString AnimationName;
	CEdit AnimationEndEdit;
	CEdit AnimationStartEdit;
	CEdit Animation_Speed_Edit;
	CButton Loop_Check;
	CEdit NowAnimationName_Edit;
	CEdit NowAnimationFrame_Edit;
	CEdit NowAnimationTime_Edit;
	CEdit EventTimeMin_Eidt;
	CEdit EventTimeMax_Eidt;

	int Speed = 1;
	bool  Loop = true;
	float FrameTime		= 0;
	float FrameMin_Time = 0;
	float FrameMax_Time = 0;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPlayButton();
	afx_msg void OnPauseButton();
	afx_msg void OnStopButton();
	afx_msg void OnLoopCheck();
	afx_msg void OnMinFrameTime();
	afx_msg void OnMaxFrameTime();
	afx_msg void OnEventTimeSave();
};
