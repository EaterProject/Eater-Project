// SkySetting.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "SkySetting.h"
#include "afxdialogex.h"
#include "EaterEngineAPI.h"
#include "EngineData.h"


// SkySetting 대화 상자

IMPLEMENT_DYNAMIC(SkySetting, CustomDialog)

SkySetting::SkySetting(CWnd* pParent /*=nullptr*/)
	: CustomDialog(IDD_SKY_SETTING, pParent)
{

}

SkySetting::~SkySetting()
{
}

void SkySetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER15, SkyCube_Size_Slider);
	DDX_Control(pDX, IDC_SLIDER23, SkyCube_Threhold_Slider);
	DDX_Control(pDX, IDC_SLIDER16, SkyCube_Angle_Slider);
	DDX_Control(pDX, IDC_EDIT17, SkyCube_Size_Edit);
	DDX_Control(pDX, IDC_EDIT31, SkyCube_Threshold_Edit);
	DDX_Control(pDX, IDC_EDIT18, SkyCube_Angle_Edit);
	DDX_Control(pDX, IDC_CHECK14, SkyCube_HDRI_Check);


	DDX_Control(pDX, IDC_SLIDER18, SkyLight_Factor_Slider);
	DDX_Control(pDX, IDC_EDIT22, SkyLight_Factor_Edit);
	DDX_Control(pDX, IDC_SLIDER24, SkyLight_Threshold_Slider);
	DDX_Control(pDX, IDC_EDIT32, SkyLight_Threshold_Edit);
	DDX_Control(pDX, IDC_SLIDER19, SkyLight_Angle_Slider);
	DDX_Control(pDX, IDC_EDIT23, SkyLight_Angle_Edit);
	DDX_Control(pDX, IDC_CHECK15, SkyLight_HDRI_Check);
	DDX_Control(pDX, IDC_EDIT26, SkyCube_Map_Edit);
	DDX_Control(pDX, IDC_EDIT28, SkyLight_Map_Edit);
}

void SkySetting::SetCheck(CString FileName)
{
	int FileType = GetFileNameType(ChangeToString(FileName));


	if (FileType != DDS && FileType != PNG && FileType != HDR)
	{
		MessageBox(L"DDS/PNG/HDR 파일만 들어올수 있습니다");
		return;
	}

	RECT Edit_SkyLight_Rect;
	RECT Edit_SkyCube_Rect;

	SkyLight_Map_Edit.GetWindowRect(&Edit_SkyLight_Rect);
	SkyCube_Map_Edit.GetWindowRect(&Edit_SkyCube_Rect);

	if (DropRect(Edit_SkyLight_Rect) == true)
	{
		SkyLightName = CutStringFileType(FileName);

		SkyLight_Map_Edit.SetWindowTextW(ChangeToCString(SkyLightName));

		BakeConvertSkyLightMap(SkyLightName, SkyLightAngle, SkyLightThreshold, SkyLightHDRI);
	}
	else if (DropRect(Edit_SkyCube_Rect) == true)
	{
		SkyCubeName = CutStringFileType(FileName);

		SkyCube_Map_Edit.SetWindowTextW(ChangeToCString(SkyCubeName));

		BakeConvertSkyCubeMap(SkyCubeName, SkyCubeAngle, SkyCubeThreshold, SkyCubeHDRI);
	}
}


BEGIN_MESSAGE_MAP(SkySetting, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK14, &SkySetting::OnSkyCubeHDRI)
	ON_BN_CLICKED(IDC_BUTTON21, &SkySetting::OnSkyCubeBakeButton)
	ON_BN_CLICKED(IDC_BUTTON28, &SkySetting::OnSkyLightBakeButton)
	ON_BN_CLICKED(IDC_CHECK15, &SkySetting::OnSkyLightHDRI)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// SkySetting 메시지 처리기


void SkySetting::OnSkyCubeHDRI()
{
	SkyCubeHDRI ^= true;
	BakeConvertSkyCubeMap(SkyCubeName, SkyCubeAngle, SkyCubeThreshold, SkyCubeHDRI);
}


void SkySetting::OnSkyCubeBakeButton()
{
	SaveConvertSkyCubeMap(SkyCubeName);
}

void SkySetting::OnSkyLightBakeButton()
{
	SaveConvertSkyLightMap(SkyLightName);
}


void SkySetting::OnSkyLightHDRI()
{
	SkyLightHDRI ^= true;
	BakeConvertSkyLightMap(SkyLightName, SkyLightAngle, SkyLightThreshold, SkyLightHDRI);
}

void SkySetting::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	mRenderOption = GetRenderOptionData();
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (pScrollBar->GetDlgCtrlID() == SkyCube_Size_Slider.GetDlgCtrlID())
	{
		int pos = SkyCube_Size_Slider.GetPos();
		mRenderOption->SkyCube_Size = pos;
		SkyCube_Size_Edit.SetWindowTextW(ChangeToCString(pos));
		RenderSetting();
	}

	if (pScrollBar->GetDlgCtrlID() == SkyCube_Threhold_Slider.GetDlgCtrlID())
	{
		SkyCubeThreshold = (float)SkyCube_Threhold_Slider.GetPos();
		SkyCube_Threshold_Edit.SetWindowTextW(ChangeToCString(SkyCubeThreshold));
		BakeConvertSkyCubeMap(SkyCubeName, SkyCubeAngle, SkyCubeThreshold, SkyCubeHDRI);
	}

	if (pScrollBar->GetDlgCtrlID() == SkyCube_Angle_Slider.GetDlgCtrlID())
	{
		float pos = (float)SkyCube_Angle_Slider.GetPos();
		SkyCubeAngle = pos/10.0f;
		SkyCube_Angle_Edit.SetWindowTextW(ChangeToCString(SkyCubeAngle));
		BakeConvertSkyCubeMap(SkyCubeName, SkyCubeAngle, SkyCubeThreshold, SkyCubeHDRI);
	}

	if (pScrollBar->GetDlgCtrlID() == SkyLight_Factor_Slider.GetDlgCtrlID()) 
	{
		float pos = (float)SkyLight_Factor_Slider.GetPos() / 100.0f;
		mRenderOption->SkyLight_Factor = pos;
		SkyLight_Factor_Edit.SetWindowTextW(ChangeToCString(pos));
		RenderSetting();
	}

	if (pScrollBar->GetDlgCtrlID() == SkyLight_Threshold_Slider.GetDlgCtrlID())
	{
		SkyLightThreshold = (float)SkyLight_Threshold_Slider.GetPos();
		SkyLight_Threshold_Edit.SetWindowTextW(ChangeToCString(SkyLightThreshold));
		BakeConvertSkyLightMap(SkyLightName, SkyLightAngle, SkyLightThreshold, SkyLightHDRI);
	}

	if (pScrollBar->GetDlgCtrlID() == SkyLight_Angle_Slider.GetDlgCtrlID())
	{
		float pos = (float)SkyLight_Angle_Slider.GetPos();
		SkyLightAngle = pos / 10.0f;
		SkyLight_Angle_Edit.SetWindowTextW(ChangeToCString(SkyLightAngle));
		BakeConvertSkyLightMap(SkyLightName, SkyLightAngle, SkyLightThreshold, SkyLightHDRI);
	}


}




BOOL SkySetting::OnInitDialog()
{
	CustomDialog::OnInitDialog();

	SkyCube_Size_Slider.SetRange(1,5000);
	SkyCube_Threhold_Slider.SetRange(1,1000);
	SkyCube_Angle_Slider.SetRange(0,3600);

	SkyCube_Size_Slider.SetPos(500);
	SkyCube_Threhold_Slider.SetPos(100);
	SkyCube_Angle_Slider.SetPos(0);

	SkyCube_Threshold_Edit.SetWindowTextW(L"100");
	SkyCube_Angle_Edit.SetWindowTextW(L"0");
	SkyCube_Size_Edit.SetWindowTextW(L"500");

	SkyCubeAngle = 0.0f;
	SkyCubeThreshold = 100.0f;
	SkyCubeHDRI = false;

	SkyLight_Factor_Slider.SetRange(0,500);
	SkyLight_Threshold_Slider.SetRange(1,1000);
	SkyLight_Angle_Slider.SetRange(0, 3600);

	SkyLight_Factor_Slider.SetPos(100);
	SkyLight_Threshold_Slider.SetPos(100);
	SkyLight_Angle_Slider.SetPos(0);


	SkyLight_Factor_Edit.SetWindowTextW(L"1");
	SkyLight_Threshold_Edit.SetWindowTextW(L"100");
	SkyLight_Angle_Edit.SetWindowTextW(L"0");

	SkyLightAngle = 0.0f;
	SkyLightThreshold = 100.0f;
	SkyLightHDRI = false;

	return TRUE; 
}


