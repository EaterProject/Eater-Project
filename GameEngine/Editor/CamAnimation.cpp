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

	CamAnime_slider.SetRange(0, 1000);
	CamAnime_slider.SetPos(0);
	NowTime_Eidt.SetWindowTextW(L"0.00");

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
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CamAnimation::OnLvnColumnclickList1)
	ON_BN_CLICKED(IDC_BUTTON7, &CamAnimation::OnBnClickedButton7)
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

	
	KeyList.insert({Node.Time,Node});
}


void CamAnimation::OnDeleteKey()
{

}


void CamAnimation::OnPlay()
{
	
}


void CamAnimation::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->GetDlgCtrlID() == CamAnime_slider.GetDlgCtrlID())
	{
		SliderPos = (float)CamAnime_slider.GetPos() * 0.1f;
		NowTime_Eidt.SetWindowTextW(ChangeToCString(SliderPos));
		

	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CamAnimation::OnLvnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	




	*pResult = 0;
}


void CamAnimation::OnBnClickedButton7()
{
	Demo::ChangeCam();
}
