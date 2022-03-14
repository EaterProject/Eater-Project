// CTAP_1.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "CTAP_Transform.h"
#include "afxdialogex.h"


#include "GameObject.h"
#include "Transform.h"
// CTAP_1 대화 상자

IMPLEMENT_DYNAMIC(CTAP_Transform, CDialogEx)

CTAP_Transform::CTAP_Transform(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAP_TRANSFORM, pParent)
{

}

CTAP_Transform::~CTAP_Transform()
{


}
BEGIN_MESSAGE_MAP(CTAP_Transform, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CTAP_Transform::OnLeftRot_Button)
	ON_BN_CLICKED(IDC_BUTTON13, &CTAP_Transform::OnRightRot_Button)
	ON_BN_CLICKED(IDC_BUTTON1, &CTAP_Transform::OnAllScaleDown_Button)
	ON_BN_CLICKED(IDC_BUTTON15, &CTAP_Transform::OnAllScaleReset_Button)
	ON_BN_CLICKED(IDC_BUTTON14, &CTAP_Transform::OnAllScaleUp_Button)
	ON_BN_CLICKED(IDC_BUTTON6, &CTAP_Transform::OnUpRot_Button)
	ON_BN_CLICKED(IDC_BUTTON8, &CTAP_Transform::OnDownRot_Button)
	ON_BN_CLICKED(IDC_BUTTON16, &CTAP_Transform::OnResetRot_Button)
END_MESSAGE_MAP()

void CTAP_Transform::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_EDIT1, Pos_X);
	DDX_Control(pDX, IDC_EDIT3, Pos_Y);
	DDX_Control(pDX, IDC_EDIT4, Pos_Z);
	DDX_Control(pDX, IDC_EDIT5, Rot_X);
	DDX_Control(pDX, IDC_EDIT6, Rot_Y);
	DDX_Control(pDX, IDC_EDIT7, Rot_Z);
	DDX_Control(pDX, IDC_EDIT8, Scl_X);
	DDX_Control(pDX, IDC_EDIT9, Scl_Y);
	DDX_Control(pDX, IDC_EDIT10, Scl_Z);
	CDialogEx::DoDataExchange(pDX);
}

void CTAP_Transform::UpdateData(BOOL bSaveAndValidate)
{
	int num = 0;
}

void CTAP_Transform::SetGameObject(Transform* mTransform)
{
	ObjectTransform = mTransform;
	Pos_X.SetWindowTextW(ChangeToCString(mTransform->Position.x));
	Pos_Y.SetWindowTextW(ChangeToCString(mTransform->Position.y));
	Pos_Z.SetWindowTextW(ChangeToCString(mTransform->Position.z));
	Rot_X.SetWindowTextW(ChangeToCString(mTransform->Rotation.x));
	Rot_Y.SetWindowTextW(ChangeToCString(mTransform->Rotation.y));
	Rot_Z.SetWindowTextW(ChangeToCString(mTransform->Rotation.z));
	Scl_X.SetWindowTextW(ChangeToCString(mTransform->Scale.x));
	Scl_Y.SetWindowTextW(ChangeToCString(mTransform->Scale.y));
	Scl_Z.SetWindowTextW(ChangeToCString(mTransform->Scale.z));
}

TransformData CTAP_Transform::GetObjectData()
{
	TransformData Data;
	CString Cx, Cy, Cz;
	float X, Y, Z;
	Pos_X.GetWindowTextW(Cx);
	Pos_Y.GetWindowTextW(Cy);
	Pos_Z.GetWindowTextW(Cz);
	X = ChangeToFloat(Cx);
	Y = ChangeToFloat(Cy);
	Z = ChangeToFloat(Cz);
	ObjectTransform->Position = {X,Y,Z};

	Rot_X.GetWindowTextW(Cx);
	Rot_Y.GetWindowTextW(Cy);
	Rot_Z.GetWindowTextW(Cz);
	X = ChangeToFloat(Cx);
	Y = ChangeToFloat(Cy);
	Z = ChangeToFloat(Cz);
	ObjectTransform->Rotation = { X,Y,Z };

	Scl_X.GetWindowTextW(Cx);
	Scl_Y.GetWindowTextW(Cy);
	Scl_Z.GetWindowTextW(Cz);
	X = ChangeToFloat(Cx);
	Y = ChangeToFloat(Cy);
	Z = ChangeToFloat(Cz);
	ObjectTransform->Scale = { X,Y,Z }; 

	return Data;
}

void CTAP_Transform::Reset()
{
	Pos_X.SetWindowTextW(_T("0.00"));
	Pos_Y.SetWindowTextW(_T("0.00"));
	Pos_Z.SetWindowTextW(_T("0.00"));

	Rot_X.SetWindowTextW(_T("0.00"));
	Rot_Y.SetWindowTextW(_T("0.00"));
	Rot_Z.SetWindowTextW(_T("0.00"));

	Scl_X.SetWindowTextW(_T("1.00"));
	Scl_Y.SetWindowTextW(_T("1.00"));
	Scl_Z.SetWindowTextW(_T("1.00"));

	ObjectTransform->Position	= { 0,0,0 };
	ObjectTransform->Rotation	= { 0,0,0 };
	ObjectTransform->Scale		= { 1,1,1 };
}


BOOL CTAP_Transform::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:
			GetObjectData();
			return TRUE;
		default:
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CTAP_Transform::OnLeftRot_Button()
{
	ObjectTransform->Rotation.y -= 5;
	Rot_X.SetWindowTextW(ChangeToCString(ObjectTransform->Rotation.x));
	Rot_Y.SetWindowTextW(ChangeToCString(ObjectTransform->Rotation.y));
	Rot_Z.SetWindowTextW(ChangeToCString(ObjectTransform->Rotation.z));
}


void CTAP_Transform::OnRightRot_Button()
{
	ObjectTransform->Rotation.y += 5;
	Rot_X.SetWindowTextW(ChangeToCString(ObjectTransform->Rotation.x));
	Rot_Y.SetWindowTextW(ChangeToCString(ObjectTransform->Rotation.y));
	Rot_Z.SetWindowTextW(ChangeToCString(ObjectTransform->Rotation.z));
}


void CTAP_Transform::OnAllScaleDown_Button()
{
	ObjectTransform->Scale = {0.01,0.01 ,0.01 };
	Scl_X.SetWindowTextW(ChangeToCString(ObjectTransform->Scale.x));
	Scl_Y.SetWindowTextW(ChangeToCString(ObjectTransform->Scale.y));
	Scl_Z.SetWindowTextW(ChangeToCString(ObjectTransform->Scale.z));
}


void CTAP_Transform::OnAllScaleReset_Button()
{
	ObjectTransform->Scale = { 1.00,1.00 ,1.00 };
	Scl_X.SetWindowTextW(ChangeToCString(ObjectTransform->Scale.x));
	Scl_Y.SetWindowTextW(ChangeToCString(ObjectTransform->Scale.y));
	Scl_Z.SetWindowTextW(ChangeToCString(ObjectTransform->Scale.z));
}


void CTAP_Transform::OnAllScaleUp_Button()
{
	ObjectTransform->Scale = { 100,100 ,100 };
	Scl_X.SetWindowTextW(ChangeToCString(ObjectTransform->Scale.x));
	Scl_Y.SetWindowTextW(ChangeToCString(ObjectTransform->Scale.y));
	Scl_Z.SetWindowTextW(ChangeToCString(ObjectTransform->Scale.z));
}


void CTAP_Transform::OnUpRot_Button()
{
	ObjectTransform->Rotation.x += 5;
	Rot_X.SetWindowTextW(ChangeToCString(ObjectTransform->Rotation.x));
	Rot_Y.SetWindowTextW(ChangeToCString(ObjectTransform->Rotation.y));
	Rot_Z.SetWindowTextW(ChangeToCString(ObjectTransform->Rotation.z));
}


void CTAP_Transform::OnDownRot_Button()
{
	ObjectTransform->Rotation.x -= 5;
	Rot_X.SetWindowTextW(ChangeToCString(ObjectTransform->Rotation.x));
	Rot_Y.SetWindowTextW(ChangeToCString(ObjectTransform->Rotation.y));
	Rot_Z.SetWindowTextW(ChangeToCString(ObjectTransform->Rotation.z));
}


void CTAP_Transform::OnResetRot_Button()
{
	ObjectTransform->Rotation = { 0,0,0 };
	Rot_X.SetWindowTextW(ChangeToCString(ObjectTransform->Rotation.x));
	Rot_Y.SetWindowTextW(ChangeToCString(ObjectTransform->Rotation.y));
	Rot_Z.SetWindowTextW(ChangeToCString(ObjectTransform->Rotation.z));
}
