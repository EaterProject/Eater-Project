// CTAP_Rigidbody.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "CTAP_Rigidbody.h"
#include "afxdialogex.h"
#include "Rigidbody.h"


// CTAP_Rigidbody 대화 상자

IMPLEMENT_DYNAMIC(CTAP_Rigidbody, CustomDialog)

CTAP_Rigidbody::CTAP_Rigidbody(CWnd* pParent /*=nullptr*/)
	: CustomDialog(IDD_TAP_RIGIDBODY, pParent)
{

}

CTAP_Rigidbody::~CTAP_Rigidbody()
{
}

void CTAP_Rigidbody::SetGameObject(Rigidbody* rigidbody)
{
	mRigidbody = rigidbody;
	
	Gravity_Check.SetCheck(rigidbody->GetGravity());
	Kinematic_Click.SetCheck(rigidbody->GetKinematic());

	//bool PosX, PosY, PosZ;
	//bool RotX, RotY, RotZ;
	Vector3 FreezePos = rigidbody->GetFreezePosition();
	Vector3 FreezeRot = rigidbody->GetFreezeRotation();

	Mass_Edit.SetWindowTextW(ChangeToCString(rigidbody->GetMass()));
	for (int i = 0; i < 6; i++)
	{
		switch (i)
		{
		case 0:
			Freeze_PosX_Click.SetCheck((int)FreezePos.x);
			break;
		case 1:
			Freeze_PosY_Click.SetCheck((int)FreezePos.y);
			break;
		case 2:
			Freeze_PosZ_Click.SetCheck((int)FreezePos.z);
			break;
		case 3:
			Freeze_RotX_Click.SetCheck((int)FreezeRot.x);
			break;
		case 4:
			Freeze_RotY_Click.SetCheck((int)FreezeRot.y);
			break;
		case 5:
			Freeze_RotZ_Click.SetCheck((int)FreezeRot.z);
			break;
		}
	}
}

void CTAP_Rigidbody::UpdateObject()
{
	int Gravity		=  Gravity_Check.GetCheck();
	int Kinematic	= Kinematic_Click.GetCheck();

	
	Freeze[0] =Freeze_PosX_Click.GetCheck();
	Freeze[1] =Freeze_PosY_Click.GetCheck();
	Freeze[2] =Freeze_PosZ_Click.GetCheck();

	Freeze[3] = Freeze_RotX_Click.GetCheck();
	Freeze[4] = Freeze_RotY_Click.GetCheck();
	Freeze[5] = Freeze_RotZ_Click.GetCheck();

	CString Number;
	Mass_Edit.GetWindowTextW(Number);
	mRigidbody->SetMass(ChangeToFloat(Number));
	mRigidbody->SetGravity(Gravity);
	mRigidbody->SetKinematic(Kinematic);
}

void CTAP_Rigidbody::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT3, Mass_Edit);
	DDX_Control(pDX, IDC_CHECK9, Kinematic_Click);
	DDX_Control(pDX, IDC_CHECK1, Freeze_PosX_Click);
	DDX_Control(pDX, IDC_CHECK2, Freeze_PosY_Click);
	DDX_Control(pDX, IDC_CHECK3, Freeze_PosZ_Click);
	DDX_Control(pDX, IDC_CHECK4, Freeze_RotX_Click);
	DDX_Control(pDX, IDC_CHECK5, Freeze_RotY_Click);
	DDX_Control(pDX, IDC_CHECK6, Freeze_RotZ_Click);
	DDX_Control(pDX, IDC_CHECK11, Gravity_Check);
}


BEGIN_MESSAGE_MAP(CTAP_Rigidbody, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK11, &CTAP_Rigidbody::OnGravityButton)
	ON_BN_CLICKED(IDC_CHECK9, &CTAP_Rigidbody::OnKinematicButton)
	ON_BN_CLICKED(IDC_CHECK1, &CTAP_Rigidbody::OnFreezePosX_Button)
	ON_BN_CLICKED(IDC_CHECK2, &CTAP_Rigidbody::OnOnFreezePosY_Button)
	ON_BN_CLICKED(IDC_CHECK3, &CTAP_Rigidbody::OnFreezePosZ_Button)
	ON_BN_CLICKED(IDC_CHECK4, &CTAP_Rigidbody::OnFreezeRotX_Button)
	ON_BN_CLICKED(IDC_CHECK5, &CTAP_Rigidbody::OnFreezeRotY_Button)
	ON_BN_CLICKED(IDC_CHECK6, &CTAP_Rigidbody::OnFreezeRotZ_Button)
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
			UpdateObject();
			return TRUE;
		default:
			break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CTAP_Rigidbody::OnGravityButton()
{
	int Gravity = Gravity_Check.GetCheck();
	mRigidbody->SetGravity(Gravity);
}


void CTAP_Rigidbody::OnKinematicButton()
{
	int Kinematic = Kinematic_Click.GetCheck();
	mRigidbody->SetKinematic(Kinematic);
}


void CTAP_Rigidbody::OnFreezePosX_Button()
{
	Freeze[0] = Freeze_PosX_Click.GetCheck();
	Freeze[1] = Freeze_PosY_Click.GetCheck();
	Freeze[2] = Freeze_PosZ_Click.GetCheck();
	mRigidbody->SetFreezePosition(Freeze[0], Freeze[1], Freeze[2]);
}


void CTAP_Rigidbody::OnOnFreezePosY_Button()
{
	Freeze[0] = Freeze_PosX_Click.GetCheck();
	Freeze[1] = Freeze_PosY_Click.GetCheck();
	Freeze[2] = Freeze_PosZ_Click.GetCheck();
	mRigidbody->SetFreezePosition(Freeze[0], Freeze[1], Freeze[2]);
}


void CTAP_Rigidbody::OnFreezePosZ_Button()
{
	Freeze[0] = Freeze_PosX_Click.GetCheck();
	Freeze[1] = Freeze_PosY_Click.GetCheck();
	Freeze[2] = Freeze_PosZ_Click.GetCheck();
	mRigidbody->SetFreezePosition(Freeze[0], Freeze[1], Freeze[2]);
}


void CTAP_Rigidbody::OnFreezeRotX_Button()
{
	Freeze[3] = Freeze_RotX_Click.GetCheck();
	Freeze[4] = Freeze_RotY_Click.GetCheck();
	Freeze[5] = Freeze_RotZ_Click.GetCheck();
	mRigidbody->SetFreezePosition(Freeze[3], Freeze[4], Freeze[5]);
}


void CTAP_Rigidbody::OnFreezeRotY_Button()
{
	Freeze[3] = Freeze_RotX_Click.GetCheck();
	Freeze[4] = Freeze_RotY_Click.GetCheck();
	Freeze[5] = Freeze_RotZ_Click.GetCheck();
	mRigidbody->SetFreezePosition(Freeze[3], Freeze[4], Freeze[5]);
}


void CTAP_Rigidbody::OnFreezeRotZ_Button()
{
	Freeze[3] = Freeze_RotX_Click.GetCheck();
	Freeze[4] = Freeze_RotY_Click.GetCheck();
	Freeze[5] = Freeze_RotZ_Click.GetCheck();
	mRigidbody->SetFreezePosition(Freeze[3], Freeze[4], Freeze[5]);
}
