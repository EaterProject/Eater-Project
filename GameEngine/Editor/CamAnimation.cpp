// CamAnimation.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "CamAnimation.h"
#include "afxdialogex.h"
#include "GrobalFunction.h"
#include "Demo.h"
#include "GameObject.h"
#include "Transform.h"


// CamAnimation 대화 상자

IMPLEMENT_DYNAMIC(CamAnimation, CDialogEx)
#define LERP(prev, next, time) ((prev * (1.0f - time)) + (next * time))
CamAnimation::CamAnimation(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CAM_ANIMATION, pParent)
{

}

CamAnimation::~CamAnimation()
{

}

BOOL CamAnimation::OnInitDialog()
{
	CDialog::OnInitDialog();
	mListCtrl.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);

	CRect ListSize;
	mListCtrl.GetClientRect(&ListSize);

	//컨트롤크기의 반
	int SizeX = ListSize.Width() / 3;

	//컨트롤크기의 반에서 6등분한 크기
	int Size6 = (SizeX*2) / 6;


	mListCtrl.InsertColumn(0, _T("시간"), LVCFMT_LEFT, SizeX);

	mListCtrl.InsertColumn(1, _T("위치_X"), LVCFMT_LEFT, Size6);
	mListCtrl.InsertColumn(2, _T("위치_Y"), LVCFMT_LEFT, Size6);
	mListCtrl.InsertColumn(3, _T("위치_Z"), LVCFMT_LEFT, Size6);

	mListCtrl.InsertColumn(4, _T("회전_X"), LVCFMT_LEFT, Size6);
	mListCtrl.InsertColumn(5, _T("회전_Y"), LVCFMT_LEFT, Size6);
	mListCtrl.InsertColumn(6, _T("회전_Z"), LVCFMT_LEFT, Size6);

	CamAnime_slider.SetRange(0, 1);
	CamAnime_slider.SetPos(0);
	NowTime_Eidt.SetWindowTextW(L"0.00");

	SetTimer(0, 10,NULL);

	return 0;
}

void CamAnimation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, mListCtrl);
	DDX_Control(pDX, IDC_SLIDER1, CamAnime_slider);
	DDX_Control(pDX, IDC_EDIT2, NowTime_Eidt);
}

BEGIN_MESSAGE_MAP(CamAnimation, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CamAnimation::OnAddKey)
	ON_BN_CLICKED(IDC_BUTTON2, &CamAnimation::OnDeleteKey)
	ON_BN_CLICKED(IDC_BUTTON4, &CamAnimation::OnPlay)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON7, &CamAnimation::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON9, &CamAnimation::OnAllDelete)
	ON_BN_CLICKED(IDC_BUTTON10, &CamAnimation::OnBnClickedButton10)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CamAnimation 메시지 처리기
BOOL CamAnimation::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:



			return TRUE;
		default:
			break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CamAnimation::AddItem(int index, KeyNode& Node)
{
	mListCtrl.InsertItem(index, ChangeToCString(index));
	mListCtrl.SetItemText(index, 1, ChangeToCString(Node.PosX));
	mListCtrl.SetItemText(index, 2, ChangeToCString(Node.PosY));
	mListCtrl.SetItemText(index, 3, ChangeToCString(Node.PosZ));
	mListCtrl.SetItemText(index, 4, ChangeToCString(Node.RotX));
	mListCtrl.SetItemText(index, 5, ChangeToCString(Node.RotY));
	mListCtrl.SetItemText(index, 6, ChangeToCString(Node.RotZ));
}

void CamAnimation::OnAddKey()
{
	MainCam = Demo::GetCamera();
	Transform* CamTR = MainCam->GetTransform();

	KeyNode Node;
	Node.index = 0;
	Node.Time = SliderPos;
	Node.PosX = CamTR->Position.x;
	Node.PosY = CamTR->Position.y;
	Node.PosZ = CamTR->Position.z;

	Node.RotX = CamTR->Rotation.x;
	Node.RotY = CamTR->Rotation.y;
	Node.RotZ = CamTR->Rotation.z;

	
	mListCtrl.InsertItem(0, ChangeToCString(SliderPos));
	mListCtrl.SetItemText(0, 1, ChangeToCString(Node.PosX));
	mListCtrl.SetItemText(0, 2, ChangeToCString(Node.PosY));
	mListCtrl.SetItemText(0, 3, ChangeToCString(Node.PosZ));
	mListCtrl.SetItemText(0, 4, ChangeToCString(Node.RotX));
	mListCtrl.SetItemText(0, 5, ChangeToCString(Node.RotY));
	mListCtrl.SetItemText(0, 6, ChangeToCString(Node.RotZ));

	
	KeyList.push_back(Node);
}


void CamAnimation::OnDeleteKey()
{

}


void CamAnimation::OnPlay()
{
	isPlay = true;
}


void CamAnimation::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CamAnime_slider.SetRange(0, (int)AddKeyList.size()-1);

	if (pScrollBar->GetDlgCtrlID() == CamAnime_slider.GetDlgCtrlID())
	{
		SliderPos = CamAnime_slider.GetPos();
		NowTime_Eidt.SetWindowTextW(ChangeToCString(SliderPos));
		MainCam->GetTransform()->Position = { AddKeyList[SliderPos].PosX,AddKeyList[SliderPos].PosY ,AddKeyList[SliderPos].PosZ};
		MainCam->GetTransform()->Rotation = { AddKeyList[SliderPos].RotX,AddKeyList[SliderPos].RotY ,AddKeyList[SliderPos].RotZ };
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CamAnimation::OnBnClickedButton7()
{
	PlayIndex = 0;
	isPlay = false;
}


void CamAnimation::OnAllDelete()
{
	mListCtrl.DeleteAllItems();
	CamAnime_slider.SetRange(0, 0);
	KeyList.clear();
}


void CamAnimation::OnBnClickedButton10()
{
	int LerpCount = 100;
	mListCtrl.DeleteAllItems();
	AddKeyList.clear();

	int StartKeySize = KeyList.size();
	
	float ADD = (1.0f / LerpCount);
	for (int i = 0; i < StartKeySize-2; i++)
	{
		float Number = ADD;
		KeyNode StartNode = KeyList[i];
		KeyNode EndNode = KeyList[i + 1];

		Vector3 StartPos = { StartNode.PosX,StartNode.PosY,StartNode.PosZ };
		Vector3 EndPos = { EndNode.PosX,EndNode.PosY,EndNode.PosZ };

		Vector3 StartRot = { StartNode.RotX,StartNode.RotY,StartNode.RotZ };
		Vector3 EndRot = { EndNode.RotX,EndNode.RotY,EndNode.RotZ };

		AddKeyList.push_back(StartNode);
		AddItem((int)AddKeyList.size() - 1, StartNode);
		for (int j = 0; j < LerpCount-2; j++)
		{
			Vector3 PosKey = Vector3::Lerp(StartPos, EndPos, Number);
			Vector3 RotKey = Vector3::Lerp(StartRot, EndRot, Number);

			KeyNode Temp = StartNode;
			Temp.PosX = PosKey.x;
			Temp.PosY = PosKey.y;
			Temp.PosZ = PosKey.z;

			Temp.RotX = RotKey.z;
			Temp.RotY = RotKey.y;
			Temp.RotZ = RotKey.z;
			AddKeyList.push_back(Temp);
			AddItem((int)AddKeyList.size() - 1, Temp);

			Number += ADD;
		}
		AddKeyList.push_back(EndNode);
		AddItem((int)AddKeyList.size() - 1, EndNode);
	}
}


void CamAnimation::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 0 && isPlay == true)
	{
		if (PlayIndex >= AddKeyList.size() - 1)
		{
			isPlay = false;
			PlayIndex = 0;
		}
		else
		{
			NowTime_Eidt.SetWindowTextW(ChangeToCString(PlayIndex));
			MainCam->GetTransform()->Position = { AddKeyList[PlayIndex].PosX,AddKeyList[PlayIndex].PosY ,AddKeyList[PlayIndex].PosZ };
			MainCam->GetTransform()->Rotation = { AddKeyList[PlayIndex].RotX,AddKeyList[PlayIndex].RotY ,AddKeyList[PlayIndex].RotZ };
			PlayIndex++;
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}
