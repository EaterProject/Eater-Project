// CTAP_Rigidbody.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "CTAP_Rigidbody.h"
#include "afxdialogex.h"
#include "Rigidbody.h"


// CTAP_Rigidbody 대화 상자

IMPLEMENT_DYNAMIC(CTAP_Rigidbody, CDialogEx)

CTAP_Rigidbody::CTAP_Rigidbody(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAP_RIGIDBODY, pParent)
{

}

CTAP_Rigidbody::~CTAP_Rigidbody()
{
}

void CTAP_Rigidbody::SetGameObject(Rigidbody* rigidbody)
{
	mRigidbody = rigidbody;
	
	bool PosX, PosY, PosZ;
	PosX = rigidbody;

	//Freeze_PosX_Click.SetCheck()

}

void CTAP_Rigidbody::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, Gravity_Edit);
	DDX_Control(pDX, IDC_EDIT3, Mass_Edit);
	DDX_Control(pDX, IDC_CHECK9, Kinematic_Click);
	DDX_Control(pDX, IDC_CHECK1, Freeze_PosX_Click);
	DDX_Control(pDX, IDC_CHECK2, Freeze_PosY_Click);
	DDX_Control(pDX, IDC_CHECK3, Freeze_PosZ_Click);
	DDX_Control(pDX, IDC_CHECK4, Freeze_RotX_Click);
	DDX_Control(pDX, IDC_CHECK5, Freeze_RotY_Click);
	DDX_Control(pDX, IDC_CHECK6, Freeze_RotZ_Click);
}


BEGIN_MESSAGE_MAP(CTAP_Rigidbody, CDialogEx)
END_MESSAGE_MAP()


// CTAP_Rigidbody 메시지 처리기


BOOL CTAP_Rigidbody::PreTranslateMessage(MSG* pMsg)
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
