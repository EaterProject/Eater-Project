// CreateMaterial.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "CreateMaterial.h"
#include "afxdialogex.h"
#include "GrobalFunction.h"
#include "EngineData.h"
#include "Material.h"
#include "RightOption.h"
#include "EditorData.h"
#include "EditorManager.h"
// CreateMaterial 대화 상자

IMPLEMENT_DYNAMIC(CreateMaterial, CDialogEx)

CreateMaterial::CreateMaterial(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CREATE_MATERIAL, pParent)
{
	
}

CreateMaterial::~CreateMaterial()
{

}

BOOL CreateMaterial::OnInitDialog()
{
	CDialog::OnInitDialog();
	Emissive_Slider.SetRange(0, 100);
	Roughnees_Slider.SetRange(0, 100);
	Matallic_Slider.SetRange(0, 100);

	Emissive_Edit.SetWindowTextW(L"");
	Roughness_Edit.SetWindowTextW(L"");
	Matallic_Edit.SetWindowTextW(L"");

	AddColor_R.SetWindowTextW(L"255");
	AddColor_G.SetWindowTextW(L"255");
	AddColor_B.SetWindowTextW(L"255");
	Reset();


	return 0;
}

void CreateMaterial::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT3, Diffuse_Edit);
	DDX_Control(pDX, IDC_EDIT4, Nomal_Eidt);
	DDX_Control(pDX, IDC_EDIT5, EmissiveName_Edit);
	DDX_Control(pDX, IDC_EDIT6, ORM_Edit);
	DDX_Control(pDX, IDC_SLIDER4, Emissive_Slider);
	DDX_Control(pDX, IDC_SLIDER3, Roughnees_Slider);
	DDX_Control(pDX, IDC_SLIDER6, Matallic_Slider);
	DDX_Control(pDX, IDC_EDIT2, Emissive_Edit);
	DDX_Control(pDX, IDC_EDIT7, Roughness_Edit);
	DDX_Control(pDX, IDC_EDIT8, Matallic_Edit);
	DDX_Control(pDX, IDC_SLIDER7, Add_R_Slider);
	DDX_Control(pDX, IDC_SLIDER8, Add_G_Slider);
	DDX_Control(pDX, IDC_SLIDER9, Add_B_Slider);
	DDX_Control(pDX, IDC_EDIT21, AddColor_R);
	DDX_Control(pDX, IDC_EDIT22, AddColor_G);
	DDX_Control(pDX, IDC_EDIT23, AddColor_B);
	DDX_Control(pDX, IDC_EDIT1, MaterialName_Edit);
}

void CreateMaterial::Reset()
{
	MaterialName_Edit.SetWindowTextW(L"NO");

	Diffuse_Edit.SetWindowTextW(L"NO");
	Nomal_Eidt.SetWindowTextW(L"NO");
	ORM_Edit.SetWindowTextW(L"NO");
	EmissiveName_Edit.SetWindowTextW(L"NO");

	Emissive_Slider.SetRange(0, 100);
	Matallic_Slider.SetRange(0, 200);
	Roughnees_Slider.SetRange(0, 200);

	Emissive_Slider.SetPos((int)(0));
	Roughnees_Slider.SetPos((int)(100));
	Matallic_Slider.SetPos((int)(100));

	Add_R_Slider.SetRange(0, 255);
	Add_G_Slider.SetRange(0, 255);
	Add_B_Slider.SetRange(0, 255);
	Add_R_Slider.SetPos((int)(0));
	Add_G_Slider.SetPos((int)(0));
	Add_B_Slider.SetPos((int)(0));

	Emissive_Edit.SetWindowTextW(L"0");
	Roughness_Edit.SetWindowTextW(L"0");
	Matallic_Edit.SetWindowTextW(L"0");
	AddColor_R.SetWindowTextW(L"0");
	AddColor_G.SetWindowTextW(L"0");
	AddColor_B.SetWindowTextW(L"0");
}

void CreateMaterial::CheckTexture(POINT point)
{

}


BEGIN_MESSAGE_MAP(CreateMaterial, CDialogEx)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDOK, &CreateMaterial::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CreateMaterial::OnBnClickedCancel)
END_MESSAGE_MAP()


// CreateMaterial 메시지 처리기


void CreateMaterial::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//if (mMaterial == nullptr) { return; }

	if (pScrollBar->GetDlgCtrlID() == Emissive_Slider.GetDlgCtrlID())
	{
		float Emissive = (float)Emissive_Slider.GetPos();
		Emissive *= 0.1f;

		Emissive_Edit.SetWindowTextW(ChangeToCString(Emissive));
	}

	if (pScrollBar->GetDlgCtrlID() == Roughnees_Slider.GetDlgCtrlID())
	{
		float Roughnees = (float)Roughnees_Slider.GetPos();
		Roughnees -= 100;
		Roughnees *= 0.01f;
		Roughness_Edit.SetWindowTextW(ChangeToCString(Roughnees));
	}

	if (pScrollBar->GetDlgCtrlID() == Matallic_Slider.GetDlgCtrlID())
	{
		float Matallic = (float)Matallic_Slider.GetPos();
		Matallic -= 100;
		Matallic *= 0.01f;
		Matallic_Edit.SetWindowTextW(ChangeToCString(Matallic));
	}

	if (pScrollBar->GetDlgCtrlID() == Add_R_Slider.GetDlgCtrlID())
	{
		int AddColor = Add_R_Slider.GetPos();
		AddColor_R.SetWindowTextW(ChangeToCString(AddColor / 255.0f));
	}

	if (pScrollBar->GetDlgCtrlID() == Add_G_Slider.GetDlgCtrlID())
	{
		int AddColor = Add_G_Slider.GetPos();
		AddColor_G.SetWindowTextW(ChangeToCString(AddColor / 255.0f));
	}

	if (pScrollBar->GetDlgCtrlID() == Add_B_Slider.GetDlgCtrlID())
	{
		int AddColor = Add_B_Slider.GetPos();
		AddColor_B.SetWindowTextW(ChangeToCString(AddColor / 255.0f));
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CreateMaterial::OnBnClickedOk()
{
	CString Name;
	MaterialName_Edit.GetWindowTextW(Name);
	if (Name == L"" || Name == L"NO")
	{
		MessageBox(L"이름을 입력해야 합니다");
		return;
	}
	InstanceMaterial m;
	CString temp;
	m.Name = ChangeToString(Name);
	m.Alpha = false;
	Diffuse_Edit.GetWindowTextW(temp);
	m.DiffuseMap = ChangeToString(temp);

	Nomal_Eidt.GetWindowTextW(temp);
	m.NormalMap = ChangeToString(temp);

	Emissive_Edit.GetWindowTextW(temp);
	m.EmissiveMap = ChangeToString(temp);

	ORM_Edit.GetWindowTextW(temp);
	m.ORMMap = ChangeToString(temp);


	int E = Emissive_Slider.GetPos();
	int R = Roughnees_Slider.GetPos();
	int M = Matallic_Slider.GetPos();

	float ColorR = (float)Add_R_Slider.GetPos()/ 255.0f;
	float ColorG = (float)Add_G_Slider.GetPos()/ 255.0f;
	float ColorB = (float)Add_B_Slider.GetPos()/ 255.0f;
	m.Emissive = (E - 100) *0.01f;
	m.Roughness = (R - 100) * 0.01f;
	m.Metallic = (M - 100) * 0.01f;

	m.AddColorR = ColorR;
	m.AddColorG = ColorG;
	m.AddColorB = ColorB;



	RightOption::GetThis()->m_EditorManager->CreateMaterialData(&m);

	CDialogEx::OnOK();
	Reset();
}


void CreateMaterial::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
	Reset();
}
