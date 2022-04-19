// CamAnimation.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "CamAnimation.h"
#include "afxdialogex.h"
#include "GrobalFunction.h"
#include "EditorToolScene.h"
#include "GameObject.h"
#include "Transform.h"
#include "EaterHeader.h"


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


	mListCtrl.InsertColumn(0, _T("인덱스"), LVCFMT_LEFT, SizeX);

	mListCtrl.InsertColumn(1, _T("위치_X"), LVCFMT_LEFT, Size6);
	mListCtrl.InsertColumn(2, _T("위치_Y"), LVCFMT_LEFT, Size6);
	mListCtrl.InsertColumn(3, _T("위치_Z"), LVCFMT_LEFT, Size6);

	mListCtrl.InsertColumn(4, _T("회전_X"), LVCFMT_LEFT, Size6);
	mListCtrl.InsertColumn(5, _T("회전_Y"), LVCFMT_LEFT, Size6);
	mListCtrl.InsertColumn(6, _T("회전_Z"), LVCFMT_LEFT, Size6);

	CamAnime_slider.SetRange(0, 1);
	CamAnime_slider.SetPos(0);
	NowTime_Eidt.SetWindowTextW(L"0.00");

	AddKeyCount		= 100;
	PlayIndex		= 0;
	isPlay			= false;
	NowSliderPos	= 0;
	OneFrameTime	= 0.01f;
	OriginalIndex	= 0;
	PlayTime		= 1.0f;

	AddKeyCount_Edit.SetWindowTextW(L"100");
	OneFrameTime_Eidt.SetWindowTextW(L"0.01");
	Original_Edit.SetWindowTextW(L"0");
	OriginalMax_Edit.SetWindowTextW(L"0");

	SetTimer(0, (UINT)(1000 * OneFrameTime),NULL);

	return 0;
}

void CamAnimation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, mListCtrl);
	DDX_Control(pDX, IDC_SLIDER1, CamAnime_slider);
	DDX_Control(pDX, IDC_EDIT2, NowTime_Eidt);
	DDX_Control(pDX, IDC_EDIT60, Max_Edit);
	DDX_Control(pDX, IDC_EDIT1, AddKeyCount_Edit);
	DDX_Control(pDX, IDC_EDIT7, OneFrameTime_Eidt);
	DDX_Control(pDX, IDC_EDIT9, Original_Edit);
	DDX_Control(pDX, IDC_SLIDER2, Original_Slider);
	DDX_Control(pDX, IDC_EDIT62, OriginalMax_Edit);
	DDX_Control(pDX, IDC_EDIT16, SaveButton_Edit);
}

BEGIN_MESSAGE_MAP(CamAnimation, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CamAnimation::OnAddKey)
	ON_BN_CLICKED(IDC_BUTTON2, &CamAnimation::OnDeleteKey)
	ON_BN_CLICKED(IDC_BUTTON4, &CamAnimation::OnPlay)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON7, &CamAnimation::OnPlayerButton)
	ON_BN_CLICKED(IDC_BUTTON9, &CamAnimation::OnAllDelete)
	ON_BN_CLICKED(IDC_BUTTON10, &CamAnimation::OnBnChangeKey)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON5, &CamAnimation::OnAddOption)
	ON_BN_CLICKED(IDC_BUTTON13, &CamAnimation::OnSaveButton)
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
	MainCam = EditorToolScene::FindMainCamera();
	MainCamTR = MainCam->GetTransform();

	KeyNode Node;
	Node.Time = (float)NowSliderPos;
	Node.PosX = MainCamTR->Position.x;
	Node.PosY = MainCamTR->Position.y;
	Node.PosZ = MainCamTR->Position.z;

	Node.RotX = MainCamTR->Rotation.x;
	Node.RotY = MainCamTR->Rotation.y;
	Node.RotZ = MainCamTR->Rotation.z;

	
	mListCtrl.InsertItem(0, ChangeToCString((int)KeyList.size()));
	mListCtrl.SetItemText(0, 1, ChangeToCString(Node.PosX));
	mListCtrl.SetItemText(0, 2, ChangeToCString(Node.PosY));
	mListCtrl.SetItemText(0, 3, ChangeToCString(Node.PosZ));
	mListCtrl.SetItemText(0, 4, ChangeToCString(Node.RotX));
	mListCtrl.SetItemText(0, 5, ChangeToCString(Node.RotY));
	mListCtrl.SetItemText(0, 6, ChangeToCString(Node.RotZ));

	
	KeyList.push_back(Node);
	OriginalMax_Edit.SetWindowTextW(ChangeToCString((int)KeyList.size()));
	Original_Slider.SetRange(0, (int)KeyList.size() - 1);
}


void CamAnimation::OnDeleteKey()
{

}


void CamAnimation::OnPlay()
{
	isPlay = true;
	AddKeyList.clear();

	int StartKeySize = (int)KeyList.size();

	float ADD = (1.0f / AddKeyCount);
	for (int i = 0; i < StartKeySize-1; i++)
	{
		float Number = ADD;
		KeyNode StartNode	= KeyList[i];
		KeyNode EndNode		= KeyList[i + 1];

		Vector3 StartPos	= { StartNode.PosX,StartNode.PosY,StartNode.PosZ };
		Vector3 EndPos		= { EndNode.PosX,EndNode.PosY,EndNode.PosZ };
		Vector3 StartRot	= { StartNode.RotX,StartNode.RotY,StartNode.RotZ };
		Vector3 EndRot		= { EndNode.RotX,EndNode.RotY,EndNode.RotZ };

		for (int j = 0; j < AddKeyCount; j++)
		{
			Vector3 PosKey = Vector3::Lerp(StartPos, EndPos, Number);
			Vector3 RotKey = Vector3::Lerp(StartRot, EndRot, Number);

			KeyNode Temp = StartNode;
			Temp.PosX = PosKey.x;
			Temp.PosY = PosKey.y;
			Temp.PosZ = PosKey.z;

			Temp.RotX = RotKey.x;
			Temp.RotY = RotKey.y;
			Temp.RotZ = RotKey.z;
			AddKeyList.push_back(Temp);
			Number += ADD;
		}
	}
	CamAnime_slider.SetRange(0, (int)AddKeyList.size() - 1);
	CamAnime_slider.SetPos(0);
	Max_Edit.SetWindowTextW(ChangeToCString((int)AddKeyList.size() - 1));
	NowTime_Eidt.SetWindowTextW(L"0");
}


void CamAnimation::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (AddKeyList.size() == 0) { return; }
	CamAnime_slider.SetRange(0, (int)AddKeyList.size()-1);
	
	if (pScrollBar->GetDlgCtrlID() == CamAnime_slider.GetDlgCtrlID())
	{
		NowSliderPos = CamAnime_slider.GetPos();
		NowTime_Eidt.SetWindowTextW(ChangeToCString(NowSliderPos));
		MainCam->GetTransform()->Position = { AddKeyList[NowSliderPos].PosX,AddKeyList[NowSliderPos].PosY ,AddKeyList[NowSliderPos].PosZ};
		MainCam->GetTransform()->Rotation = { AddKeyList[NowSliderPos].RotX,AddKeyList[NowSliderPos].RotY ,AddKeyList[NowSliderPos].RotZ };
	}

	if (pScrollBar->GetDlgCtrlID() == Original_Slider.GetDlgCtrlID())
	{
		OriginalSliderPos = Original_Slider.GetPos();
		Original_Edit.SetWindowTextW(ChangeToCString(OriginalSliderPos));
		MainCam->GetTransform()->Position = { KeyList[OriginalSliderPos].PosX,KeyList[OriginalSliderPos].PosY ,KeyList[OriginalSliderPos].PosZ };
		MainCam->GetTransform()->Rotation = { KeyList[OriginalSliderPos].RotX,KeyList[OriginalSliderPos].RotY ,KeyList[OriginalSliderPos].RotZ };
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CamAnimation::OnPlayerButton()
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


void CamAnimation::OnBnChangeKey()
{
	mListCtrl.DeleteAllItems();
	int Size = (int)KeyList.size();
	for (int i = 0; i < (int)KeyList.size(); i++) 
	{
		AddItem(i,KeyList[i]);
	}
	KeyList[OriginalSliderPos].PosX = MainCamTR->Position.x;
	KeyList[OriginalSliderPos].PosY = MainCamTR->Position.y;
	KeyList[OriginalSliderPos].PosZ = MainCamTR->Position.z;

	KeyList[OriginalSliderPos].RotX = MainCamTR->Rotation.x;
	KeyList[OriginalSliderPos].RotY = MainCamTR->Rotation.y;
	KeyList[OriginalSliderPos].RotZ = MainCamTR->Rotation.z;

	AfxMessageBox(L"변경완료");
}


void CamAnimation::OnTimer(UINT_PTR nIDEvent)
{
	if (AddKeyList.size() == 0) { return; }

	if (nIDEvent == 0 && isPlay == true)
	{
		if (PlayIndex > AddKeyList.size() - 1)
		{
			isPlay = false;
			PlayIndex = 0;
		}
		else
		{
			NowTime_Eidt.SetWindowTextW(ChangeToCString(PlayIndex));
			CamAnime_slider.SetPos(PlayIndex);
			MainCam->GetTransform()->Position = { AddKeyList[PlayIndex].PosX,AddKeyList[PlayIndex].PosY ,AddKeyList[PlayIndex].PosZ };
			MainCam->GetTransform()->Rotation = { AddKeyList[PlayIndex].RotX,AddKeyList[PlayIndex].RotY ,AddKeyList[PlayIndex].RotZ };
			PlayIndex++;
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CamAnimation::OnAddOption()
{
	CString Data;
	AddKeyCount_Edit.GetWindowTextW(Data);
	AddKeyCount = ChangeToInt(Data);

	OneFrameTime_Eidt.GetWindowTextW(Data);
	OneFrameTime = ChangeToFloat(Data);

	KillTimer(0);
	SetTimer(0, (UINT)(1000* OneFrameTime), NULL);
	AfxMessageBox(L"적용완료");
}


void CamAnimation::OnSaveButton()
{
	CString Data;
	SaveButton_Edit.GetWindowTextW(Data);
	if (Data == "")
	{
		AfxMessageBox(L"Error : 저장할 데이터 이름이 없습니다 \n 이름을 입력해주세요");
		return;
	}

	int KeySize = (int)KeyList.size();
	EATER_OPEN_WRITE_FILE(ChangeToString(Data),"../Assets/Model/Animation/",".Eater");
	EATER_SET_NODE("CAM_ANIMATION");

	EATER_SET_MAP("AddKeyCount",ChangeToString(AddKeyCount));
	EATER_SET_MAP("Frame",ChangeToString(OneFrameTime));

	EATER_SET_LIST_START("KEY", KeySize, 6);
	for (int i = 0; i < KeySize; i++)
	{
		EATER_SET_LIST(KeyList[i].PosX);
		EATER_SET_LIST(KeyList[i].PosY);
		EATER_SET_LIST(KeyList[i].PosZ);

		EATER_SET_LIST(KeyList[i].RotX);
		EATER_SET_LIST(KeyList[i].RotY);
		EATER_SET_LIST(KeyList[i].RotZ, true);
	}
	EATER_CLOSE_WRITE_FILE();
	AfxMessageBox(L"성공 : ../Assets/Model/Animation경로에 저장됨");
}
