// CTAP_Collider.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "CTAP_Collider.h"
#include "afxdialogex.h"
#include "Collider.h"
#include "PhysCollider.h"
#include "GrobalFunction.h"


// CTAP_Collider 대화 상자

IMPLEMENT_DYNAMIC(CTAP_Collider, CDialogEx)

CTAP_Collider::CTAP_Collider(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAP_COLLIDER, pParent)
{

}

CTAP_Collider::~CTAP_Collider()
{
}

void CTAP_Collider::SetGameObject(Collider* collider)
{
	mPhysCollider	= collider->GetCollider();
	mCollider		= collider;
	
	//Trigger 체크박스 설정
	bool Check = mPhysCollider->GetTrigger();
	Trigger_Check.SetCheck(Check);

	Vector3 Size	= mPhysCollider->GetSize();
	PhysCollider::TYPE type = mPhysCollider->GetType();
	switch (type)
	{
	case PhysCollider::TYPE::BOX:
		ColliderType_Combo.SetCurSel(0);
		BoxType_X_Eidt.SetWindowTextW(ChangeToCString(Size.x));
		BoxType_Y_Eidt.SetWindowTextW(ChangeToCString(Size.y));
		BoxType_Z_Eidt.SetWindowTextW(ChangeToCString(Size.z));

		//처음 타입은 Box 이므로 다른것들은 안보이게
		SphereType_R_Eidt.SendMessage(EM_SETREADONLY, TRUE, 0);
		Capsule_H_Edit.SendMessage(EM_SETREADONLY, TRUE, 0);
		Capsule_R_Edit.SendMessage(EM_SETREADONLY, TRUE, 0);
		break;
	case PhysCollider::TYPE::SPHERE:
		ColliderType_Combo.SetCurSel(1);
		SphereType_R_Eidt.SetWindowTextW(ChangeToCString(Size.x));
		break;
	case PhysCollider::TYPE::CAPSULE:
		ColliderType_Combo.SetCurSel(2);
		Capsule_H_Edit.SetWindowTextW(ChangeToCString(Size.x));
		Capsule_R_Edit.SetWindowTextW(ChangeToCString(Size.y));
		break;
	case PhysCollider::TYPE::MESH:
		ColliderType_Combo.SetCurSel(4);
		break;
	case PhysCollider::TYPE::TERRAIN:
		ColliderType_Combo.SetCurSel(3);
		break;
	}

	Vector3 Center	= mPhysCollider->GetCenter();
	bool isTrigger	= mPhysCollider->GetTrigger();
	
	Center_X_Edit.SetWindowTextW(ChangeToCString(Center.x));
	Center_Y_Edit.SetWindowTextW(ChangeToCString(Center.y));
	Center_Z_Edit.SetWindowTextW(ChangeToCString(Center.z));

	
	Dynamic_Eidt.SetWindowTextW(ChangeToCString(mCollider->GetMaterial_Dynamic()));
	Static_Eidt.SetWindowTextW(ChangeToCString(mCollider->GetMaterial_Static()));
	Restition_Edit.SetWindowTextW(ChangeToCString(mCollider->GetMaterial_Restitution()));

	DynamicFriction_Slider.SetPos((int)(mCollider->GetMaterial_Dynamic() * 100));
	StaticFriction_Slider.SetPos((int)(mCollider->GetMaterial_Static() * 100));
	Restitution_Slider.SetPos((int)(mCollider->GetMaterial_Restitution() * 100));
}

void CTAP_Collider::SetPoint(CString Name)
{
	int num  = ColliderType_Combo.GetCurSel();
	if (num == 3)
	{
		std::string FileName = ChangeToString(Name);
		mCollider->SetTriangleCollider(FileName.substr(0,FileName.rfind('.')));
		TriangleType_Edit.SetWindowTextW(Name);
	}
	else
	{
		MessageBox(L"Collider 타입을 Triangle 로 변경해야 합니다");
	}
}

void CTAP_Collider::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, ColliderType_Combo);
	DDX_Control(pDX, IDC_EDIT1, BoxType_X_Eidt);
	DDX_Control(pDX, IDC_EDIT3, BoxType_Y_Eidt);
	DDX_Control(pDX, IDC_EDIT4, BoxType_Z_Eidt);
	DDX_Control(pDX, IDC_EDIT5, SphereType_R_Eidt);
	DDX_Control(pDX, IDC_EDIT6, Capsule_H_Edit);
	DDX_Control(pDX, IDC_EDIT7, Capsule_R_Edit);
	DDX_Control(pDX, IDC_EDIT8, Center_X_Edit);
	DDX_Control(pDX, IDC_EDIT9, Center_Y_Edit);
	DDX_Control(pDX, IDC_EDIT10, Center_Z_Edit);
	DDX_Control(pDX, IDC_SLIDER2, StaticFriction_Slider);
	DDX_Control(pDX, IDC_SLIDER1, DynamicFriction_Slider);
	DDX_Control(pDX, IDC_SLIDER3, Restitution_Slider);
	DDX_Control(pDX, IDC_EDIT2, Dynamic_Eidt);
	DDX_Control(pDX, IDC_EDIT60, Static_Eidt);
	DDX_Control(pDX, IDC_EDIT63, Restition_Edit);
	DDX_Control(pDX, IDC_CHECK1, Trigger_Check);
	DDX_Control(pDX, IDC_EDIT16, TriangleType_Edit);
}


BEGIN_MESSAGE_MAP(CTAP_Collider, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK1, &CTAP_Collider::OnTriggerCheck)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON1, &CTAP_Collider::OnCreatePhyCollider)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CTAP_Collider::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CTAP_Collider 메시지 처리기


void CTAP_Collider::OnTriggerCheck()
{
	int Check = Trigger_Check.GetCheck();
	mCollider->SetTrigger(Check);
}


BOOL CTAP_Collider::PreTranslateMessage(MSG* pMsg)
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


BOOL CTAP_Collider::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ColliderType_Combo.InsertString(0, L"BOX");
	ColliderType_Combo.InsertString(1, L"SPHERE");
	ColliderType_Combo.InsertString(2, L"CAPSULE");
	ColliderType_Combo.InsertString(3, L"TRIANGLE");
	ColliderType_Combo.SetCurSel(0);

	BoxType_X_Eidt.SetWindowTextW(L"0.5");
	BoxType_Y_Eidt.SetWindowTextW(L"0.5");
	BoxType_Z_Eidt.SetWindowTextW(L"0.5");

	SphereType_R_Eidt.SetWindowTextW(L"0.5");

	Capsule_H_Edit.SetWindowTextW(L"0.5");
	Capsule_R_Edit.SetWindowTextW(L"1.0");

	DynamicFriction_Slider.SetRange(0, 100);
	StaticFriction_Slider.SetRange(0, 100);
	Restitution_Slider.SetRange(0, 100);

	return TRUE;  
}


void CTAP_Collider::UpdateObject()
{
	CString X, Y, Z;

	PhysCollider::TYPE Type = mPhysCollider->GetType();
	switch (Type)
	{
	case PhysCollider::TYPE::BOX:
		BoxType_X_Eidt.GetWindowTextW(X);
		BoxType_Y_Eidt.GetWindowTextW(Y);
		BoxType_Z_Eidt.GetWindowTextW(Z);
		mPhysCollider->SetBoxCollider(ChangeToFloat(X), ChangeToFloat(Y), ChangeToFloat(Z));
		break;
	case PhysCollider::TYPE::SPHERE:
		SphereType_R_Eidt.GetWindowTextW(X);
		mPhysCollider->SetSphereCollider(ChangeToFloat(X));
		break;
	case PhysCollider::TYPE::CAPSULE:
		break;
	}

	bool Check = Trigger_Check.GetCheck();
	mCollider->SetTrigger(Check);

	CString Number;
	CString CenterX, CenterY, CenterZ;

	Center_X_Edit.GetWindowTextW(CenterX);
	Center_Y_Edit.GetWindowTextW(CenterY);
	Center_Z_Edit.GetWindowTextW(CenterZ);

	mCollider->SetCenter(ChangeToFloat(CenterX), ChangeToFloat(CenterY), ChangeToFloat(CenterZ));

	Dynamic_Eidt.GetWindowTextW(Number);
	mCollider->SetMaterial_Dynamic(ChangeToFloat(Number));

	Static_Eidt.GetWindowTextW(Number);
	mCollider->SetMaterial_Static(ChangeToFloat(Number));

	Restition_Edit.GetWindowTextW(Number);
	mCollider->SetMaterial_Restitution(ChangeToFloat(Number));

}

void CTAP_Collider::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->GetDlgCtrlID() == StaticFriction_Slider.GetDlgCtrlID())
	{
		int Pos = StaticFriction_Slider.GetPos();
		Static_Eidt.SetWindowTextW(ChangeToCString((float)Pos * 0.01f));
		mCollider->SetMaterial_Static((float)Pos * 0.01f);
	}

	if (pScrollBar->GetDlgCtrlID() == DynamicFriction_Slider.GetDlgCtrlID())
	{
		int Pos = DynamicFriction_Slider.GetPos();
		Dynamic_Eidt.SetWindowTextW(ChangeToCString((float)Pos * 0.01f));
		mCollider->SetMaterial_Dynamic((float)Pos * 0.01f);
	}

	if (pScrollBar->GetDlgCtrlID() == Restitution_Slider.GetDlgCtrlID())
	{
		int Pos = Restitution_Slider.GetPos();
		Restition_Edit.SetWindowTextW(ChangeToCString((float)Pos * 0.01f));
		mCollider->SetMaterial_Restitution((float)Pos * 0.01f);
	}




	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CTAP_Collider::OnCreatePhyCollider()
{
	UpdateObject();
	mCollider->CreatePhys();
}


void CTAP_Collider::OnCbnSelchangeCombo1()
{
	Vector3 Size = mPhysCollider->GetSize();
	int type = ColliderType_Combo.GetCurSel();
	switch (type)
	{
	case 0:
		BoxType_X_Eidt.SendMessage(EM_SETREADONLY, false, 0);
		BoxType_Y_Eidt.SendMessage(EM_SETREADONLY, false, 0);
		BoxType_Z_Eidt.SendMessage(EM_SETREADONLY, false, 0);
		ColliderType_Combo.SetCurSel(0);
		BoxType_X_Eidt.SetWindowTextW(ChangeToCString(Size.x));
		BoxType_Y_Eidt.SetWindowTextW(ChangeToCString(Size.y));
		BoxType_Z_Eidt.SetWindowTextW(ChangeToCString(Size.z));
		mPhysCollider->SetBoxCollider(Size.x, Size.y, Size.z);


		SphereType_R_Eidt.SendMessage(EM_SETREADONLY, TRUE, 0);
		Capsule_H_Edit.SendMessage(EM_SETREADONLY, TRUE, 0);
		Capsule_R_Edit.SendMessage(EM_SETREADONLY, TRUE, 0);
		break;
	case 1:
		ColliderType_Combo.SetCurSel(1);
		SphereType_R_Eidt.SendMessage(EM_SETREADONLY, false, 0);
		SphereType_R_Eidt.SetWindowTextW(ChangeToCString(Size.x));
		mPhysCollider->SetSphereCollider(Size.x);

		BoxType_X_Eidt.SendMessage(EM_SETREADONLY, TRUE, 0);
		BoxType_Y_Eidt.SendMessage(EM_SETREADONLY, TRUE, 0);
		BoxType_Z_Eidt.SendMessage(EM_SETREADONLY, TRUE, 0);
		Capsule_H_Edit.SendMessage(EM_SETREADONLY, TRUE, 0);
		Capsule_R_Edit.SendMessage(EM_SETREADONLY, TRUE, 0);
		break;
	case 2:
		ColliderType_Combo.SetCurSel(2);
		Capsule_H_Edit.SendMessage(EM_SETREADONLY, false, 0);
		Capsule_R_Edit.SendMessage(EM_SETREADONLY, false, 0);
		Capsule_H_Edit.SetWindowTextW(ChangeToCString(Size.x));
		Capsule_R_Edit.SetWindowTextW(ChangeToCString(Size.y));
		mPhysCollider->SetCapsuleCollider(Size.x,Size.y);

		BoxType_X_Eidt.SendMessage(EM_SETREADONLY, TRUE, 0);
		BoxType_Y_Eidt.SendMessage(EM_SETREADONLY, TRUE, 0);
		BoxType_Z_Eidt.SendMessage(EM_SETREADONLY, TRUE, 0);
		SphereType_R_Eidt.SendMessage(EM_SETREADONLY, TRUE, 0);
		break;
	case 4:
		ColliderType_Combo.SetCurSel(4);
		break;
	case 3:
		ColliderType_Combo.SetCurSel(3);
		break;
	}
}
