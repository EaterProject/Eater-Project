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
	, SkyCube_SaveName_Edit(_T(""))
	, SkyLight_SaveName_Edit(_T(""))
	, ColorGrading_SaveName_Edit(_T(""))
	, SkyLightIndex(0)
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
	DDX_Text(pDX, IDC_EDIT3, SkyCube_SaveName_Edit);
	DDX_Text(pDX, IDC_EDIT4, SkyLight_SaveName_Edit);
	DDX_Text(pDX, IDC_EDIT5, ColorGrading_SaveName_Edit);
	DDX_Control(pDX, IDC_EDIT29, ColorGrading_Map_Edit);
	DDX_Control(pDX, IDC_CHECK16, ColorGrading_Volume_Check);
	DDX_Control(pDX, IDC_SPIN3, SkyLight_Spin);
	DDX_Text(pDX, IDC_EDIT30, SkyLightIndex);
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
	RECT Edit_ColorGrading_Rect;

	SkyLight_Map_Edit.GetWindowRect(&Edit_SkyLight_Rect);
	SkyCube_Map_Edit.GetWindowRect(&Edit_SkyCube_Rect);
	ColorGrading_Map_Edit.GetWindowRect(&Edit_ColorGrading_Rect);

	if (DropRect(Edit_SkyLight_Rect) == true)
	{
		SkyLightName = CutStringFileType(FileName);

		SkyLight_Map_Edit.SetWindowTextW(ChangeToCString(SkyLightName));

		BakeConvertSkyLightMap(SkyLightName, SkyLightAngle, SkyLightThreshold, SkyLightHDRI, SkyLightIndex);
	}
	else if (DropRect(Edit_SkyCube_Rect) == true)
	{
		SkyCubeName = CutStringFileType(FileName);

		SkyCube_Map_Edit.SetWindowTextW(ChangeToCString(SkyCubeName));

		BakeConvertSkyCubeMap(SkyCubeName, SkyCubeAngle, SkyCubeThreshold, SkyCubeHDRI);
	}
	else if (DropRect(Edit_ColorGrading_Rect) == true)
	{
		ColorGradingName = CutStringFileType(FileName);
		
		ColorGrading_Map_Edit.SetWindowTextW(ChangeToCString(ColorGradingName));
		
		if (ColorGradingVolume)
		{
			SetColorGradingBaseTexture(ColorGradingName);
		}
	}
}


BEGIN_MESSAGE_MAP(SkySetting, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK14, &SkySetting::OnSkyCubeHDRI)
	ON_BN_CLICKED(IDC_BUTTON21, &SkySetting::OnSkyCubeBakeButton)
	ON_BN_CLICKED(IDC_BUTTON28, &SkySetting::OnSkyLightBakeButton)
	ON_BN_CLICKED(IDC_CHECK15, &SkySetting::OnSkyLightHDRI)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON33, &SkySetting::OnColorGradingBakeButton)
	ON_BN_CLICKED(IDC_CHECK16, &SkySetting::OnColorGradingVolume)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &SkySetting::OnDeltaposSpin3)
END_MESSAGE_MAP()


// SkySetting 메시지 처리기


void SkySetting::OnSkyCubeHDRI()
{
	SkyCubeHDRI ^= true;
	BakeConvertSkyCubeMap(SkyCubeName, SkyCubeAngle, SkyCubeThreshold, SkyCubeHDRI);
}


void SkySetting::OnSkyCubeBakeButton()
{
	HWND hWndActive = ::GetForegroundWindow();

	if (SkyCubeName.empty())
	{
		MessageBoxA(hWndActive, "텍스쳐가 없는뎅?", "Error", MB_OK);

		return;
	}

	UpdateData(TRUE);

	std::string name = ChangeToString(SkyCube_SaveName_Edit);

	if (name.empty())
	{
		MessageBoxA(hWndActive, "이름이 없는뎅?", "Error", MB_OK);

		return;
	}

	/// 저장 이름 추가하면 두번째 매개변수에 삽입
	SaveConvertSkyCubeMap(SkyCubeName, name);

	if (MessageBoxA(hWndActive, "저장 완료", "SkyCubeBake", MB_OK) == IDOK)
	{
		this->ShowWindow(SW_HIDE);
	}
}

void SkySetting::OnSkyLightBakeButton()
{
	HWND hWndActive = ::GetForegroundWindow();
	if (SkyLightName.empty())
	{
		MessageBoxA(hWndActive, "텍스쳐가 없는뎅?", "Error", MB_OK);

		return;
	}

	UpdateData(TRUE);

	std::string name = ChangeToString(SkyLight_SaveName_Edit);

	if (name.empty())
	{
		MessageBoxA(hWndActive, "이름이 없는뎅?", "Error", MB_OK);

		return;
	}

	/// 저장 이름 추가하면 두번째 매개변수에 삽입
	SaveConvertSkyLightMap(SkyLightName, name);

	if (MessageBoxA(hWndActive, "저장 완료", "SkyLightBake", MB_OK) == IDOK)
	{
		this->ShowWindow(SW_HIDE);
	}
}

void SkySetting::OnColorGradingBakeButton()
{
	HWND hWndActive = ::GetForegroundWindow();

	if (ColorGradingName.empty())
	{
		MessageBoxA(hWndActive, "텍스쳐가 없는뎅?", "Error", MB_OK);

		return;
	}

	UpdateData(TRUE);

	std::string name = ChangeToString(ColorGrading_SaveName_Edit);

	if (name.empty())
	{
		MessageBoxA(hWndActive, "이름이 없는뎅?", "Error", MB_OK);

		return;
	}

	/// 저장 이름 추가하면 두번째 매개변수에 삽입
	SaveSpriteToVolumeTexture_LUT(ColorGradingName, name);

	if (MessageBoxA(hWndActive, "저장 완료", "SkyLightBake", MB_OK) == IDOK)
	{
		this->ShowWindow(SW_HIDE);
	}
}


void SkySetting::OnSkyLightHDRI()
{
	SkyLightHDRI ^= true;
	BakeConvertSkyLightMap(SkyLightName, SkyLightAngle, SkyLightThreshold, SkyLightHDRI, SkyLightIndex);
}

void SkySetting::OnColorGradingVolume()
{
	ColorGradingVolume ^= true;

	if (ColorGradingVolume)
	{
		SetColorGradingBaseTexture(ColorGradingName);
	}
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
		SkyCubeThreshold = (float)SkyCube_Threhold_Slider.GetPos() * 0.01f;
		SkyCube_Threshold_Edit.SetWindowTextW(ChangeToCString(SkyCubeThreshold));
		BakeConvertSkyCubeMap(SkyCubeName, SkyCubeAngle, SkyCubeThreshold, SkyCubeHDRI);
	}

	if (pScrollBar->GetDlgCtrlID() == SkyCube_Angle_Slider.GetDlgCtrlID())
	{
		float pos = (float)SkyCube_Angle_Slider.GetPos();
		SkyCubeAngle = pos / 10.0f;
		SkyCube_Angle_Edit.SetWindowTextW(ChangeToCString(SkyCubeAngle));
		BakeConvertSkyCubeMap(SkyCubeName, SkyCubeAngle, SkyCubeThreshold, SkyCubeHDRI);
	}

	if (pScrollBar->GetDlgCtrlID() == SkyLight_Factor_Slider.GetDlgCtrlID()) 
	{
		float pos = (float)SkyLight_Factor_Slider.GetPos() * 0.01f;
		mRenderOption->SkyLight_Factor = pos;
		SkyLight_Factor_Edit.SetWindowTextW(ChangeToCString(pos));
		RenderSetting();
	}

	if (pScrollBar->GetDlgCtrlID() == SkyLight_Threshold_Slider.GetDlgCtrlID())
	{
		SkyLightThreshold = (float)SkyLight_Threshold_Slider.GetPos() * 0.01f;
		SkyLight_Threshold_Edit.SetWindowTextW(ChangeToCString(SkyLightThreshold));
		BakeConvertSkyLightMap(SkyLightName, SkyLightAngle, SkyLightThreshold, SkyLightHDRI, SkyLightIndex);
	}

	if (pScrollBar->GetDlgCtrlID() == SkyLight_Angle_Slider.GetDlgCtrlID())
	{
		float pos = (float)SkyLight_Angle_Slider.GetPos();
		SkyLightAngle = pos / 10.0f;
		SkyLight_Angle_Edit.SetWindowTextW(ChangeToCString(SkyLightAngle));
		BakeConvertSkyLightMap(SkyLightName, SkyLightAngle, SkyLightThreshold, SkyLightHDRI, SkyLightIndex);
	}
}




BOOL SkySetting::OnInitDialog()
{
	CustomDialog::OnInitDialog();

	SkyCube_Size_Slider.SetRange(1,5000);
	SkyCube_Threhold_Slider.SetRange(100,10000);
	SkyCube_Angle_Slider.SetRange(0,3600);

	SkyCube_Size_Slider.SetPos(500);
	SkyCube_Threhold_Slider.SetPos(10000);
	SkyCube_Angle_Slider.SetPos(0);

	SkyCube_Threshold_Edit.SetWindowTextW(L"100");
	SkyCube_Angle_Edit.SetWindowTextW(L"0");
	SkyCube_Size_Edit.SetWindowTextW(L"500");

	SkyCubeAngle = 0.0f;
	SkyCubeThreshold = 100.0f;
	SkyCubeHDRI = false;

	SkyLight_Factor_Slider.SetRange(0,500);
	SkyLight_Threshold_Slider.SetRange(100,10000);
	SkyLight_Angle_Slider.SetRange(0, 3600);

	SkyLight_Factor_Slider.SetPos(100);
	SkyLight_Threshold_Slider.SetPos(10000);
	SkyLight_Angle_Slider.SetPos(0);


	SkyLight_Threshold_Edit.SetWindowTextW(L"100");
	SkyLight_Angle_Edit.SetWindowTextW(L"0");
	SkyLight_Factor_Edit.SetWindowTextW(L"1");

	SkyLight_Spin.SetRange(-100, 100);
	SkyLight_Spin.SetPos(0);

	SkyLightAngle = 0.0f;
	SkyLightThreshold = 100.0f;
	SkyLightHDRI = false;

	ColorGradingVolume = false;

	return TRUE; 
}

void SkySetting::OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (pNMUpDown->iDelta > 0)
	{
		SkyLightIndex++;
	}
	else
	{
		SkyLightIndex--;

		// 인덱스가 음수일경우 보정..
		if (SkyLightIndex < 0)
		{
			SkyLight_Spin.SetPos(0);
			SkyLightIndex = 0;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}
