// CTAP_2.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "CTAP_Animation.h"
#include "afxdialogex.h"
#include "EditorData.h"
#include "AnimationController.h"


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
}

//AnimationData CTAP_Animation::GetObjectData()
//{
//	//AnimationData Data;
//	//CString str;
//	//AnimationList.GetLBText(AnimationList.GetCurSel(), str);
//	//Data.NowAnimation = GetDataString(str);
//	//return Data;
//}

void CTAP_Animation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, AnimationList);
	DDX_Control(pDX, IDC_EDIT1, AnimationEndEdit);
	DDX_Control(pDX, IDC_EDIT4, AnimationStartEdit);
}

BEGIN_MESSAGE_MAP(CTAP_Animation, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CTAP_Animation::OnCbnSelchangeCombo1)
	ON_WM_HSCROLL()
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
	CString AnimationName;
	AnimationList.GetLBText(AnimationList.GetCurSel(), AnimationName);
	AC->Choice(ChangeToString(AnimationName));
}


void CTAP_Animation::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//if (pScrollBar->GetDlgCtrlID() == Rotation_X_Slider.GetDlgCtrlID())
	//{
	//	ObjectTransform->Rotation.x = (float)Rotation_X_Slider.GetPos();
	//	Rot_X.SetWindowTextW(ChangeToCString(ObjectTransform->Rotation.x));
	//}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
