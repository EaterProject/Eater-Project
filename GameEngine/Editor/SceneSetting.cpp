// SceneSetting.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "SceneSetting.h"
#include "afxdialogex.h"
#include "EaterEngineAPI.h"
#include "EngineData.h"


// SceneSetting 대화 상자

IMPLEMENT_DYNAMIC(SceneSetting, CustomDialog)

SceneSetting::SceneSetting(CWnd* pParent /*=nullptr*/)
	: CustomDialog(IDD_SCENE_SETTING, pParent)
{

}

SceneSetting::~SceneSetting()
{

}

BOOL SceneSetting::OnInitDialog()
{
	CustomDialog::OnInitDialog();
	//tool tip 셋팅
	mTooltip.Create(this);
	mTooltip.AddTool(GetDlgItem(IDC_SLIDER4),_T("샘플링시 비교할 Texel 반지름"));
	mTooltip.AddTool(GetDlgItem(IDC_SLIDER7),_T("차폐되는 최소 깊이값"));
	mTooltip.AddTool(GetDlgItem(IDC_SLIDER10), _T("FOG 움직이는 속도"));
	mTooltip.AddTool(GetDlgItem(IDC_SLIDER11), _T("카메라 기준 FOG 시작거리"));
	mTooltip.AddTool(GetDlgItem(IDC_SLIDER19), _T("FOG 거리 오프셋"));
	mTooltip.AddTool(GetDlgItem(IDC_SLIDER12), _T("FOG 거리 범위"));
	mTooltip.AddTool(GetDlgItem(IDC_SLIDER13), _T("FOG 높이 오프셋"));
	mTooltip.AddTool(GetDlgItem(IDC_SLIDER14), _T("FOG 높이 범위"));
	mTooltip.AddTool(GetDlgItem(IDC_SLIDER16), _T("Bloom 추출 시작 영역"));
	mTooltip.AddTool(GetDlgItem(IDC_SLIDER17), _T("Bloom 혼합률"));

	//체크 박스 초기화
	Debug_Check.SetCheck(true);
	SSAO_Check.SetCheck(true);
	FOG_Check.SetCheck(true);
	Shadow_Check.SetCheck(true);
	IBL_Check.SetCheck(true);
	BLOOM_Check.SetCheck(true);
	FXAA_Check.SetCheck(true);
	HDR_Check.SetCheck(true);

	//데이터 초기화
	SSAO_DataSetting();
	FOG_DataSetting();
	Bloom_DataSetting();
	//IBL_DataSetting();
	//Environment_DataSetting();


	return true;
}

void SceneSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//체크박스
	DDX_Control(pDX, IDC_CHECK1, Debug_Check);
	DDX_Control(pDX, IDC_CHECK3, SSAO_Check);
	DDX_Control(pDX, IDC_CHECK4, FOG_Check);
	DDX_Control(pDX, IDC_CHECK6, HDR_Check);
	DDX_Control(pDX, IDC_CHECK2, Shadow_Check);
	DDX_Control(pDX, IDC_CHECK9, IBL_Check);
	DDX_Control(pDX, IDC_CHECK5, BLOOM_Check);
	DDX_Control(pDX, IDC_CHECK10, FXAA_Check);
	//SSAO
	DDX_Control(pDX, IDC_SLIDER4, SSAO_Radius_Slider);
	DDX_Control(pDX, IDC_SLIDER7, SSAO_SurfaceEpsiln_Slider);
	DDX_Control(pDX, IDC_EDIT1, SSAO_Radius_Edit);
	DDX_Control(pDX, IDC_EDIT5, SSAO_SurfaceEpsiln_Edit);
	DDX_Control(pDX, IDC_SLIDER8, SSAO_BlurCount_Slider);
	DDX_Control(pDX, IDC_EDIT6, SSAO_Blur_Edit);
	//FOG
	DDX_Control(pDX, IDC_SLIDER9, FOG_ColorR_Slider);
	DDX_Control(pDX, IDC_SLIDER6, FOG_ColorG_Slider);
	DDX_Control(pDX, IDC_SLIDER5, FOG_ColorB_Slider);
	DDX_Control(pDX, IDC_EDIT3, FOG_ColorR_Edit);
	DDX_Control(pDX, IDC_EDIT4, FOG_ColorG_Edit);
	DDX_Control(pDX, IDC_EDIT7, FOG_ColorB_Edit);
	DDX_Control(pDX, IDC_SLIDER10, FOG_MoveSpeed_Slider);
	DDX_Control(pDX, IDC_SLIDER11, FOG_StartDistance_Slider);
	DDX_Control(pDX, IDC_SLIDER12, FOG_DistanceValue_Slider);
	DDX_Control(pDX, IDC_SLIDER13, FOG_HeightOffset_Slider);
	DDX_Control(pDX, IDC_SLIDER14, FOG_HeightValue_Slider);
	DDX_Control(pDX, IDC_EDIT16, FOG_HeightOffset_Edit);
	DDX_Control(pDX, IDC_EDIT15, FOG_HeightValue_Edit);
	DDX_Control(pDX, IDC_EDIT10, FOG_DistanceValue_Edit);
	DDX_Control(pDX, IDC_EDIT9, FOG_StartDistance_Edit);
	DDX_Control(pDX, IDC_EDIT8, FOG_MoveSpeed_Edit);
	DDX_Control(pDX, IDC_SLIDER19, Fog_DistanceOffset_Slider);
	DDX_Control(pDX, IDC_EDIT23, Fog_DistanceOffset_Edit);

	DDX_Control(pDX, IDC_SLIDER16, Bloom_Threshold_Min_Slider);
	DDX_Control(pDX, IDC_EDIT18, Bloom_Threshold_Min_Edit);
	DDX_Control(pDX, IDC_SLIDER17, Bloom_Factor_Slider);
	DDX_Control(pDX, IDC_EDIT21, Bloom_Factor_Edit);

	DDX_Control(pDX, IDC_CHECK12, RenderTarget_Check);
	DDX_Control(pDX, IDC_SLIDER20, Bloom_Threshold_Max_Slider);
	DDX_Control(pDX, IDC_EDIT24, Bloom_Threshold_Max_Edit);
}

void SceneSetting::SSAO_DataSetting()
{
	//slider 초기화
	SSAO_Radius_Slider.SetRange(0, 500);
	SSAO_SurfaceEpsiln_Slider.SetRange(0, 1000);
	SSAO_Radius_Slider.SetPos(0);
	SSAO_SurfaceEpsiln_Slider.SetPos(50);
	SSAO_BlurCount_Slider.SetRange(1, 8);
	SSAO_BlurCount_Slider.SetPos(4);

	//Editor 초기화
	SSAO_Radius_Edit.SetWindowTextW(L"0.25");
	SSAO_Blur_Edit.SetWindowTextW(L"4");
	SSAO_SurfaceEpsiln_Edit.SetWindowTextW(L"0.05");
}

void SceneSetting::FOG_DataSetting()
{
	FOG_ColorR_Slider.SetRange(0, 255);
	FOG_ColorG_Slider.SetRange(0, 255);
	FOG_ColorB_Slider.SetRange(0, 255);
	FOG_MoveSpeed_Slider.SetRange(0, 1000);
	FOG_StartDistance_Slider.SetRange(0, 1000);
	Fog_DistanceOffset_Slider.SetRange(0, 100);
	FOG_DistanceValue_Slider.SetRange(0, 100);
	FOG_HeightOffset_Slider.SetRange(0, 100);
	FOG_HeightValue_Slider.SetRange(0, 100);

	FOG_MoveSpeed_Slider.SetPos(0);
	FOG_StartDistance_Slider.SetPos(0);
	Fog_DistanceOffset_Slider.SetPos(0);
	FOG_DistanceValue_Slider.SetPos(0);
	FOG_HeightOffset_Slider.SetPos(0);
	FOG_HeightValue_Slider.SetPos(0);
	FOG_ColorR_Slider.SetPos(0);
	FOG_ColorG_Slider.SetPos(0);
	FOG_ColorB_Slider.SetPos(0);

	FOG_MoveSpeed_Edit.SetWindowTextW(L"0.0");
	FOG_StartDistance_Edit.SetWindowTextW(L"0.0");
	Fog_DistanceOffset_Edit.SetWindowTextW(L"0.0");
	FOG_DistanceValue_Edit.SetWindowTextW(L"0.0");
	FOG_HeightOffset_Edit.SetWindowTextW(L"0.0");
	FOG_HeightValue_Edit.SetWindowTextW(L"0.0");
	FOG_ColorR_Edit.SetWindowTextW(L"0.0");
	FOG_ColorG_Edit.SetWindowTextW(L"0.0");
	FOG_ColorB_Edit.SetWindowTextW(L"0.0");
}

void SceneSetting::Bloom_DataSetting()
{
	Bloom_Factor_Slider.SetRange(0,1000);
	Bloom_Threshold_Min_Slider.SetRange(0,2000);
	Bloom_Threshold_Max_Slider.SetRange(10000, 1000000);

	Bloom_Factor_Edit.SetWindowTextW(L"0");
	Bloom_Threshold_Min_Edit.SetWindowTextW(L"0");
	Bloom_Threshold_Max_Edit.SetWindowTextW(L"100");
}

//void SceneSetting::IBL_DataSetting()
//{
//	IBL_Factor_Slider.SetRange(1, 5000);
//	IBL_Factor_Slider.SetPos(0);
//	IBL_Factor_Edit.SetWindowTextW(L"0");
//}
//
//void SceneSetting::Environment_DataSetting()
//{
//	Environment_size_Slider.SetRange(1, 5000);
//	Environment_size_Slider.SetPos(500);
//	Environment_size_Edit.SetWindowTextW(L"500.0");
//}

void SceneSetting::SSAO_Slider_Setting(RenderOption* Option, CScrollBar* pScrollBar)
{
	if (pScrollBar->GetDlgCtrlID() == SSAO_Radius_Slider.GetDlgCtrlID())
	{
		float Pos = (float)SSAO_Radius_Slider.GetPos();
		Option->AO_Radius = Pos  * 0.01f;
		SSAO_Radius_Edit.SetWindowTextW(ChangeToCString(Pos * 0.01f));
	}

	if (pScrollBar->GetDlgCtrlID() == SSAO_SurfaceEpsiln_Slider.GetDlgCtrlID())
	{
		float Pos = (float)SSAO_SurfaceEpsiln_Slider.GetPos();
		Option->AO_SurfaceEpsilon = Pos * 0.001f;
		SSAO_SurfaceEpsiln_Edit.SetWindowTextW(ChangeToCString(Pos * 0.001f));
	}

	if (pScrollBar->GetDlgCtrlID() == SSAO_BlurCount_Slider.GetDlgCtrlID())
	{
		int Pos = SSAO_BlurCount_Slider.GetPos();
		Option->AO_BlurCount = Pos;
		SSAO_Blur_Edit.SetWindowTextW(ChangeToCString(Pos));
	}
}

void SceneSetting::FOG_Slider_Setting(RenderOption* Option, CScrollBar* pScrollBar)
{
	if (pScrollBar->GetDlgCtrlID() == FOG_ColorR_Slider.GetDlgCtrlID())
	{
		float Pos = (float)FOG_ColorR_Slider.GetPos();
		Option->FOG_Color.x = Pos/255;
		FOG_ColorR_Edit.SetWindowTextW(ChangeToCString(Pos));
	}

	if (pScrollBar->GetDlgCtrlID() == FOG_ColorG_Slider.GetDlgCtrlID())
	{
		float Pos = (float)FOG_ColorG_Slider.GetPos();
		Option->FOG_Color.y = Pos / 255;
		FOG_ColorG_Edit.SetWindowTextW(ChangeToCString(Pos));
	}

	if (pScrollBar->GetDlgCtrlID() == FOG_ColorB_Slider.GetDlgCtrlID())
	{
		float Pos = (float)FOG_ColorB_Slider.GetPos();
		Option->FOG_Color.z = Pos / 255;
		FOG_ColorB_Edit.SetWindowTextW(ChangeToCString(Pos));
	}

	if (pScrollBar->GetDlgCtrlID() == FOG_MoveSpeed_Slider.GetDlgCtrlID())
	{
		float Pos = (float)FOG_MoveSpeed_Slider.GetPos();
		Option->FOG_MoveSpeed = Pos * 0.001f;
		FOG_MoveSpeed_Edit.SetWindowTextW(ChangeToCString(Pos * 0.001f));
	}

	if (pScrollBar->GetDlgCtrlID() == FOG_StartDistance_Slider.GetDlgCtrlID())
	{
		float Pos = (float)FOG_StartDistance_Slider.GetPos();
		Option->FOG_StartDistance = Pos;
		FOG_StartDistance_Edit.SetWindowTextW(ChangeToCString(Pos));
	}

	if (pScrollBar->GetDlgCtrlID() == FOG_DistanceValue_Slider.GetDlgCtrlID())
	{
		float Pos = (float)FOG_DistanceValue_Slider.GetPos();
		Option->FOG_DistanceValue = Pos;
		FOG_DistanceValue_Edit.SetWindowTextW(ChangeToCString(Pos));
	}

	if (pScrollBar->GetDlgCtrlID() == FOG_HeightOffset_Slider.GetDlgCtrlID())
	{
		float Pos = (float)FOG_HeightOffset_Slider.GetPos();
		Option->FOG_HeightOffset = Pos;
		FOG_HeightOffset_Edit.SetWindowTextW(ChangeToCString(Pos));
	}

	if (pScrollBar->GetDlgCtrlID() == FOG_HeightValue_Slider.GetDlgCtrlID())
	{
		float Pos = (float)FOG_HeightValue_Slider.GetPos();
		Option->FOG_HeightValue = Pos;
		FOG_HeightValue_Edit.SetWindowTextW(ChangeToCString(Pos));
	}

	if (pScrollBar->GetDlgCtrlID() == Fog_DistanceOffset_Slider.GetDlgCtrlID())
	{
		float Pos = (float)Fog_DistanceOffset_Slider.GetPos();
		Option->FOG_DistanceOffset = Pos;
		Fog_DistanceOffset_Edit.SetWindowTextW(ChangeToCString(Pos));
	}
}

//void SceneSetting::Environment_Slider_Setting(RenderOption* Option, CScrollBar* pScrollBar)
//{
//	if (pScrollBar->GetDlgCtrlID() == Environment_size_Slider.GetDlgCtrlID())
//	{
//		float Pos = (float)Environment_size_Slider.GetPos();
//		Option->SkyCube_Size = Pos;
//		Environment_size_Edit.SetWindowTextW(ChangeToCString(Pos));
//	}
//}
//
//void SceneSetting::IBL_Slider_Setting(RenderOption* Option, CScrollBar* pScrollBar)
//{
//	if (pScrollBar->GetDlgCtrlID() == IBL_Factor_Slider.GetDlgCtrlID())
//	{
//		float Pos = (float)IBL_Factor_Slider.GetPos();
//		Option->SkyLight_Factor = Pos * 0.001f;
//		IBL_Factor_Edit.SetWindowTextW(ChangeToCString(Pos * 0.001f));
//	}
//}

void SceneSetting::Bloom_Slider_Setting(RenderOption* Option, CScrollBar* pScrollBar)
{
	if (pScrollBar->GetDlgCtrlID() == Bloom_Threshold_Min_Slider.GetDlgCtrlID())
	{
		float Pos = (float)Bloom_Threshold_Min_Slider.GetPos();
		Option->BLOOM_Threshold_Min = Pos * 0.001f;
		Bloom_Threshold_Min_Edit.SetWindowTextW(ChangeToCString(Pos *  0.001f));
	}

	if (pScrollBar->GetDlgCtrlID() == Bloom_Threshold_Max_Slider.GetDlgCtrlID())
	{
		float Pos = (float)Bloom_Threshold_Max_Slider.GetPos();
		Option->BLOOM_Threshold_Max = Pos * 0.001f;
		Bloom_Threshold_Max_Edit.SetWindowTextW(ChangeToCString(Pos * 0.001f));
	}

	if (pScrollBar->GetDlgCtrlID() == Bloom_Factor_Slider.GetDlgCtrlID())
	{
		float Pos = (float)Bloom_Factor_Slider.GetPos();
		Option->BLOOM_Factor = Pos * 0.001f;
		Bloom_Factor_Edit.SetWindowTextW(ChangeToCString(Pos * 0.001f));
	}


}

void SceneSetting::Setting()
{
	RenderOption* mOption = GetRenderOptionData();

	///SSAO
	SSAO_Radius_Slider.SetPos(mOption->AO_Radius * 100);
	SSAO_Radius_Edit.SetWindowTextW(ChangeToCString(mOption->AO_Radius));
	SSAO_SurfaceEpsiln_Slider.SetPos(mOption->AO_SurfaceEpsilon * 1000);
	SSAO_SurfaceEpsiln_Edit.SetWindowTextW(ChangeToCString(mOption->AO_SurfaceEpsilon));
	SSAO_BlurCount_Slider.SetPos(mOption->AO_BlurCount);
	SSAO_Blur_Edit.SetWindowTextW(ChangeToCString(mOption->AO_BlurCount));

	///FOG
	FOG_ColorR_Slider.SetPos(mOption->FOG_Color.x * 255);
	FOG_ColorG_Slider.SetPos(mOption->FOG_Color.y * 255);
	FOG_ColorB_Slider.SetPos(mOption->FOG_Color.z * 255);
	FOG_ColorR_Edit.SetWindowTextW(ChangeToCString(mOption->FOG_Color.x * 255));
	FOG_ColorG_Edit.SetWindowTextW(ChangeToCString(mOption->FOG_Color.y * 255));
	FOG_ColorB_Edit.SetWindowTextW(ChangeToCString(mOption->FOG_Color.z * 255));
	FOG_MoveSpeed_Slider.SetPos((int)mOption->FOG_MoveSpeed * 100);
	FOG_MoveSpeed_Edit.SetWindowTextW(ChangeToCString(mOption->FOG_MoveSpeed));
	FOG_StartDistance_Slider.SetPos((int)mOption->FOG_StartDistance);
	FOG_StartDistance_Edit.SetWindowTextW(ChangeToCString(mOption->FOG_StartDistance));
	FOG_DistanceValue_Slider.SetPos((int)mOption->FOG_DistanceValue);
	FOG_DistanceValue_Edit.SetWindowTextW(ChangeToCString(mOption->FOG_DistanceValue));
	FOG_HeightOffset_Slider.SetPos((int)mOption->FOG_HeightOffset);
	FOG_HeightOffset_Edit.SetWindowTextW(ChangeToCString(mOption->FOG_HeightOffset));
	FOG_HeightValue_Slider.SetPos((int)mOption->FOG_HeightValue);
	FOG_HeightValue_Edit.SetWindowTextW(ChangeToCString(mOption->FOG_HeightValue));
	Fog_DistanceOffset_Slider.SetPos((int)mOption->FOG_DistanceOffset);
	Fog_DistanceOffset_Edit.SetWindowTextW(ChangeToCString(mOption->FOG_DistanceOffset));

	///SkyCube
	//Environment_size_Slider.SetPos(mOption->SkyCube_Size);
	//Environment_size_Edit.SetWindowTextW(ChangeToCString(mOption->SkyCube_Size));

	///Bloom
	Bloom_Threshold_Min_Slider.SetPos((int)mOption->BLOOM_Threshold_Min * 1000);
	Bloom_Threshold_Min_Edit.SetWindowTextW(ChangeToCString(mOption->BLOOM_Threshold_Min));
	Bloom_Threshold_Max_Slider.SetPos((int)mOption->BLOOM_Threshold_Max * 1000);
	Bloom_Threshold_Max_Edit.SetWindowTextW(ChangeToCString(mOption->BLOOM_Threshold_Max));
	Bloom_Factor_Slider.SetPos((int)mOption->BLOOM_Factor * 1000);
	Bloom_Factor_Edit.SetWindowTextW(ChangeToCString(mOption->BLOOM_Factor));

	///SkyLight
	//IBL_Factor_Slider.SetPos((int)mOption->SkyLight_Factor * 1000);
	//IBL_Factor_Edit.SetWindowTextW(ChangeToCString(mOption->SkyLight_Factor));
}


BEGIN_MESSAGE_MAP(SceneSetting, CDialogEx)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHECK1, &SceneSetting::OnDebugButton)
	ON_BN_CLICKED(IDC_CHECK6, &SceneSetting::OnHDR_Button)
	ON_BN_CLICKED(IDC_CHECK3, &SceneSetting::OnSSAO_Button)
	ON_BN_CLICKED(IDC_CHECK4, &SceneSetting::OnFOG_Button)
	ON_BN_CLICKED(IDC_CHECK2, &SceneSetting::OnShadow_Button)
	ON_BN_CLICKED(IDC_CHECK9, &SceneSetting::OnIBL_Button)
	ON_BN_CLICKED(IDC_CHECK5, &SceneSetting::OnBloom_Button)
	ON_BN_CLICKED(IDC_CHECK10, &SceneSetting::OnFXAA_Button)
	ON_BN_CLICKED(IDC_CHECK12, &SceneSetting::OnBnClickedCheck12)
END_MESSAGE_MAP()


// SceneSetting 메시지 처리기


void SceneSetting::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	RenderOption* Option = GetRenderOptionData();

	SSAO_Slider_Setting(Option, pScrollBar);
	FOG_Slider_Setting(Option, pScrollBar);
	//IBL_Slider_Setting(Option, pScrollBar);
	Bloom_Slider_Setting(Option, pScrollBar);
	//Environment_Slider_Setting(Option, pScrollBar);
	
	RenderSetting();
	CustomDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void SceneSetting::OnDebugButton()
{
	RenderOption* Option = GetRenderOptionData();
	Option->DebugOption ^= DEBUG_OPTION::DEBUG_MODE;
	RenderSetting();
}


void SceneSetting::OnHDR_Button()
{
	RenderOption* Option = GetRenderOptionData();
	Option->PostProcessOption ^= POSTPROCESS_OPTION::RENDER_HDR;
	RenderSetting();
}


void SceneSetting::OnSSAO_Button()
{
	RenderOption* Option = GetRenderOptionData();
	Option->RenderingOption ^= RENDER_OPTION::RENDER_SSAO;
	RenderSetting();
}


void SceneSetting::OnFOG_Button()
{
	RenderOption* Option = GetRenderOptionData();
	Option->RenderingOption ^= RENDER_OPTION::RENDER_FOG;
	RenderSetting();
}


void SceneSetting::OnShadow_Button()
{
	RenderOption* Option = GetRenderOptionData();
	Option->RenderingOption ^= RENDER_OPTION::RENDER_SHADOW;
	RenderSetting();
}


void SceneSetting::OnIBL_Button()
{
	RenderOption* Option = GetRenderOptionData();
	Option->RenderingOption ^= RENDER_OPTION::RENDER_IBL;
	RenderSetting();
}


void SceneSetting::OnBloom_Button()
{
	RenderOption* Option = GetRenderOptionData();
	Option->PostProcessOption ^= POSTPROCESS_OPTION::RENDER_BLOOM;
	RenderSetting();
}


void SceneSetting::OnFXAA_Button()
{
	RenderOption* Option = GetRenderOptionData();
	Option->PostProcessOption ^= POSTPROCESS_OPTION::RENDER_FXAA;
	RenderSetting();
}


void SceneSetting::OnBnClickedCheck12()
{
	RenderOption* Option = GetRenderOptionData();
	Option->DebugOption ^= DEBUG_OPTION::DEBUG_RENDERTARGET;
	RenderSetting();
}



BOOL SceneSetting::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	mTooltip.RelayEvent(pMsg);
	return CustomDialog::PreTranslateMessage(pMsg);
}

