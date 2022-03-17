// CTAP_Particle.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "afxdialogex.h"
#include "CTAP_Particle.h"

#include "ParticleSystem.h"
#include "GrobalFunction.h"

// CTAP_Particle 대화 상자

IMPLEMENT_DYNAMIC(CTAP_Particle, CDialogEx)

CTAP_Particle::CTAP_Particle(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAP_PARTICLE, pParent)
{

}

CTAP_Particle::~CTAP_Particle()
{
}

void CTAP_Particle::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, RenderType_Combo);
	DDX_Control(pDX, IDC_EDIT3, MaxCount_Edit);
	DDX_Control(pDX, IDC_EDIT4, DelayTime_Edit);
	DDX_Control(pDX, IDC_EDIT5, ShapeRadius_Edit);
	DDX_Control(pDX, IDC_CHECK1, ShapeRadius_Check);
	DDX_Control(pDX, IDC_EDIT6, TextureTiling_X_Edit);
	DDX_Control(pDX, IDC_EDIT7, TextureTiling_Y_Edit);
	DDX_Control(pDX, IDC_EDIT8, ShapeRadius_X_Edit);
	DDX_Control(pDX, IDC_EDIT9, ShapeRadius_Y_Edit);
	DDX_Control(pDX, IDC_EDIT10, ShapeRadius_Z_Edit);
	DDX_Control(pDX, IDC_EDIT16, StartForce_X);
	DDX_Control(pDX, IDC_EDIT17, StartForce_Y);
	DDX_Control(pDX, IDC_EDIT18, StartForce_Z);
	DDX_Control(pDX, IDC_EDIT21, StartForce_X_R);
	DDX_Control(pDX, IDC_EDIT22, StartForce_Y_R);
	DDX_Control(pDX, IDC_EDIT23, StartForce_Z_R);
	DDX_Control(pDX, IDC_CHECK2, StartForce_Check);
	DDX_Control(pDX, IDC_EDIT24, StartColor_min_R);
	DDX_Control(pDX, IDC_EDIT25, StartColor_min_G);
	DDX_Control(pDX, IDC_EDIT26, StartColor_min_B);
	DDX_Control(pDX, IDC_EDIT27, StartColor_min_A);
	DDX_Control(pDX, IDC_CHECK3, StartColor_Check);
	DDX_Control(pDX, IDC_EDIT28, StartColor_max_R);
	DDX_Control(pDX, IDC_EDIT29, StartColor_max_G);
	DDX_Control(pDX, IDC_EDIT30, StartColor_max_B);
	DDX_Control(pDX, IDC_EDIT31, StartColor_max_A);
	DDX_Control(pDX, IDC_EDIT32, StartLifeTime);
	DDX_Control(pDX, IDC_EDIT33, StartLifeTime_min);
	DDX_Control(pDX, IDC_EDIT34, StartLifeTime_max);
	DDX_Control(pDX, IDC_CHECK4, StartLife_Check);
	DDX_Control(pDX, IDC_EDIT15, StartScale);
	DDX_Control(pDX, IDC_EDIT35, StartScale_min);
	DDX_Control(pDX, IDC_EDIT36, StartScale_max);
	DDX_Control(pDX, IDC_CHECK5, StartScale_Check);
	DDX_Control(pDX, IDC_EDIT37, StartRotation);
	DDX_Control(pDX, IDC_CHECK6, StartRotation_Check);
	DDX_Control(pDX, IDC_EDIT38, StartRotation_min);
	DDX_Control(pDX, IDC_EDIT39, StartRotation_max);
	DDX_Control(pDX, IDC_EDIT40, LifeForce_X);
	DDX_Control(pDX, IDC_EDIT41, LifeForce_Y);
	DDX_Control(pDX, IDC_EDIT42, LifeForce_Z);
	DDX_Control(pDX, IDC_EDIT43, LifeForce_X_max);
	DDX_Control(pDX, IDC_EDIT44, LifeForce_Y_max);
	DDX_Control(pDX, IDC_EDIT45, LifeForce_Z_max);
	DDX_Control(pDX, IDC_CHECK7, LifeForce_Check);
	DDX_Control(pDX, IDC_EDIT46, LifeRotation);
	DDX_Control(pDX, IDC_EDIT47, LifeRotation_min);
	DDX_Control(pDX, IDC_EDIT48, LifeRotation_max);
	DDX_Control(pDX, IDC_CHECK8, LifeRotation_Check);
	DDX_Control(pDX, IDC_EDIT50, LifeScale_min);
	DDX_Control(pDX, IDC_EDIT51, LifeScale_max);
	DDX_Control(pDX, IDC_COMBO3, LifeScale_Combo);
	DDX_Control(pDX, IDC_EDIT49, LifeColor_min_R);
	DDX_Control(pDX, IDC_EDIT52, LifeColor_min_G);
	DDX_Control(pDX, IDC_EDIT53, LifeColor_min_B);
	DDX_Control(pDX, IDC_EDIT54, LifeColor_min_A);
	DDX_Control(pDX, IDC_EDIT55, LifeColor_max_R);
	DDX_Control(pDX, IDC_EDIT56, LifeColor_max_G);
	DDX_Control(pDX, IDC_EDIT57, LifeColor_max_B);
	DDX_Control(pDX, IDC_EDIT58, LifeColor_max_A);
	DDX_Control(pDX, IDC_COMBO4, LifeColor_Combo);
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

	ShapeRadius_X_Edit.ShowWindow(SW_HIDE);
	ShapeRadius_Y_Edit.ShowWindow(SW_HIDE);
	ShapeRadius_Z_Edit.ShowWindow(SW_HIDE);
	ShapeRadius_X_Edit.SetWindowTextW(L"0");
	ShapeRadius_Y_Edit.SetWindowTextW(L"0");
	ShapeRadius_Z_Edit.SetWindowTextW(L"0");

	StartForce_X.SetWindowTextW(L"0");
	StartForce_Y.SetWindowTextW(L"0");
	StartForce_Z.SetWindowTextW(L"0");
	StartForce_X_R.SetWindowTextW(L"0");
	StartForce_Y_R.SetWindowTextW(L"0");
	StartForce_Z_R.SetWindowTextW(L"0");

	StartForce_X_R.ShowWindow(SW_HIDE);
	StartForce_Y_R.ShowWindow(SW_HIDE);
	StartForce_Z_R.ShowWindow(SW_HIDE);


	StartColor_max_R.SetWindowTextW(L"255");
	StartColor_max_G.SetWindowTextW(L"255");
	StartColor_max_B.SetWindowTextW(L"255");
	StartColor_max_A.SetWindowTextW(L"255");
	StartColor_min_R.SetWindowTextW(L"255");
	StartColor_min_G.SetWindowTextW(L"255");
	StartColor_min_B.SetWindowTextW(L"255");
	StartColor_min_A.SetWindowTextW(L"255");

	StartColor_max_R.ShowWindow(SW_HIDE);
	StartColor_max_G.ShowWindow(SW_HIDE);
	StartColor_max_B.ShowWindow(SW_HIDE);
	StartColor_max_A.ShowWindow(SW_HIDE);
	StartColor_min_R.ShowWindow(SW_SHOW);
	StartColor_min_G.ShowWindow(SW_SHOW);
	StartColor_min_B.ShowWindow(SW_SHOW);
	StartColor_min_A.ShowWindow(SW_SHOW);


	StartLifeTime.SetWindowTextW(L"1");
	StartLifeTime_min.SetWindowTextW(L"0");
	StartLifeTime_max.SetWindowTextW(L"1");
	StartLifeTime_min.ShowWindow(SW_HIDE);
	StartLifeTime_max.ShowWindow(SW_HIDE);

	StartScale.SetWindowTextW(L"1");
	StartScale_min.SetWindowTextW(L"0");
	StartScale_max.SetWindowTextW(L"1");
	StartLifeTime_min.ShowWindow(SW_HIDE);
	StartLifeTime_max.ShowWindow(SW_HIDE);

	StartScale_max.SetWindowTextW(L"1");
	StartScale_min.SetWindowTextW(L"1");
	StartScale.SetWindowTextW(L"1");
	StartScale_max.ShowWindow(SW_HIDE);
	StartScale_min.ShowWindow(SW_HIDE);
	

	StartRotation.SetWindowTextW(L"0");
	StartRotation_min.SetWindowTextW(L"0");
	StartRotation_max.SetWindowTextW(L"0");
	StartRotation_min.ShowWindow(SW_HIDE);
	StartRotation_max.ShowWindow(SW_HIDE);

	LifeForce_X.SetWindowTextW(L"0");
	LifeForce_Y.SetWindowTextW(L"0");
	LifeForce_Z.SetWindowTextW(L"0");
	LifeForce_X_max.SetWindowTextW(L"0");
	LifeForce_Y_max.SetWindowTextW(L"0");
	LifeForce_Z_max.SetWindowTextW(L"0");

	LifeForce_X_max.ShowWindow(SW_HIDE);
	LifeForce_Y_max.ShowWindow(SW_HIDE);
	LifeForce_Z_max.ShowWindow(SW_HIDE);


	LifeRotation.SetWindowTextW(L"0");
	LifeRotation_min.SetWindowTextW(L"0");
	LifeRotation_max.SetWindowTextW(L"0");

	LifeRotation.ShowWindow(SW_SHOW);
	LifeRotation_min.ShowWindow(SW_HIDE);
	LifeRotation_max.ShowWindow(SW_HIDE);

	LifeScale_Combo.InsertString(0, L"NONE");
	LifeScale_Combo.InsertString(1, L"UP");
	LifeScale_Combo.InsertString(2, L"DOWN");
	LifeScale_Combo.InsertString(3, L"UPDOWN");
	LifeScale_Combo.SetCurSel(0);
	LifeScale_min.SetWindowTextW(L"1");
	LifeScale_max.SetWindowTextW(L"1");

	LifeColor_min_R.SetWindowTextW(L"255");
	LifeColor_min_G.SetWindowTextW(L"255");
	LifeColor_min_B.SetWindowTextW(L"255");
	LifeColor_min_A.SetWindowTextW(L"255");
	LifeColor_max_R.SetWindowTextW(L"255");
	LifeColor_max_G.SetWindowTextW(L"255");
	LifeColor_max_B.SetWindowTextW(L"255");
	LifeColor_max_A.SetWindowTextW(L"255");
	LifeColor_Combo.SetCurSel(0);

	return 0;
}


BEGIN_MESSAGE_MAP(CTAP_Particle, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK1, &CTAP_Particle::OnShapeRadius_Button)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_CHECK2, &CTAP_Particle::OnStartForce_Button)
	ON_BN_CLICKED(IDC_CHECK3, &CTAP_Particle::OnStartColor_Button)
	ON_BN_CLICKED(IDC_CHECK4, &CTAP_Particle::OnStartLifeTime_Check)
	ON_BN_CLICKED(IDC_CHECK5, &CTAP_Particle::OnStartScale_Button)
	ON_BN_CLICKED(IDC_CHECK6, &CTAP_Particle::OnStartRotationCheck)
	ON_BN_CLICKED(IDC_CHECK7, &CTAP_Particle::OnLifeForceCheck)
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_CHECK8, &CTAP_Particle::OnBnClickedCheck8)
END_MESSAGE_MAP()


// CTAP_Particle 메시지 처리기


void CTAP_Particle::SetGameObject(ParticleSystem* ObjectParticleSystem)
{
	mParticleSystem = ObjectParticleSystem;

	CRect rt;
	GetWindowRect(rt);
	int Num = rt.Height();


	MaxCount_Edit.SetWindowTextW(L"0");
	DelayTime_Edit.SetWindowTextW(L"0");
	ShapeRadius_Edit.SetWindowTextW(L"0");
	TextureTiling_X_Edit.SetWindowTextW(L"1");
	TextureTiling_Y_Edit.SetWindowTextW(L"1");
}

void CTAP_Particle::GetObjectData()
{
	CString Temp01;
	CString Temp02;
	CString Temp03;
	CString Temp04;
	int Check;

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

	MaxCount_Edit.GetWindowTextW(Temp01);
	mParticleSystem->SetMaxParticles(ChangeToInt(Temp01));

	DelayTime_Edit.GetWindowTextW(Temp01);
	mParticleSystem->SetDelayTime(ChangeToInt(Temp01));

	Check = ShapeRadius_Check.GetCheck();
	if (Check == true)
	{
		ShapeRadius_X_Edit.GetWindowTextW(Temp01);
		ShapeRadius_Y_Edit.GetWindowTextW(Temp02);
		ShapeRadius_Z_Edit.GetWindowTextW(Temp03);
		mParticleSystem->SetShapeRadius(ChangeToFloat(Temp01), ChangeToFloat(Temp02), ChangeToFloat(Temp03));
	}
	else
	{
		ShapeRadius_Edit.GetWindowTextW(Temp01);
		mParticleSystem->SetShapeRadius(ChangeToInt(Temp01));
	}

	TextureTiling_X_Edit.GetWindowTextW(Temp01);
	TextureTiling_Y_Edit.GetWindowTextW(Temp02);
	mParticleSystem->SetTextureTiling(ChangeToInt(Temp01), ChangeToInt(Temp02));


	Check = StartForce_Check.GetCheck();
	if (Check == true)
	{
		StartForce_X.GetWindowTextW(Temp01);
		StartForce_Y.GetWindowTextW(Temp02);
		StartForce_Z.GetWindowTextW(Temp03);
		Vector3 Start = Vector3(ChangeToFloat(Temp01), ChangeToFloat(Temp02), ChangeToFloat(Temp03));
		StartForce_X_R.GetWindowTextW(Temp01);
		StartForce_Y_R.GetWindowTextW(Temp02);
		StartForce_Z_R.GetWindowTextW(Temp03);
		Vector3 End = Vector3(ChangeToFloat(Temp01), ChangeToFloat(Temp02), ChangeToFloat(Temp03));
		mParticleSystem->SetStartForce(Start,End);
	}
	else
	{
		StartForce_X.GetWindowTextW(Temp01);
		StartForce_Y.GetWindowTextW(Temp02);
		StartForce_Z.GetWindowTextW(Temp03);
		Vector3 Start = Vector3(ChangeToFloat(Temp01), ChangeToFloat(Temp02), ChangeToFloat(Temp03));
		mParticleSystem->SetStartForce(Start);
	}

	Check = StartColor_Check.GetCheck();
	if (Check == true)
	{
		StartColor_min_R.GetWindowTextW(Temp01);
		StartColor_min_G.GetWindowTextW(Temp02);
		StartColor_min_B.GetWindowTextW(Temp03);
		StartColor_min_A.GetWindowTextW(Temp04);
		Vector4 Start = Vector4(ChangeToFloat(Temp01), ChangeToFloat(Temp02), ChangeToFloat(Temp03), ChangeToFloat(Temp04));
		StartColor_max_R.GetWindowTextW(Temp01);
		StartColor_max_G.GetWindowTextW(Temp02);
		StartColor_max_B.GetWindowTextW(Temp03);
		StartColor_max_A.GetWindowTextW(Temp04);
		Vector4 End = Vector4(ChangeToFloat(Temp01), ChangeToFloat(Temp02), ChangeToFloat(Temp03), ChangeToFloat(Temp04));
		mParticleSystem->SetStartColor(Start, End);
	}
	else
	{
		StartColor_min_R.GetWindowTextW(Temp01);
		StartColor_min_G.GetWindowTextW(Temp02);
		StartColor_min_B.GetWindowTextW(Temp03);
		StartColor_min_A.GetWindowTextW(Temp04);
		Vector4 Start = Vector4(ChangeToFloat(Temp01), ChangeToFloat(Temp02), ChangeToFloat(Temp03), ChangeToFloat(Temp04));
		mParticleSystem->SetStartColor(Start);
	}

	Check = StartLife_Check.GetCheck();
	if (Check == true)
	{
		StartLifeTime_min.GetWindowTextW(Temp01);
		StartLifeTime_max.GetWindowTextW(Temp02);
		mParticleSystem->SetStartLifeTime(ChangeToFloat(Temp01), ChangeToFloat(Temp02));
	}
	else
	{
		StartLifeTime.GetWindowTextW(Temp01);
		mParticleSystem->SetStartLifeTime(ChangeToFloat(Temp01));
	}

	Check = StartScale_Check.GetCheck();
	if (Check == true)
	{
		StartScale_min.GetWindowTextW(Temp01);
		StartScale_max.GetWindowTextW(Temp02);
		mParticleSystem->SetStartScale(ChangeToFloat(Temp01), ChangeToFloat(Temp02));
	}
	else
	{
		StartScale.GetWindowTextW(Temp01);
		mParticleSystem->SetStartScale(ChangeToFloat(Temp01));
	}

	Check = StartRotation_Check.GetCheck();
	if (Check == true)
	{
		StartRotation_max.GetWindowTextW(Temp01);
		StartRotation_min.GetWindowTextW(Temp02);
		mParticleSystem->SetStartRotation(ChangeToFloat(Temp01), ChangeToFloat(Temp02));
	}
	else
	{
		StartRotation.GetWindowTextW(Temp01);
		mParticleSystem->SetStartRotation(ChangeToFloat(Temp01));
	}


	Check = LifeForce_Check.GetCheck();
	if (Check == true)
	{
		LifeForce_X.GetWindowTextW(Temp01);
		LifeForce_Y.GetWindowTextW(Temp02);
		LifeForce_Z.GetWindowTextW(Temp03);
		Vector3 Start = Vector3(ChangeToFloat(Temp01), ChangeToFloat(Temp02), ChangeToFloat(Temp03));
		LifeForce_X_max.GetWindowTextW(Temp01);
		LifeForce_Y_max.GetWindowTextW(Temp02);
		LifeForce_Z_max.GetWindowTextW(Temp03);
		Vector3 End = Vector3(ChangeToFloat(Temp01), ChangeToFloat(Temp02), ChangeToFloat(Temp03));
		mParticleSystem->SetLifeTimeForce(Start, End);
	}
	else
	{
		LifeForce_X.GetWindowTextW(Temp01);
		LifeForce_Y.GetWindowTextW(Temp02);
		LifeForce_Z.GetWindowTextW(Temp03);
		Vector3 Start = Vector3(ChangeToFloat(Temp01), ChangeToFloat(Temp02), ChangeToFloat(Temp03));
		mParticleSystem->SetLifeTimeForce(Start);
	}

	Check = LifeRotation_Check.GetCheck();
	if (Check == true)
	{
		LifeRotation_min.GetWindowTextW(Temp01);
		LifeRotation_max.GetWindowTextW(Temp02);
		mParticleSystem->SetLifeTimeRotation(ChangeToFloat(Temp01), ChangeToFloat(Temp02));
	}
	else
	{
		LifeRotation.GetWindowTextW(Temp01);
		mParticleSystem->SetLifeTimeRotation(ChangeToFloat(Temp01));
	}


	int Choice = LifeScale_Combo.GetCurSel();
	LifeScale_min.GetWindowTextW(Temp01);
	LifeScale_max.GetWindowTextW(Temp02);
	switch (Choice)
	{
	case 0:
		mParticleSystem->SetLifeTimeScale(ChangeToFloat(Temp01), ChangeToFloat(Temp02),PARTICLE_LIFETIME_OPTION::NONE);
		break;
	case 1:
		mParticleSystem->SetLifeTimeScale(ChangeToFloat(Temp01), ChangeToFloat(Temp02), PARTICLE_LIFETIME_OPTION::UP);
		break;
	case 2:
		mParticleSystem->SetLifeTimeScale(ChangeToFloat(Temp01), ChangeToFloat(Temp02), PARTICLE_LIFETIME_OPTION::DOWN);
		break;
	case 3:
		mParticleSystem->SetLifeTimeScale(ChangeToFloat(Temp01), ChangeToFloat(Temp02), PARTICLE_LIFETIME_OPTION::UPDOWN);
		break;
	}

	LifeColor_min_R.GetWindowTextW(Temp01);
	LifeColor_min_G.GetWindowTextW(Temp02);
	LifeColor_min_B.GetWindowTextW(Temp03);
	LifeColor_min_A.GetWindowTextW(Temp04);
	Vector4 Start = Vector4(ChangeToFloat(Temp01), ChangeToFloat(Temp02), ChangeToFloat(Temp03), ChangeToFloat(Temp04));

	LifeColor_max_R.GetWindowTextW(Temp01);
	LifeColor_max_G.GetWindowTextW(Temp02);
	LifeColor_max_B.GetWindowTextW(Temp03);
	LifeColor_max_A.GetWindowTextW(Temp04);
	Vector4 End = Vector4(ChangeToFloat(Temp01), ChangeToFloat(Temp02), ChangeToFloat(Temp03), ChangeToFloat(Temp04));

	int Choice02 = LifeColor_Combo.GetCurSel();
	switch (Choice02)
	{
	case 0:
		mParticleSystem->SetLifeTimeColor(Start, End, PARTICLE_LIFETIME_OPTION::NONE);
		break;
	case 1:
		mParticleSystem->SetLifeTimeColor(Start, End, PARTICLE_LIFETIME_OPTION::UP);
		break;
	case 2:
		mParticleSystem->SetLifeTimeColor(Start, End, PARTICLE_LIFETIME_OPTION::DOWN);
		break;
	case 3:
		mParticleSystem->SetLifeTimeColor(Start, End, PARTICLE_LIFETIME_OPTION::UPDOWN);
		break;
	}
	
	// 변한 데이터 업데이트..
	mParticleSystem->DataUpdate();
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


void CTAP_Particle::OnShapeRadius_Button()
{
	int Check = ShapeRadius_Check.GetCheck();
	if (Check == 0)
	{
		//false
		ShapeRadius_Edit.ShowWindow(SW_SHOW);
		ShapeRadius_X_Edit.ShowWindow(SW_HIDE);
		ShapeRadius_Y_Edit.ShowWindow(SW_HIDE);
		ShapeRadius_Z_Edit.ShowWindow(SW_HIDE);
	}
	else
	{
		//true
		ShapeRadius_Edit.ShowWindow(SW_HIDE);
		ShapeRadius_X_Edit.ShowWindow(SW_SHOW);
		ShapeRadius_Y_Edit.ShowWindow(SW_SHOW);
		ShapeRadius_Z_Edit.ShowWindow(SW_SHOW);
	}
}


void CTAP_Particle::OnStartForce_Button()
{
	int Check = StartForce_Check.GetCheck();
	if (Check == 0)
	{
		StartForce_X_R.ShowWindow(SW_HIDE);
		StartForce_Y_R.ShowWindow(SW_HIDE);
		StartForce_Z_R.ShowWindow(SW_HIDE);
	}
	else
	{
		StartForce_X_R.ShowWindow(SW_SHOW);
		StartForce_Y_R.ShowWindow(SW_SHOW);
		StartForce_Z_R.ShowWindow(SW_SHOW);
	}
}


void CTAP_Particle::OnStartColor_Button()
{
	int Check = StartColor_Check.GetCheck();
	if (Check == 0)
	{
		StartColor_max_R.ShowWindow(SW_HIDE);
		StartColor_max_G.ShowWindow(SW_HIDE);
		StartColor_max_B.ShowWindow(SW_HIDE);
		StartColor_max_A.ShowWindow(SW_HIDE);
	}
	else
	{
		StartColor_max_R.ShowWindow(SW_SHOW);
		StartColor_max_G.ShowWindow(SW_SHOW);
		StartColor_max_B.ShowWindow(SW_SHOW);
		StartColor_max_A.ShowWindow(SW_SHOW);
	}
}

void CTAP_Particle::OnStartLifeTime_Check()
{
	int Check = StartLife_Check.GetCheck();
	if (Check == 0)
	{
		StartLifeTime_min.ShowWindow(SW_HIDE);
		StartLifeTime_max.ShowWindow(SW_HIDE);
		StartLifeTime.ShowWindow(SW_SHOW);
	}
	else
	{
		StartLifeTime.ShowWindow(SW_HIDE);
		StartLifeTime_min.ShowWindow(SW_SHOW);
		StartLifeTime_max.ShowWindow(SW_SHOW);
	}
}


void CTAP_Particle::OnStartScale_Button()
{
	int Check = StartScale_Check.GetCheck();
	if (Check == 0)
	{
		StartScale_min.ShowWindow(SW_HIDE);
		StartScale_max.ShowWindow(SW_HIDE);
		StartScale.ShowWindow(SW_SHOW);
	}
	else
	{
		StartScale_min.ShowWindow(SW_SHOW);
		StartScale_max.ShowWindow(SW_SHOW);
		StartScale.ShowWindow(SW_HIDE);
	}
}


void CTAP_Particle::OnStartRotationCheck()
{
	int Check = StartRotation_Check.GetCheck();
	if (Check == true)
	{
		StartRotation_min.ShowWindow(SW_SHOW);
		StartRotation_max.ShowWindow(SW_SHOW);
		StartRotation.ShowWindow(SW_HIDE);
	}
	else
	{
		StartRotation_min.ShowWindow(SW_HIDE);
		StartRotation_max.ShowWindow(SW_HIDE);
		StartRotation.ShowWindow(SW_SHOW);
	}
}


void CTAP_Particle::OnLifeForceCheck()
{
	int Check = LifeForce_Check.GetCheck();
	if (Check == true)
	{
		LifeForce_X_max.ShowWindow(SW_SHOW);
		LifeForce_Y_max.ShowWindow(SW_SHOW);
		LifeForce_Z_max.ShowWindow(SW_SHOW);
		
	}
	else
	{
		LifeForce_X_max.ShowWindow(SW_HIDE);
		LifeForce_Y_max.ShowWindow(SW_HIDE);
		LifeForce_Z_max.ShowWindow(SW_HIDE);
	}
}


void CTAP_Particle::OnBnClickedCheck8()
{
	int Check = LifeRotation_Check.GetCheck();
	if (Check == true)
	{
		LifeRotation_min.ShowWindow(SW_SHOW);
		LifeRotation_max.ShowWindow(SW_SHOW);
		LifeRotation.ShowWindow(SW_HIDE);
	}
	else
	{
		LifeRotation_min.ShowWindow(SW_HIDE);
		LifeRotation_max.ShowWindow(SW_HIDE);
		LifeRotation.ShowWindow(SW_SHOW);
	}
}
