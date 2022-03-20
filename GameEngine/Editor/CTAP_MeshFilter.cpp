// CTAP_MeshFilter.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "CTAP_MeshFilter.h"
#include "afxdialogex.h"
#include "MeshFilter.h"
#include "GrobalFunction.h"
#include "EngineData.h"
#include "Material.h"

// CTAP_MeshFilter 대화 상자

IMPLEMENT_DYNAMIC(CTAP_MeshFilter, CDialogEx)

CTAP_MeshFilter::CTAP_MeshFilter(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAP_MESHFILTER, pParent)
{

}

CTAP_MeshFilter::~CTAP_MeshFilter()
{

}

void CTAP_MeshFilter::SetGameObject(MeshFilter* ObjectMeshFilter)
{
	mMeshFilter = ObjectMeshFilter;
	mMaterial = ObjectMeshFilter->GetMaterial();

	std::string MeshName = ObjectMeshFilter->GetMeshName();
	
	MeshName_Edit.SetWindowTextW(ChangeToCString(MeshName));

	Diffuse_Edit.GetWindowRect(EditRect[Diffuse_Index]);
	MeshName_Edit.GetWindowRect(EditRect[MeshName_Index]);
	Nomal_Eidt.GetWindowRect(EditRect[Nomal_Index]);
	ORM_Edit.GetWindowRect(EditRect[ORM_Index]);
	EmissiveName_Edit.GetWindowRect(EditRect[Emissive_Index]);

	if (mMaterial != nullptr)
	{
		std::string Diffuse		= mMaterial->GetDiffuseName();
		std::string Nomal		= mMaterial->GetNormalName();
		std::string Emissive	= mMaterial->GetEmissiveName();
		std::string ORM			= mMaterial->GetORMName();

		Diffuse_Edit.SetWindowTextW(ChangeToCString(Diffuse));
		Nomal_Eidt.SetWindowTextW(ChangeToCString(Nomal));
		ORM_Edit.SetWindowTextW(ChangeToCString(ORM));
		EmissiveName_Edit.SetWindowTextW(ChangeToCString(Emissive));

		float EmissiveF = mMaterial->m_MaterialData->Material_SubData->EmissiveFactor;
		float RoughnessF = mMaterial->m_MaterialData->Material_SubData->RoughnessFactor;
		float MetallicF = mMaterial->m_MaterialData->Material_SubData->MetallicFactor;

		float AddColorR = mMaterial->m_MaterialData->Material_SubData->AddColor.x;
		float AddColorG = mMaterial->m_MaterialData->Material_SubData->AddColor.y;
		float AddColorB = mMaterial->m_MaterialData->Material_SubData->AddColor.z;
		float AddColorA = mMaterial->m_MaterialData->Material_SubData->AddColor.w;

		float BaseColorR = mMaterial->m_MaterialData->Material_SubData->BaseColor.x;
		float BaseColorG = mMaterial->m_MaterialData->Material_SubData->BaseColor.y;
		float BaseColorB = mMaterial->m_MaterialData->Material_SubData->BaseColor.z;
		float BaseColorA = mMaterial->m_MaterialData->Material_SubData->BaseColor.w;

		BaseColor_R.SetWindowTextW(ChangeToCString(BaseColorR));
		BaseColor_G.SetWindowTextW(ChangeToCString(BaseColorG));
		BaseColor_B.SetWindowTextW(ChangeToCString(BaseColorB));

		AddColor_R.SetWindowTextW(ChangeToCString(BaseColorR));
		AddColor_G.SetWindowTextW(ChangeToCString(BaseColorG));
		AddColor_B.SetWindowTextW(ChangeToCString(BaseColorB));

		Emissive_Edit.SetWindowTextW(ChangeToCString(EmissiveF));
		Roughness_Edit.SetWindowTextW(ChangeToCString(RoughnessF));
		Matallic_Edit.SetWindowTextW(ChangeToCString(MetallicF));

		Emissive_Slider.SetPos(EmissiveF);
		Roughnees_Slider.SetPos(RoughnessF);
		Matallic_Slider.SetPos(MetallicF);

		Base_R_Slider.SetRange(0, 255);
		Base_G_Slider.SetRange(0, 255);
		Base_B_Slider.SetRange(0, 255);

		Add_R_Slider.SetRange(0, 255);
		Add_G_Slider.SetRange(0, 255);
		Add_B_Slider.SetRange(0, 255);
	}
}

void CTAP_MeshFilter::UpdateGameObject()
{
	CString Name00;
	CString Name01;
	CString Name02;
	Emissive_Edit.SetWindowTextW(Name00);
	Roughness_Edit.SetWindowTextW(Name01);
	Matallic_Edit.SetWindowTextW(Name02);

	if (Name00 != "")
	{
		mMaterial->m_MaterialData->Material_SubData->EmissiveFactor = ChangeToFloat(Name00);
	}

	if (Name01 != "")
	{
		mMaterial->m_MaterialData->Material_SubData->RoughnessFactor = ChangeToFloat(Name01);
	}

	if (Name02 != "")
	{
		mMaterial->m_MaterialData->Material_SubData->MetallicFactor = ChangeToFloat(Name02);
	}

	CString GetNumber;
	Vector4 BaseColor;
	Vector4 AddColor;
	BaseColor_R.GetWindowTextW(GetNumber);
	BaseColor.x = ChangeToFloat(GetNumber);

	BaseColor_G.GetWindowTextW(GetNumber);
	BaseColor.y = ChangeToFloat(GetNumber);

	BaseColor_B.GetWindowTextW(GetNumber);
	BaseColor.z = ChangeToFloat(GetNumber);

	AddColor_R.GetWindowTextW(GetNumber);
	AddColor.x = ChangeToFloat(GetNumber);

	AddColor_G.GetWindowTextW(GetNumber);
	AddColor.y = ChangeToFloat(GetNumber);

	AddColor_B.GetWindowTextW(GetNumber);
	AddColor.z = ChangeToFloat(GetNumber);


	if (mMaterial != nullptr)
	{
		mMaterial->SetAddColor(AddColor);
		mMaterial->SetBaseColor(BaseColor);
	}
}

void CTAP_MeshFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, Diffuse_Edit);
	DDX_Control(pDX, IDC_EDIT4, MeshName_Edit);
	DDX_Control(pDX, IDC_EDIT3, Nomal_Eidt);
	DDX_Control(pDX, IDC_EDIT5, ORM_Edit);
	DDX_Control(pDX, IDC_SLIDER1, Emissive_Slider);
	DDX_Control(pDX, IDC_SLIDER2, Roughnees_Slider);
	DDX_Control(pDX, IDC_SLIDER3, Matallic_Slider);
	DDX_Control(pDX, IDC_EDIT2, Emissive_Edit);
	DDX_Control(pDX, IDC_EDIT7, Roughness_Edit);
	DDX_Control(pDX, IDC_EDIT8, Matallic_Edit);
	DDX_Control(pDX, IDC_EDIT9, EmissiveName_Edit);
	DDX_Control(pDX, IDC_EDIT15, BaseColor_R);
	DDX_Control(pDX, IDC_EDIT16, BaseColor_G);
	DDX_Control(pDX, IDC_EDIT17, BaseColor_B);
	DDX_Control(pDX, IDC_EDIT21, AddColor_R);
	DDX_Control(pDX, IDC_EDIT22, AddColor_G);
	DDX_Control(pDX, IDC_EDIT23, AddColor_B);
	DDX_Control(pDX, IDC_SLIDER4, Base_R_Slider);
	DDX_Control(pDX, IDC_SLIDER5, Base_G_Slider);
	DDX_Control(pDX, IDC_SLIDER6, Base_B_Slider);
	DDX_Control(pDX, IDC_SLIDER7, Add_R_Slider);
	DDX_Control(pDX, IDC_SLIDER8, Add_G_Slider);
	DDX_Control(pDX, IDC_SLIDER9, Add_B_Slider);
}

BOOL CTAP_MeshFilter::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	Emissive_Slider.SetRange(0, 100);
	Roughnees_Slider.SetRange(0, 100);
	Matallic_Slider.SetRange(0, 100);

	Emissive_Edit.SetWindowTextW(L"");
	Roughness_Edit.SetWindowTextW(L"");
	Matallic_Edit.SetWindowTextW(L"");

	BaseColor_R.SetWindowTextW(L"255");
	BaseColor_G.SetWindowTextW(L"255");
	BaseColor_B.SetWindowTextW(L"255");

	AddColor_R.SetWindowTextW(L"255");
	AddColor_G.SetWindowTextW(L"255");
	AddColor_B.SetWindowTextW(L"255");

	return 0;
}


BEGIN_MESSAGE_MAP(CTAP_MeshFilter, CDialogEx)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(M_MSG_MeshFilter, &CTAP_MeshFilter::OnUserFun)
	ON_BN_CLICKED(IDC_BUTTON1, &CTAP_MeshFilter::OnDiffuse_Button)
	ON_BN_CLICKED(IDC_BUTTON2, &CTAP_MeshFilter::OnNomal_Button)
	ON_BN_CLICKED(IDC_BUTTON10, &CTAP_MeshFilter::OnORM_Button)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON26, &CTAP_MeshFilter::OnEmissive_Button)
END_MESSAGE_MAP()

LRESULT CTAP_MeshFilter::OnUserFun(WPARAM wParam, LPARAM lparam)
{
	CString* pstrString = (CString*)lparam;
	CString strString = pstrString->GetString();
	std::string FileName = ChangeToString(strString);
	FileName = FileName.substr(0, FileName.rfind('.'));

	int Type = GetFileNameType(strString);

	POINT point;
	GetCursorPos(&point);

	if (EditRect[MeshName_Index].left	<= point.x &&
		EditRect[MeshName_Index].right	>= point.x &&
		EditRect[MeshName_Index].top	<= point.y &&
		EditRect[MeshName_Index].bottom >= point.y)
	{
		if (Type == FBX || Type == EATER)
		{
			MeshName_Edit.SetWindowTextW(strString);
			mMeshFilter->SetMeshName(FileName);
		}
		else
		{
			AfxMessageBox(L"Error : FBX , EATER 파일만 가능합니다");
		}
	}

	for (int i = 1; i <= 4; i++)
	{
		if (EditRect[i].left	<= point.x &&
			EditRect[i].right	>= point.x &&
			EditRect[i].top		<= point.y &&
			EditRect[i].bottom	>= point.y)
		{
			if (Type == PNG || Type == DDS)
			{
				switch (i)
				{
				case 1:
					Diffuse_Edit.SetWindowTextW(strString);
					mMeshFilter->SetDiffuseTextureName(FileName);
					break;
				case 2:
					Nomal_Eidt.SetWindowTextW(strString);
					mMeshFilter->SetNormalTextureName(FileName);
					break;
				case 3:
					ORM_Edit.SetWindowTextW(strString);
					mMeshFilter->SetORMTextureName(FileName);
					break;
				case 4:
					EmissiveName_Edit.SetWindowTextW(strString);
					mMeshFilter->SetEmissiveTextureName(FileName);
					break;
				}
			}
			else
			{
				AfxMessageBox(L"Error : PNG , DDS 파일만 가능합니다");
			}
		}
	}
	return LRESULT();
}

void CTAP_MeshFilter::OnDiffuse_Button()
{
	Diffuse_Edit.SetWindowTextW(L"");
	mMeshFilter->SetDiffuseTextureName("");
}

void CTAP_MeshFilter::OnNomal_Button()
{
	Nomal_Eidt.SetWindowTextW(L"");
	mMeshFilter->SetNormalTextureName("");
}

void CTAP_MeshFilter::OnEmissive_Button()
{
	EmissiveName_Edit.SetWindowTextW(L"");
	mMeshFilter->SetEmissiveTextureName("");
}
void CTAP_MeshFilter::OnORM_Button()
{
	ORM_Edit.SetWindowTextW(L"");
	mMeshFilter->SetORMTextureName("");
}


BOOL CTAP_MeshFilter::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:
			UpdateGameObject();
			return TRUE;
		default:
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CTAP_MeshFilter::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	mMaterial = mMeshFilter->GetMaterial();
	if (mMaterial == nullptr) { return; }

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (pScrollBar->GetDlgCtrlID() == Emissive_Slider.GetDlgCtrlID())
	{
		int Emissive = Emissive_Slider.GetPos();
		Emissive_Edit.SetWindowTextW(ChangeToCString(Emissive));
		mMaterial->SetEmissiveFactor(Emissive * 0.1);
	}

	if (pScrollBar->GetDlgCtrlID() == Roughnees_Slider.GetDlgCtrlID())
	{
		int Roughnees = Roughnees_Slider.GetPos();
		Roughness_Edit.SetWindowTextW(ChangeToCString(Roughnees));
		mMaterial->SetRoughnessFactor(Roughnees * 0.01);
	}

	if (pScrollBar->GetDlgCtrlID() == Matallic_Slider.GetDlgCtrlID())
	{
		int Matallic = Matallic_Slider.GetPos();
		Matallic_Edit.SetWindowTextW(ChangeToCString(Matallic));
		mMaterial->SetMetallicFactor(Matallic * 0.01);
	}

	if (pScrollBar->GetDlgCtrlID() == Base_R_Slider.GetDlgCtrlID())
	{
		int Base_R = Base_R_Slider.GetPos();
		mMaterial->m_MaterialData->Material_SubData->BaseColor.x = Base_R / 255.0f;
		BaseColor_R.SetWindowTextW(ChangeToCString(Base_R / 255.0f));
	}

	if (pScrollBar->GetDlgCtrlID() == Base_G_Slider.GetDlgCtrlID())
	{
		int Base_G = Base_G_Slider.GetPos();
		mMaterial->m_MaterialData->Material_SubData->BaseColor.y = Base_G / 255.0f;
		BaseColor_G.SetWindowTextW(ChangeToCString(Base_G / 255.0f));
	}

	if (pScrollBar->GetDlgCtrlID() == Base_B_Slider.GetDlgCtrlID())
	{
		int Base_B = Base_B_Slider.GetPos();
		mMaterial->m_MaterialData->Material_SubData->BaseColor.z = Base_B / 255.0f;
		BaseColor_B.SetWindowTextW(ChangeToCString(Base_B / 255.0f));
	}


	if (pScrollBar->GetDlgCtrlID() == Add_R_Slider.GetDlgCtrlID())
	{
		int AddColor = Add_R_Slider.GetPos();
		mMaterial->m_MaterialData->Material_SubData->AddColor.x = AddColor / 255.0f;
		AddColor_R.SetWindowTextW(ChangeToCString(AddColor / 255.0f));
	}

	if (pScrollBar->GetDlgCtrlID() == Add_G_Slider.GetDlgCtrlID())
	{
		int AddColor = Add_G_Slider.GetPos();
		mMaterial->m_MaterialData->Material_SubData->AddColor.y = AddColor / 255.0f;
		AddColor_G.SetWindowTextW(ChangeToCString(AddColor / 255.0f));
	}

	if (pScrollBar->GetDlgCtrlID() == Add_B_Slider.GetDlgCtrlID())
	{
		int AddColor = Add_B_Slider.GetPos();
		mMaterial->m_MaterialData->Material_SubData->AddColor.z = AddColor / 255.0f;
		AddColor_B.SetWindowTextW(ChangeToCString(AddColor / 255.0f));
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


