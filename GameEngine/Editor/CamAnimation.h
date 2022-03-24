#pragma once


// CamAnimation 대화 상자
#include <vector>
class GameObject;
class Transform;
struct KeyNode 
{
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
	void AddItem(int index,KeyNode& Node);

	bool	isPlay = false;		//현재 재생중인지 여부
	int		NowSliderPos;			//슬라이더 위치
	int		OriginalSliderPos;		//원본 슬라이더 위치;
	int		PlayIndex;				//현재 재생중인 인덱스
	int		OriginalIndex;			//보간하지 않은 인덱스
	int		AddKeyCount;			//보간시 추가할 키 개수
	float	OneFrameTime;			//한프레임이 지나가는 속도
public:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CListCtrl mListCtrl;
	CSliderCtrl CamAnime_slider;
	CSliderCtrl Original_Slider;

	CEdit NowTime_Eidt;			//현재 시간을 보여줄 에디터
	CEdit Max_Edit;				//최대 시간을 보여줄 에디터

	CEdit AddKeyCount_Edit;		//추가할 키 개수를 보여줄 에디터
	CEdit OneFrameTime_Eidt;	//한프레임 속도를 보여줄 에디터
	CEdit PlayTime_Edit;		//플레이타임 속도를 보여줄 에디터

	CEdit Original_Edit;		//보간하지 않은 값 인덱스
	CEdit OriginalMax_Edit;		//보간하지 않은 값 Max
	CEdit SaveButton_Edit;		//저장버튼

	GameObject* MainCam;
	Transform* MainCamTR;
	std::vector<KeyNode> KeyList;
	std::vector<KeyNode> AddKeyList;

	afx_msg void OnAddKey();
	afx_msg void OnDeleteKey();
	afx_msg void OnPlay();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPlayerButton();
	afx_msg void OnAllDelete();
	afx_msg void OnBnChangeKey();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnAddOption();
	afx_msg void OnSaveButton();
};
