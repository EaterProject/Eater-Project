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
	ON_BN_CLICKED(IDC_BUTTON1, &CTAP_Transform::OnAllScaleDown_Button)
	ON_BN_CLICKED(IDC_BUTTON15, &CTAP_Transform::OnAllScaleReset_Button)
	ON_BN_CLICKED(IDC_BUTTON14, &CTAP_Transform::OnAllScaleUp_Button)
	ON_BN_CLICKED(IDC_BUTTON16, &CTAP_Transform::OnResetRot_Button)
	ON_BN_CLICKED(IDC_BUTTON17, &CTAP_Transform::OnUpPosition_Y_Button)
	ON_BN_CLICKED(IDC_BUTTON18, &CTAP_Transform::OnDownPosition_Y_Button)
	ON_BN_CLICKED(IDC_BUTTON19, &CTAP_Transform::OnUpPosition_Z_Button)
	ON_BN_CLICKED(IDC_BUTTON20, &CTAP_Transform::OnDownPosition_Z_Button)
	ON_BN_CLICKED(IDC_BUTTON21, &CTAP_Transform::OnUpPosition_X_Button)
	ON_BN_CLICKED(IDC_BUTTON22, &CTAP_Transform::OnDownPosition_X_Button)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON2, &CTAP_Transform::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON13, &CTAP_Transform::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON25, &CTAP_Transform::OnBnClickedButton25)
	ON_BN_CLICKED(IDC_BUTTON23, &CTAP_Transform::OnBnClickedButton23)
END_MESSAGE_MAP()

BOOL CTAP_Transform::OnInitDialog()
{
	CDialog::OnInitDialog();
	Rotation_X_Slider.SetRange(-360, 360);
	Rotation_X_Slider.SetPos(2);

	Rotation_Y_Slider.SetRange(-360, 360);
	Rotation_Y_Slider.SetPos(2);

	Rotation_Z_Slider.SetRange(-360, 360);
	Rotation_Z_Slider.SetPos(2);
	return 0;
}

void CTAP_Transform::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, Pos_X);
	DDX_Control(pDX, IDC_EDIT3, Pos_Y);
	DDX_Control(pDX, IDC_EDIT4, Pos_Z);
	DDX_Control(pDX, IDC_EDIT5, Rot_X);
	DDX_Control(pDX, IDC_EDIT6, Rot_Y);
	DDX_Control(pDX, IDC_EDIT7, Rot_Z);
	DDX_Control(pDX, IDC_EDIT8, Scl_X);
	DDX_Control(pDX, IDC_EDIT9, Scl_Y);
	DDX_Control(pDX, IDC_EDIT10, Scl_Z);
	DDX_Control(pDX, IDC_SLIDER1, Rotation_X_Slider);
	DDX_Control(pDX, IDC_SLIDER2, Rotation_Y_Slider);
	DDX_Control(pDX, IDC_SLIDER3, Rotation_Z_Slider);
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

	//Rotation_X_Slider.SetPos((int)mTransform->Rotation.x);
	//Rotation_Y_Slider.SetPos((int)mTransform->Rotation.y);
	//Rotation_Z_Slider.SetPos((int)mTransform->Rotation.z);
	//Rotation_X_Slider.ShowWindow(SW_SHOW);
	//Rotation_Y_Slider.ShowWindow(SW_SHOW);
	//Rotation_Z_Slider.ShowWindow(SW_SHOW);
}

void CTAP_Transform::UpdateGameObject()
{
	CString Cx;
	CString Cy;
	CString Cz;
	Vector3 Pos;
	Vector3 Rot;
	Vector3 Scl;

	Pos_X.GetWindowTextW(Cx);
	Pos_Y.GetWindowTextW(Cy);
	Pos_Z.GetWindowTextW(Cz);
	Pos.x = ChangeToFloat(Cx);
	Pos.y = ChangeToFloat(Cy);
	Pos.z = ChangeToFloat(Cz);
	ObjectTransform->Position = { Pos.x,Pos.y,Pos.z };

	Rot_X.GetWindowTextW(Cx);
	Rot_Y.GetWindowTextW(Cy);
	Rot_Z.GetWindowTextW(Cz);
	Rot.x = ChangeToFloat(Cx);
	Rot.y = ChangeToFloat(Cy);
	Rot.z = ChangeToFloat(Cz);
	Rotation_X_Slider.SetPos((int)Rot.x);
	Rotation_Y_Slider.SetPos((int)Rot.y);
	Rotation_Z_Slider.SetPos((int)Rot.z);
	ObjectTransform->Rotation = { Rot.x,Rot.y,Rot.z };

	Scl_X.GetWindowTextW(Cx);
	Scl_Y.GetWindowTextW(Cy);
	Scl_Z.GetWindowTextW(Cz);
	Scl.x = ChangeToFloat(Cx);
	Scl.y = ChangeToFloat(Cy);
	Scl.z = ChangeToFloat(Cz);
	ObjectTransform->Scale = { Scl.x,Scl.y,Scl.z };
}

void CTAP_Transform::GetData(ObjectOption& Obj)
{
	CString Cx;
	CString Cy;
	CString Cz;
	Vector3 Pos;
	Vector3 Rot;
	Vector3 Scl;

	Pos_X.GetWindowTextW(Cx);
	Pos_Y.GetWindowTextW(Cy);
	Pos_Z.GetWindowTextW(Cz);
	Pos.x = ChangeToFloat(Cx);
	Pos.y = ChangeToFloat(Cy);
	Pos.z = ChangeToFloat(Cz);
	ObjectTransform->Position = { Pos.x,Pos.y,Pos.z };

	Rot_X.GetWindowTextW(Cx);
	Rot_Y.GetWindowTextW(Cy);
	Rot_Z.GetWindowTextW(Cz);
	Rot.x = ChangeToFloat(Cx);
	Rot.y = ChangeToFloat(Cy);
	Rot.z = ChangeToFloat(Cz);
	Rotation_X_Slider.SetPos((int)Rot.x);
	Rotation_Y_Slider.SetPos((int)Rot.y);
	Rotation_Z_Slider.SetPos((int)Rot.z);
	ObjectTransform->Rotation = { Rot.x,Rot.y,Rot.z };

	Scl_X.GetWindowTextW(Cx);
	Scl_Y.GetWindowTextW(Cy);
	Scl_Z.GetWindowTextW(Cz);
	Scl.x = ChangeToFloat(Cx);
	Scl.y = ChangeToFloat(Cy);
	Scl.z = ChangeToFloat(Cz);
	ObjectTransform->Scale = { Scl.x,Scl.y,Scl.z };

	Obj.Position	= Pos;
	Obj.Rotation	= Rot;
	Obj.Scale		= Scl;
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
			UpdateGameObject();
			return TRUE;

		case VK_RIGHT:
			ObjectTransform->SetTranlate(1, 0, 0);
			break;
		case VK_LEFT:
			ObjectTransform->SetTranlate(-1, 0, 0);
			break;
		default:
			break;
		}
	}
	else
	{
		ObjectTransform->SetTranlate(0, 0, 0);
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CTAP_Transform::OnAllScaleDown_Button()
{
	ObjectTransform->Scale = {0.01f,0.01f ,0.01f };
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

void CTAP_Transform::OnResetRot_Button()
{
	ObjectTransform->Rotation = { 0,0,0 };
	Rot_X.SetWindowTextW(ChangeToCString(ObjectTransform->Rotation.x));
	Rot_Y.SetWindowTextW(ChangeToCString(ObjectTransform->Rotation.y));
	Rot_Z.SetWindowTextW(ChangeToCString(ObjectTransform->Rotation.z));
}


void CTAP_Transform::OnUpPosition_Y_Button()
{
	ObjectTransform->Position.y += 1;
	Pos_Y.SetWindowTextW(ChangeToCString(ObjectTransform->Position.y));
}


void CTAP_Transform::OnDownPosition_Y_Button()
{
	ObjectTransform->Position.y -= 1;
	Pos_Y.SetWindowTextW(ChangeToCString(ObjectTransform->Position.y));
}


void CTAP_Transform::OnUpPosition_Z_Button()
{
	ObjectTransform->Position.z += 1;
	Pos_Z.SetWindowTextW(ChangeToCString(ObjectTransform->Position.z));
}


void CTAP_Transform::OnDownPosition_Z_Button()
{
	ObjectTransform->Position.z -= 1;
	Pos_Z.SetWindowTextW(ChangeToCString(ObjectTransform->Position.z));
}


void CTAP_Transform::OnUpPosition_X_Button()
{
	ObjectTransform->Position.x += 1;
	Pos_X.SetWindowTextW(ChangeToCString(ObjectTransform->Position.x));
}


void CTAP_Transform::OnDownPosition_X_Button()
{
	ObjectTransform->Position.x -= 1;
	Pos_X.SetWindowTextW(ChangeToCString(ObjectTransform->Position.x));
}




void CTAP_Transform::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString SilderText;
	if (pScrollBar->GetDlgCtrlID() == Rotation_X_Slider.GetDlgCtrlID()) 
	{
		ObjectTransform->Rotation.x = (float)Rotation_X_Slider.GetPos();
		Rot_X.SetWindowTextW(ChangeToCString(ObjectTransform->Rotation.x));
	}

	if (pScrollBar->GetDlgCtrlID() == Rotation_Y_Slider.GetDlgCtrlID())
	{
		ObjectTransform->Rotation.y = (float)Rotation_Y_Slider.GetPos();
		Rot_Y.SetWindowTextW(ChangeToCString(ObjectTransform->Rotation.y));
	}

	if (pScrollBar->GetDlgCtrlID() == Rotation_Z_Slider.GetDlgCtrlID())
	{
		ObjectTransform->Rotation.z = (float)Rotation_Z_Slider.GetPos();
		Rot_Z.SetWindowTextW(ChangeToCString(ObjectTransform->Rotation.z));
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CTAP_Transform::OnBnClickedButton2()
{
	ObjectTransform->Rotation.x = 0;
	Rotation_X_Slider.SetPos(0);
	Rot_X.SetWindowTextW(ChangeToCString(ObjectTransform->Rotation.x));
}


void CTAP_Transform::OnBnClickedButton13()
{
	ObjectTransform->Rotation.y = 0;
	Rotation_Y_Slider.SetPos(0);
	Rot_Y.SetWindowTextW(ChangeToCString(ObjectTransform->Rotation.y));
}


void CTAP_Transform::OnBnClickedButton25()
{
	ObjectTransform->Rotation.z = 0;
	Rotation_Z_Slider.SetPos(0);
	Rot_Z.SetWindowTextW(ChangeToCString(ObjectTransform->Rotation.z));
}


void CTAP_Transform::OnBnClickedButton23()
{
	ObjectTransform->Position = {0,0,0};
	Pos_X.SetWindowTextW(ChangeToCString(ObjectTransform->Position.x));
	Pos_Y.SetWindowTextW(ChangeToCString(ObjectTransform->Position.y));
	Pos_Z.SetWindowTextW(ChangeToCString(ObjectTransform->Position.z));
}
