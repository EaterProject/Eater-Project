// CTAP_2.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "CTAP_Animation.h"
#include "afxdialogex.h"
#include "EditorData.h"
#include "AnimationController.h"
#include "GameObject.h"
#include "DialogFactory.h"
#include "EditorManager.h"

// CTAP_2 대화 상자

IMPLEMENT_DYNAMIC(CTAP_Animation, CustomDialog)

CTAP_Animation::CTAP_Animation(CWnd* pParent /*=nullptr*/)
	: CustomDialog(IDD_TAP_ANIMATION, pParent)
{

}

CTAP_Animation::~CTAP_Animation()
{

}

void CTAP_Animation::SetGameObject(AnimationController* Data)
{
	AC = Data;
	AnimationList.ResetContent();
	std::vector<std::string> NameData;
	Data->GetAnimationList(&NameData);
	
	int Count = (int)NameData.size();
	for (int i = 0; i < Count; i++)
	{
		AnimationList.AddString(ChangeToCString(NameData[i]));
	}

	AnimationList.SetCurSel(0);

	if (NameData.size() != 0)
	{
		Data->Choice(NameData[0]);
	}

	
	int EndFrame = AC->GetEndFrame();
	AnimationEndEdit.SetWindowText(ChangeToCString(EndFrame));
	AnimationStartEdit.SetWindowTextW(L"0");
	Loop_Check.SetCheck(true);
}

void CTAP_Animation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, AnimationList);
	DDX_Control(pDX, IDC_EDIT1, AnimationEndEdit);
	DDX_Control(pDX, IDC_EDIT4, AnimationStartEdit);
	DDX_Control(pDX, IDC_EDIT5, Animation_Speed_Edit);
	DDX_Control(pDX, IDC_CHECK1, Loop_Check);
	DDX_Control(pDX, IDC_EDIT2, NowAnimationName_Edit);
	DDX_Control(pDX, IDC_SLIDER1, AnimationBar_Slider);
	DDX_Control(pDX, IDC_EDIT3, NowAnimationFrame_Edit);
	DDX_Control(pDX, IDC_EDIT8, NowAnimationTime_Edit);
	DDX_Control(pDX, IDC_EDIT6, EventTimeMin_Eidt);
	DDX_Control(pDX, IDC_EDIT7, EventTimeMax_Eidt);
}

BEGIN_MESSAGE_MAP(CTAP_Animation, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CTAP_Animation::OnCbnSelchangeCombo1)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON5, &CTAP_Animation::OnPlayButton)
	ON_BN_CLICKED(IDC_BUTTON6, &CTAP_Animation::OnPauseButton)
	ON_BN_CLICKED(IDC_BUTTON8, &CTAP_Animation::OnStopButton)
	ON_BN_CLICKED(IDC_CHECK1, &CTAP_Animation::OnLoopCheck)
	ON_BN_CLICKED(IDC_BUTTON1, &CTAP_Animation::OnMinFrameTime)
	ON_BN_CLICKED(IDC_BUTTON2, &CTAP_Animation::OnMaxFrameTime)
	ON_BN_CLICKED(IDC_BUTTON27, &CTAP_Animation::OnEventTimeSave)
END_MESSAGE_MAP()

BOOL CTAP_Animation::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:
			//GetObjectData();
			return TRUE;
		default:
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CTAP_Animation::OnCbnSelchangeCombo1()
{
	AnimationList.GetLBText(AnimationList.GetCurSel(), AnimationName);
	AC->Choice(ChangeToString(AnimationName));
	
	NowAnimationName_Edit.SetWindowTextW(AnimationName);
	int End = AC->GetEndFrame();
	//float Time = AC->GetFrameTime();
	AnimationEndEdit.SetWindowTextW(ChangeToCString(End));
	AnimationBar_Slider.SetRange(0, End);
	AnimationBar_Slider.SetPos(0);
}


void CTAP_Animation::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->GetDlgCtrlID() == AnimationBar_Slider.GetDlgCtrlID())
	{
		int FramePoint = AnimationBar_Slider.GetPos();
		NowAnimationFrame_Edit.SetWindowTextW(ChangeToCString(FramePoint));
		AC->SetFrame(FramePoint);
		FrameTime = AC->GetFrameTime();
		NowAnimationTime_Edit.SetWindowTextW(ChangeToCString(FrameTime));
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CTAP_Animation::OnPlayButton()
{
	AC->Play();
}


void CTAP_Animation::OnPauseButton()
{
	AC->Pause();
}


void CTAP_Animation::OnStopButton()
{
	AC->Stop();
}


void CTAP_Animation::OnLoopCheck()
{
	Loop ^= true;
	AC->Choice(ChangeToString(AnimationName),Speed,Loop);
}


void CTAP_Animation::OnMinFrameTime()
{
	FrameMin_Time = FrameTime;
	EventTimeMin_Eidt.SetWindowTextW(ChangeToCString(FrameTime));
}

void CTAP_Animation::OnMaxFrameTime()
{
	FrameMax_Time = FrameTime;
	EventTimeMax_Eidt.SetWindowTextW(ChangeToCString(FrameTime));
}


void CTAP_Animation::OnEventTimeSave()
{
	std::string ObjectName		= AC->gameobject->Name;
	std::string AnimationName	= AC->GetNowAnimationName();

	std::string FileName	= "../Assets/Model/Animation/" + ObjectName + AnimationName + ".Eater";
	EditorManager* mManager = DialogFactory::GetFactory()->GetEditorManager();
	//mManager->ConvertData(FileName, ObjectName+AnimationName,CHANGE_TYPE::ANIMATION);
	mManager->ConvertAnimationData(FileName, ObjectName + AnimationName, FrameMin_Time, FrameMax_Time);
	AfxMessageBox(L"애니메이션 Min,Max 변환 완료");
}
