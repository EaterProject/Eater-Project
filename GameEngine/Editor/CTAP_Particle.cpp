// CTAP_Particle.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "afxdialogex.h"
#include "CTAP_Particle.h"
#include "DialogFactory.h"
#include "ParticleSystem.h"
#include "EditorManager.h"
#include "DialogFactory.h"

// CTAP_Particle 대화 상자

IMPLEMENT_DYNAMIC(CTAP_Particle, CustomDialog)

CTAP_Particle::CTAP_Particle(CWnd* pParent /*=nullptr*/)
	: CustomDialog(IDD_TAP_PARTICLE, pParent)
	, MaxParticle(0)
	, StartColor_Min_Alpha(0)
	, StartColor_Max_Alpha(0)
	, LifeColor_Min_Alpha(0)
	, LifeColor_Max_Alpha(0)
	, DelayTime(0)
	, ShapeRadius_X(0)
	, ShapeRadius_Y(0)
	, ShapeRadius_Z(0)
	, Tiling_X(0)
	, Tiling_Y(0)
	, RateOverTime(0)
	, Strength_Str(_T(""))
{

}

CTAP_Particle::~CTAP_Particle()
{
}

void CTAP_Particle::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, RenderType_Combo);
	DDX_Control(pDX, IDC_EDIT16, StartForce_X);
	DDX_Control(pDX, IDC_EDIT17, StartForce_Y);
	DDX_Control(pDX, IDC_EDIT18, StartForce_Z);
	DDX_Control(pDX, IDC_EDIT21, StartForce_X_R);
	DDX_Control(pDX, IDC_EDIT22, StartForce_Y_R);
	DDX_Control(pDX, IDC_EDIT23, StartForce_Z_R);
	DDX_Control(pDX, IDC_EDIT33, StartLifeTime_min);
	DDX_Control(pDX, IDC_EDIT34, StartLifeTime_max);
	DDX_Control(pDX, IDC_EDIT35, StartScale_min);
	DDX_Control(pDX, IDC_EDIT36, StartScale_max);
	DDX_Control(pDX, IDC_EDIT38, StartRotation_min);
	DDX_Control(pDX, IDC_EDIT39, StartRotation_max);
	DDX_Control(pDX, IDC_EDIT40, LifeForce_X);
	DDX_Control(pDX, IDC_EDIT41, LifeForce_Y);
	DDX_Control(pDX, IDC_EDIT42, LifeForce_Z);
	DDX_Control(pDX, IDC_EDIT43, LifeForce_X_max);
	DDX_Control(pDX, IDC_EDIT44, LifeForce_Y_max);
	DDX_Control(pDX, IDC_EDIT45, LifeForce_Z_max);
	DDX_Control(pDX, IDC_EDIT47, LifeRotation_min);
	DDX_Control(pDX, IDC_EDIT48, LifeRotation_max);
	DDX_Control(pDX, IDC_EDIT50, LifeScale_min);
	DDX_Control(pDX, IDC_EDIT51, LifeScale_max);
	DDX_Control(pDX, IDC_COMBO3, LifeScale_Combo);
	DDX_Control(pDX, IDC_COMBO4, LifeColor_Combo);
	DDX_Text(pDX, IDC_MAX_PARTICLE, MaxParticle);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON1, Custom_StartColor_Min_Button);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON2, Custom_StartColor_Max_Button);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON3, Custom_LifeColor_Min_Button);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON4, Custom_LifeColor_Max_Button);
	DDX_Text(pDX, IDC_EDIT27, StartColor_Min_Alpha);
	DDX_Text(pDX, IDC_EDIT31, StartColor_Max_Alpha);
	DDX_Text(pDX, IDC_EDIT54, LifeColor_Min_Alpha);
	DDX_Text(pDX, IDC_EDIT58, LifeColor_Max_Alpha);
	DDX_Text(pDX, IDC_EDIT4, DelayTime);
	DDX_Text(pDX, IDC_EDIT8, ShapeRadius_X);
	DDX_Text(pDX, IDC_EDIT9, ShapeRadius_Y);
	DDX_Text(pDX, IDC_EDIT10, ShapeRadius_Z);
	DDX_Text(pDX, IDC_EDIT6, Tiling_X);
	DDX_Text(pDX, IDC_EDIT7, Tiling_Y);
	DDX_Text(pDX, IDC_EDIT64, RateOverTime);
	DDX_Control(pDX, IDC_SLIDER21, Strength_Slider);
	DDX_Text(pDX, IDC_EDIT65, Strength_Str);
}

BOOL CTAP_Particle::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	RenderType_Combo.InsertString(0, L"BILLBOARD");
	RenderType_Combo.InsertString(1, L"VERTICAL");
	RenderType_Combo.InsertString(2, L"HORIZONTAL");
	RenderType_Combo.InsertString(3, L"MESH");
	RenderType_Combo.SetCurSel(0);
	
	LifeColor_Combo.InsertString(0, L"NONE");
	LifeColor_Combo.InsertString(1, L"UP");
	LifeColor_Combo.InsertString(2, L"DOWN");
	LifeColor_Combo.InsertString(3, L"UPDOWN");
	
	StartForce_X.SetWindowTextW(L"0");
	StartForce_Y.SetWindowTextW(L"0");
	StartForce_Z.SetWindowTextW(L"0");
	StartForce_X_R.SetWindowTextW(L"0");
	StartForce_Y_R.SetWindowTextW(L"0");
	StartForce_Z_R.SetWindowTextW(L"0");
	
	StartLifeTime_min.SetWindowTextW(L"0");
	StartLifeTime_max.SetWindowTextW(L"1");
	
	StartScale_min.SetWindowTextW(L"0");
	StartScale_max.SetWindowTextW(L"1");
	
	StartScale_max.SetWindowTextW(L"1");
	StartScale_min.SetWindowTextW(L"1");
	
	StartRotation_min.SetWindowTextW(L"0");
	StartRotation_max.SetWindowTextW(L"0");
	
	LifeForce_X.SetWindowTextW(L"0");
	LifeForce_Y.SetWindowTextW(L"0");
	LifeForce_Z.SetWindowTextW(L"0");
	LifeForce_X_max.SetWindowTextW(L"0");
	LifeForce_Y_max.SetWindowTextW(L"0");
	LifeForce_Z_max.SetWindowTextW(L"0");
	
	
	LifeRotation_min.SetWindowTextW(L"0");
	LifeRotation_max.SetWindowTextW(L"0");
	
	LifeScale_Combo.InsertString(0, L"NONE");
	LifeScale_Combo.InsertString(1, L"UP");
	LifeScale_Combo.InsertString(2, L"DOWN");
	LifeScale_Combo.InsertString(3, L"UPDOWN");
	LifeScale_Combo.SetCurSel(0);
	LifeScale_min.SetWindowTextW(L"1");
	LifeScale_max.SetWindowTextW(L"1");
	
	LifeColor_Combo.SetCurSel(0);
	
	Strength_Slider.SetRange(1, 100);
	Strength_Slider.SetPos(1);
	return 0;
}

void CTAP_Particle::UpDateSetting()
{
	//랜더 옵션 셋팅
	int Choice01 = RenderType_Combo.GetCurSel();
	switch (Choice01)
	{
	case 0:
		mParticleSystem->SetRenderType(PARTICLE_RENDER_OPTION::BILLBOARD);
		break;
	case 1:
		mParticleSystem->SetRenderType(PARTICLE_RENDER_OPTION::VERTICAL_BILLBOARD);
		break;
	case 2:
		mParticleSystem->SetRenderType(PARTICLE_RENDER_OPTION::HORIZONTAL_BILLBOARD);
		break;
	case 3:
		mParticleSystem->SetRenderType(PARTICLE_RENDER_OPTION::MESH);
		break;
	}

	//최대 카운터 셋팅
	CString Number;
	mParticleSystem->SetMaxParticles(MaxParticle);
	mParticleSystem->SetDelayTime(DelayTime);
	//딜레시 시간 셋팅

	//범위 조정 셋팅
	int Check;
	CString NumberX, NumberY, NumberZ;
	mParticleSystem->SetShapeRadius(ShapeRadius_X, ShapeRadius_Y, ShapeRadius_Z);

	//타일링 셋팅
	mParticleSystem->SetTextureTiling(Tiling_X, Tiling_Y);

	//1초에 출력할 파티클 개수
	mParticleSystem->SetRateOverTime(RateOverTime);
}

void CTAP_Particle::UpDateStartSetting()
{
	//Start Setting
	int Check;
	CString NumberX, NumberY, NumberZ, NumberW;
	
	StartForce_X.GetWindowTextW(NumberX);
	StartForce_Y.GetWindowTextW(NumberY);
	StartForce_Z.GetWindowTextW(NumberZ);
	Vector3 Start = Vector3(ChangeToFloat(NumberX), ChangeToFloat(NumberY), ChangeToFloat(NumberZ));
	StartForce_X_R.GetWindowTextW(NumberX);
	StartForce_Y_R.GetWindowTextW(NumberY);
	StartForce_Z_R.GetWindowTextW(NumberZ);
	Vector3 End = Vector3(ChangeToFloat(NumberX), ChangeToFloat(NumberY), ChangeToFloat(NumberZ));
	mParticleSystem->SetStartForce(Start, End);

	StartColorMin.w = StartColor_Min_Alpha;
	StartColorMax.w = StartColor_Max_Alpha;
	mParticleSystem->SetStartColor(StartColorMin, StartColorMax);


	StartLifeTime_min.GetWindowTextW(NumberX);
	StartLifeTime_max.GetWindowTextW(NumberY);
	mParticleSystem->SetStartLifeTime(ChangeToFloat(NumberX), ChangeToFloat(NumberY));

	StartScale_min.GetWindowTextW(NumberX);
	StartScale_max.GetWindowTextW(NumberY);
	mParticleSystem->SetStartScale(ChangeToFloat(NumberX), ChangeToFloat(NumberY));

	StartRotation_max.GetWindowTextW(NumberX);
	StartRotation_min.GetWindowTextW(NumberY);
	mParticleSystem->SetStartRotation(ChangeToFloat(NumberX), ChangeToFloat(NumberY));
}

void CTAP_Particle::UpDateLifeSetting()
{
	CString NumberX, NumberY, NumberZ, NumberW;
	LifeForce_X.GetWindowTextW(NumberX);
	LifeForce_Y.GetWindowTextW(NumberY);
	LifeForce_Z.GetWindowTextW(NumberZ);
	Vector3 Start = Vector3(ChangeToFloat(NumberX), ChangeToFloat(NumberY), ChangeToFloat(NumberZ));
	LifeForce_X_max.GetWindowTextW(NumberX);
	LifeForce_Y_max.GetWindowTextW(NumberY);
	LifeForce_Z_max.GetWindowTextW(NumberZ);
	Vector3 End = Vector3(ChangeToFloat(NumberX), ChangeToFloat(NumberY), ChangeToFloat(NumberZ));
	mParticleSystem->SetLifeTimeForce(Start, End);

	LifeRotation_min.GetWindowTextW(NumberX);
	LifeRotation_max.GetWindowTextW(NumberY);
	mParticleSystem->SetLifeTimeRotation(ChangeToFloat(NumberX), ChangeToFloat(NumberY));

	LifeColorMin.w = LifeColor_Min_Alpha;
	LifeColorMax.w = LifeColor_Max_Alpha;
	int Choice = LifeColor_Combo.GetCurSel();
	switch (Choice)
	{
	case 0:
		mParticleSystem->SetLifeTimeColor(LifeColorMin, LifeColorMax, PARTICLE_LIFETIME_OPTION::NONE);
		break;
	case 1:
		mParticleSystem->SetLifeTimeColor(LifeColorMin, LifeColorMax, PARTICLE_LIFETIME_OPTION::UP);
		break;
	case 2:
		mParticleSystem->SetLifeTimeColor(LifeColorMin, LifeColorMax, PARTICLE_LIFETIME_OPTION::DOWN);
		break;
	case 3:
		mParticleSystem->SetLifeTimeColor(LifeColorMin, LifeColorMax, PARTICLE_LIFETIME_OPTION::UPDOWN);
		break;
	}






	Choice = LifeScale_Combo.GetCurSel();
	LifeScale_min.GetWindowTextW(NumberX);
	LifeScale_max.GetWindowTextW(NumberY);
	switch (Choice)
	{
	case 0:
		mParticleSystem->SetLifeTimeScale(ChangeToFloat(NumberX), ChangeToFloat(NumberY), PARTICLE_LIFETIME_OPTION::NONE);
		break;
	case 1:
		mParticleSystem->SetLifeTimeScale(ChangeToFloat(NumberX), ChangeToFloat(NumberY), PARTICLE_LIFETIME_OPTION::UP);
		break;
	case 2:
		mParticleSystem->SetLifeTimeScale(ChangeToFloat(NumberX), ChangeToFloat(NumberY), PARTICLE_LIFETIME_OPTION::DOWN);
		break;
	case 3:
		mParticleSystem->SetLifeTimeScale(ChangeToFloat(NumberX), ChangeToFloat(NumberY), PARTICLE_LIFETIME_OPTION::UPDOWN);
		break;
	}

	mParticleSystem->SetRateOverTime(RateOverTime);
}


BEGIN_MESSAGE_MAP(CTAP_Particle, CDialogEx)
	ON_WM_HSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON1, &CTAP_Particle::OnSaveFile)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON1, &CTAP_Particle::OnCustom_StartColor_Min_Button)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON2, &CTAP_Particle::OnCustom_StartColor_Max_Button)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON3, &CTAP_Particle::OnCustom_LifeColor_Min_Button)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON4, &CTAP_Particle::OnCustom_LifeColor_Max_Button)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CTAP_Particle 메시지 처리기


void CTAP_Particle::SetGameObject(ParticleSystem* ObjectParticleSystem)
{
	mParticleSystem = ObjectParticleSystem;
	switch (mParticleSystem->GetRenderType())
	{
	case PARTICLE_RENDER_OPTION::BILLBOARD:
		RenderType_Combo.SetCurSel(0);
		break;
	case PARTICLE_RENDER_OPTION::VERTICAL_BILLBOARD:
		RenderType_Combo.SetCurSel(1);
		break;
	case PARTICLE_RENDER_OPTION::HORIZONTAL_BILLBOARD:
		RenderType_Combo.SetCurSel(2);
		break;
	case PARTICLE_RENDER_OPTION::MESH:
		RenderType_Combo.SetCurSel(3);
		break;
	}

	MaxParticle = mParticleSystem->GetMaxParticles();
	DelayTime	= mParticleSystem->GetDelayTime();

	//ShapeRadius
	auto Shape = mParticleSystem->GetShapeRadius();
	ShapeRadius_X = Shape.x;
	ShapeRadius_Y = Shape.y;
	ShapeRadius_Z = Shape.z;
	//TextureTiling
	auto Tiling = mParticleSystem->GetTextureTiling();
	Tiling_X = Tiling.m_Width;
	Tiling_Y = Tiling.m_Height;
	//RateOverTime
	auto mRateOverTime = mParticleSystem->GetRateOverTime();
	RateOverTime = mRateOverTime;

	Strength_Slider.SetPos((int)(mParticleSystem->GetStrength()*10.0f));
	Strength_Str = ChangeToCString(mParticleSystem->GetStrength() * 10.0f);

	//StartForce
	auto StartForce = mParticleSystem->GetStartForce();
	StartForce_X.SetWindowTextW(ChangeToCString(StartForce.m_Min.x));
	StartForce_Y.SetWindowTextW(ChangeToCString(StartForce.m_Min.y));
	StartForce_Z.SetWindowTextW(ChangeToCString(StartForce.m_Min.z));
	StartForce_X_R.SetWindowTextW(ChangeToCString(StartForce.m_Max.x));
	StartForce_Y_R.SetWindowTextW(ChangeToCString(StartForce.m_Max.y));
	StartForce_Z_R.SetWindowTextW(ChangeToCString(StartForce.m_Max.z));

	//StartColor
	auto StartColor = mParticleSystem->GetStartColor();
	Custom_StartColor_Min_Button.SetColor(RGB(StartColor.m_Min.x, StartColor.m_Min.y, StartColor.m_Min.z));
	Custom_StartColor_Max_Button.SetColor(RGB(StartColor.m_Max.x, StartColor.m_Max.y, StartColor.m_Max.z));
	StartColor_Min_Alpha = StartColor.m_Min.w;
	StartColor_Max_Alpha = StartColor.m_Max.w;
	StartColorMin = { StartColor.m_Min.x, StartColor.m_Min.y, StartColor.m_Min.z ,(float)StartColor_Min_Alpha };
	StartColorMax = { StartColor.m_Max.x, StartColor.m_Max.y, StartColor.m_Max.z, (float)StartColor_Max_Alpha };

	//StartLife
	auto StartLife = mParticleSystem->GetStartLifeTime();
	StartLifeTime_min.SetWindowTextW(ChangeToCString(StartLife.m_Min));
	StartLifeTime_max.SetWindowTextW(ChangeToCString(StartLife.m_Max));

	//StartScale
	auto mStartScale = mParticleSystem->GetStartScale();
	StartScale_min.SetWindowTextW(ChangeToCString(mStartScale.m_Min));
	StartScale_max.SetWindowTextW(ChangeToCString(mStartScale.m_Max));

	//StartRotation
	auto mStartRotation = mParticleSystem->GetStartRotation();
	StartRotation_min.SetWindowTextW(ChangeToCString(mStartRotation.m_Min));
	StartRotation_max.SetWindowTextW(ChangeToCString(mStartRotation.m_Max));

	//LifeForce
	auto mLifeForce = mParticleSystem->GetLifeTimeForce();
	LifeForce_X.SetWindowTextW(ChangeToCString(mLifeForce.m_Min.x));
	LifeForce_Y.SetWindowTextW(ChangeToCString(mLifeForce.m_Min.y));
	LifeForce_Z.SetWindowTextW(ChangeToCString(mLifeForce.m_Min.z));
	LifeForce_X_max.SetWindowTextW(ChangeToCString(mLifeForce.m_Max.x));
	LifeForce_Y_max.SetWindowTextW(ChangeToCString(mLifeForce.m_Max.y));
	LifeForce_Z_max.SetWindowTextW(ChangeToCString(mLifeForce.m_Max.z));

	//LifeColor
	auto mLifeColor = mParticleSystem->GetLifeTimeColor();
	LifeColor_Min_Alpha = mLifeColor.m_Min.w;
	LifeColor_Max_Alpha = mLifeColor.m_Max.w;
	LifeColorMin = { mLifeColor.m_Min.x, mLifeColor.m_Min.y, mLifeColor.m_Min.z,(float)LifeColor_Min_Alpha };
	LifeColorMax = { mLifeColor.m_Max.x, mLifeColor.m_Max.y, mLifeColor.m_Max.z ,(float)LifeColor_Max_Alpha };
	Custom_LifeColor_Min_Button.SetColor(RGB(mLifeColor.m_Min.x, mLifeColor.m_Min.y, mLifeColor.m_Min.z));
	Custom_LifeColor_Max_Button.SetColor(RGB(mLifeColor.m_Max.x, mLifeColor.m_Max.y, mLifeColor.m_Max.z));


	switch (mParticleSystem->GetLifeTimeColorOption())
	{
	case PARTICLE_LIFETIME_OPTION::NONE:
		LifeColor_Combo.SetCurSel(0);
		break;
	case PARTICLE_LIFETIME_OPTION::UP:
		LifeColor_Combo.SetCurSel(1);
		break;
	case PARTICLE_LIFETIME_OPTION::DOWN:
		LifeColor_Combo.SetCurSel(2);
		break;
	case PARTICLE_LIFETIME_OPTION::UPDOWN:
		LifeColor_Combo.SetCurSel(3);
		break;
	}

	//LifeScale
	auto mLifeScale = mParticleSystem->GetLifeTimeScale();
	LifeScale_min.SetWindowTextW(ChangeToCString(mLifeScale.m_Min));
	LifeScale_max.SetWindowTextW(ChangeToCString(mLifeScale.m_Max));
	switch (mParticleSystem->GetLifeTimeScaleOption())
	{
	case PARTICLE_LIFETIME_OPTION::NONE:
		LifeScale_Combo.SetCurSel(0);
		break;
	case PARTICLE_LIFETIME_OPTION::UP:
		LifeScale_Combo.SetCurSel(1);
		break;
	case PARTICLE_LIFETIME_OPTION::DOWN:
		LifeScale_Combo.SetCurSel(2);
		break;
	case PARTICLE_LIFETIME_OPTION::UPDOWN:
		LifeScale_Combo.SetCurSel(3);
		break;
	}

	//LifeRotation
	auto mLifeRotation = mParticleSystem->GetLifeTimeRotation();
	LifeRotation_min.SetWindowTextW(ChangeToCString(mLifeRotation.m_Min));
	LifeRotation_max.SetWindowTextW(ChangeToCString(mLifeRotation.m_Max));

	UpdateData(FALSE);
}

void CTAP_Particle::GetObjectData()
{
	UpdateData(TRUE);
	//처음 기본셋팅
	UpDateSetting();

	//Start 관련 셋팅
	UpDateStartSetting();
	
	//Life 관련 셋팅
	UpDateLifeSetting();

	// 변한 데이터 업데이트..
	UpdateData(FALSE);
}


BOOL CTAP_Particle::PreTranslateMessage(MSG* pMsg)
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

void CTAP_Particle::OnSaveFile()
{
	EditorManager* mEater = DialogFactory::GetFactory()->GetEditorManager();
	CFileDialog mSaveFile(FALSE,L"*.Particle",NULL,OFN_OVERWRITEPROMPT, L"파티클(*.Particle)|*.Particle");
	char full[_MAX_PATH];
	_fullpath(full, "../Assets/Model/Particle", _MAX_PATH);
	CString Temp;
	Temp = full;
	mSaveFile.m_ofn.lpstrInitialDir = Temp.GetBuffer();

	if(mSaveFile.DoModal() == IDOK)
	{
		mEater->CreateParticleData(mParticleSystem->gameobject, ChangeFilePath(mSaveFile.GetPathName()));
	}
}

void CTAP_Particle::OnCustom_StartColor_Min_Button()
{
	UpdateData(TRUE);
	COLORREF mRGB = Custom_StartColor_Min_Button.GetColor();
	float R = GetRValue(mRGB);
	float G = GetGValue(mRGB);
	float B = GetBValue(mRGB);

	StartColorMin = { R,G,B,(float)StartColor_Min_Alpha };
	mParticleSystem->SetStartColor(StartColorMin, StartColorMax);
	UpdateData(FALSE);
}	

void CTAP_Particle::OnCustom_StartColor_Max_Button()
{
	UpdateData(TRUE);
	COLORREF mRGB = Custom_StartColor_Max_Button.GetColor();
	float R = GetRValue(mRGB);
	float G = GetGValue(mRGB);
	float B = GetBValue(mRGB);

	StartColorMax = { R,G,B,(float)StartColor_Max_Alpha };
	mParticleSystem->SetStartColor(StartColorMin, StartColorMax);
	UpdateData(FALSE);
}


void CTAP_Particle::OnCustom_LifeColor_Min_Button()
{
	UpdateData(TRUE);
	COLORREF mRGB = Custom_LifeColor_Min_Button.GetColor();
	float R = GetRValue(mRGB);
	float G = GetGValue(mRGB);
	float B = GetBValue(mRGB);

	LifeColorMin = { R,G,B,(float)LifeColor_Min_Alpha };
	int Choice = LifeColor_Combo.GetCurSel();
	switch (Choice)
	{
	case 0:
		mParticleSystem->SetLifeTimeColor(LifeColorMin, LifeColorMax, PARTICLE_LIFETIME_OPTION::NONE);
		break;
	case 1:
		mParticleSystem->SetLifeTimeColor(LifeColorMin, LifeColorMax, PARTICLE_LIFETIME_OPTION::UP);
		break;
	case 2:
		mParticleSystem->SetLifeTimeColor(LifeColorMin, LifeColorMax, PARTICLE_LIFETIME_OPTION::DOWN);
		break;
	case 3:
		mParticleSystem->SetLifeTimeColor(LifeColorMin, LifeColorMax, PARTICLE_LIFETIME_OPTION::UPDOWN);
		break;
	}
	UpdateData(FALSE);
}

void CTAP_Particle::OnCustom_LifeColor_Max_Button()
{
	UpdateData(TRUE);
	COLORREF mRGB = Custom_LifeColor_Max_Button.GetColor();
	float R = GetRValue(mRGB);
	float G = GetGValue(mRGB);
	float B = GetBValue(mRGB);

	
	LifeColorMax = { R,G,B,(float)LifeColor_Max_Alpha };
	int Choice = LifeColor_Combo.GetCurSel();
	switch (Choice)
	{
	case 0:
		mParticleSystem->SetLifeTimeColor(LifeColorMin, LifeColorMax, PARTICLE_LIFETIME_OPTION::NONE);
		break;
	case 1:
		mParticleSystem->SetLifeTimeColor(LifeColorMin, LifeColorMax, PARTICLE_LIFETIME_OPTION::UP);
		break;
	case 2:
		mParticleSystem->SetLifeTimeColor(LifeColorMin, LifeColorMax, PARTICLE_LIFETIME_OPTION::DOWN);
		break;
	case 3:
		mParticleSystem->SetLifeTimeColor(LifeColorMin, LifeColorMax, PARTICLE_LIFETIME_OPTION::UPDOWN);
		break;
	}
	UpdateData(FALSE);
}

void CTAP_Particle::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->GetDlgCtrlID() == Strength_Slider.GetDlgCtrlID())
	{
		float Pos = (float)Strength_Slider.GetPos();
		mParticleSystem->SetStrength(Pos*0.1f);
		Strength_Str = ChangeToCString(Pos * 0.1f);
		UpdateData(FALSE);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


BOOL CTAP_Particle::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(0, 255, 0));

	return CustomDialog::OnEraseBkgnd(pDC);
}
