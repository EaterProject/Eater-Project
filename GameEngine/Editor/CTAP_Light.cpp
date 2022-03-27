// CTAP_Light.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "CTAP_Light.h"
#include "afxdialogex.h"
#include "Light.h"
#include "GrobalFunction.h"


// CTAP_Light 대화 상자

IMPLEMENT_DYNAMIC(CTAP_Light, CDialogEx)

CTAP_Light::CTAP_Light(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAP_LIGHT, pParent)
{

}

CTAP_Light::~CTAP_Light()
{
}

void CTAP_Light::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, Light_Type);
	DDX_Control(pDX, IDC_EDIT1, Color_R);
	DDX_Control(pDX, IDC_EDIT3, Color_G);
	DDX_Control(pDX, IDC_EDIT4, Color_B);
	DDX_Control(pDX, IDC_SLIDER4, Color_R_Slider);
	DDX_Control(pDX, IDC_SLIDER5, Color_G_Slider);
	DDX_Control(pDX, IDC_SLIDER6, Color_B_Slider);
	DDX_Control(pDX, IDC_SLIDER2, Angle_Slider);
	DDX_Control(pDX, IDC_EDIT2, Angle_Edit);
	DDX_Control(pDX, IDC_SLIDER1, Range_Slider);
	DDX_Control(pDX, IDC_EDIT59, Range_Edit);
	DDX_Control(pDX, IDC_SLIDER3, Power_Slider);
	DDX_Control(pDX, IDC_EDIT60, Power_Edit);
	DDX_Control(pDX, IDC_SLIDER7, Attenuate_Slider);
	DDX_Control(pDX, IDC_EDIT61, Attenuate_Edit);
}

BOOL CTAP_Light::OnInitDialog()
{
	CDialog::OnInitDialog();

	Light_Type.InsertString(0, L"NONE");
	Light_Type.InsertString(1, L"DirectionLight");
	Light_Type.InsertString(2, L"PointLight");
	Light_Type.InsertString(3, L"SpotLight");
	Light_Type.SetCurSel(0);

	Color_R_Slider.SetRange(0, 255);
	Color_G_Slider.SetRange(0, 255);
	Color_B_Slider.SetRange(0, 255);

	Angle_Slider.SetRange(0, 90);
	Range_Slider.SetRange(0, 100);
	Power_Slider.SetRange(0, 1000);
	Attenuate_Slider.SetRange(0, 99);


	Color_R_Slider.SetPos(0);
	Color_G_Slider.SetPos(0);
	Color_B_Slider.SetPos(0);
	Attenuate_Slider.SetPos(0);

	Angle_Slider.SetPos(0);
	Range_Slider.SetPos(0);
	Power_Slider.SetPos(0);

	Color_R.SetWindowTextW(L"0");
	Color_G.SetWindowTextW(L"0");
	Color_B.SetWindowTextW(L"0");
	Angle_Edit.SetWindowTextW(L"0");
	Range_Edit.SetWindowTextW(L"0");
	Power_Edit.SetWindowTextW(L"0");

	return 0;
}


BEGIN_MESSAGE_MAP(CTAP_Light, CDialogEx)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CTAP_Light::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CTAP_Light 메시지 처리기


void CTAP_Light::SetGameObject(Light* TempLight)
{
	mLight = TempLight;
	int Type = mLight->GetType();
	Light_Type.SetCurSel(Type);

	float Angle		= mLight->GetAngle();
	float Range		= mLight->GetRange();
	float Power		= mLight->GetPower();
	Vector3 mColor	= mLight->GetColor();
	float mAttenuate = mLight->GetAttenuate();

	Color_R_Slider.SetPos(mColor.x*255);
	Color_G_Slider.SetPos(mColor.y*255);
	Color_B_Slider.SetPos(mColor.z*255);
	Attenuate_Slider.SetPos(mAttenuate*0.01);

	float OriginalAngle = Angle * 180 / 3.141592f;
	Angle_Slider.SetPos(OriginalAngle);
	Range_Slider.SetPos(Range);
	Power_Slider.SetPos(Power);

	Color_R.SetWindowTextW(ChangeToCString(mColor.x * 255));
	Color_G.SetWindowTextW(ChangeToCString(mColor.y * 255));
	Color_B.SetWindowTextW(ChangeToCString(mColor.z * 255));

	Angle_Edit.SetWindowTextW(ChangeToCString(OriginalAngle));
	Range_Edit.SetWindowTextW(ChangeToCString(Range));
	Power_Edit.SetWindowTextW(ChangeToCString(Power));
	Attenuate_Edit.SetWindowTextW(ChangeToCString(mAttenuate));
}

BOOL CTAP_Light::PreTranslateMessage(MSG* pMsg)
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


void CTAP_Light::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->GetDlgCtrlID() == Color_R_Slider.GetDlgCtrlID())
	{
		float R =(float)Color_R_Slider.GetPos();
		float G =(float)Color_G_Slider.GetPos();
		float B =(float)Color_B_Slider.GetPos();

		mLight->SetColor(R/255, G / 255, B / 255);
	
		Color_R.SetWindowTextW(ChangeToCString(R));
		Color_G.SetWindowTextW(ChangeToCString(G));
		Color_B.SetWindowTextW(ChangeToCString(B));
	}
	if (pScrollBar->GetDlgCtrlID() == Color_G_Slider.GetDlgCtrlID())
	{
		float R = (float)Color_R_Slider.GetPos();
		float G = (float)Color_G_Slider.GetPos();
		float B = (float)Color_B_Slider.GetPos();

		mLight->SetColor(R / 255, G / 255, B / 255);

		Color_R.SetWindowTextW(ChangeToCString(R));
		Color_G.SetWindowTextW(ChangeToCString(G));
		Color_B.SetWindowTextW(ChangeToCString(B));
	}
	if (pScrollBar->GetDlgCtrlID() == Color_B_Slider.GetDlgCtrlID())
	{
		float R = (float)Color_R_Slider.GetPos();
		float G = (float)Color_G_Slider.GetPos();
		float B = (float)Color_B_Slider.GetPos();

		mLight->SetColor(R / 255, G / 255, B / 255);

		Color_R.SetWindowTextW(ChangeToCString(R));
		Color_G.SetWindowTextW(ChangeToCString(G));
		Color_B.SetWindowTextW(ChangeToCString(B));
	}
	if (pScrollBar->GetDlgCtrlID() == Angle_Slider.GetDlgCtrlID())
	{
		int Pos = Angle_Slider.GetPos();
		mLight->SetAngle(Pos);
		Angle_Edit.SetWindowTextW(ChangeToCString(Pos));
	}
	if (pScrollBar->GetDlgCtrlID() == Range_Slider.GetDlgCtrlID())
	{
		int Range = Range_Slider.GetPos();
		mLight->SetRange(Range);
		Range_Edit.SetWindowTextW(ChangeToCString(Range));
	}
	if (pScrollBar->GetDlgCtrlID() == Power_Slider.GetDlgCtrlID())
	{
		int Power = Power_Slider.GetPos();
		mLight->SetPower(Power);
		Power_Edit.SetWindowTextW(ChangeToCString(Power));
	}
	if (pScrollBar->GetDlgCtrlID() == Attenuate_Slider.GetDlgCtrlID())
	{
		float Attenuate = (float)Attenuate_Slider.GetPos();
		mLight->SetAttenuate(Attenuate* 0.01f);
		Attenuate_Edit.SetWindowTextW(ChangeToCString(Attenuate * 0.01f));
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CTAP_Light::OnCbnSelchangeCombo1()
{
	int Choice = Light_Type.GetCurSel();
	switch (Choice)
	{
	case 0:
		mLight->SetType(NONE_LIGHT);
		break;
	case 1:
		mLight->SetType(DIRECTION_LIGHT);
		break;
	case 2:
		mLight->SetType(POINT_LIGHT);
		break;
	case 3:
		mLight->SetType(SPOT_LIGHT);
		break;
	}
}
