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
	
	std::string MeshName = ObjectMeshFilter->GetBufferName();
	std::string ModelName = ObjectMeshFilter->GetModelName();
	
	MeshName_Edit.SetWindowTextW(ChangeToCString(MeshName));
	ModelName_Edit.SetWindowTextW(ChangeToCString(ModelName));

	Diffuse_Edit.GetWindowRect(EditRect[Diffuse_Index]);
	MeshName_Edit.GetWindowRect(EditRect[MeshName_Index]);
	ModelName_Edit.GetWindowRect(EditRect[ModelName_Index]);
	Nomal_Eidt.GetWindowRect(EditRect[Nomal_Index]);
	ORM_Edit.GetWindowRect(EditRect[ORM_Index]);
	MaterialName_Edit.GetWindowRect(EditRect[Material_Index]);
	EmissiveName_Edit.GetWindowRect(EditRect[Emissive_Index]);

	if (mMaterial != nullptr)
	{
		std::string Diffuse		= mMaterial->GetDiffuseName();
		std::string Nomal		= mMaterial->GetNormalName();
		std::string Emissive	= mMaterial->GetEmissiveName();
		std::string ORM			= mMaterial->GetORMName();

		MaterialName_Edit.SetWindowTextW(ChangeToCString(mMaterial->Name));
		Diffuse_Edit.SetWindowTextW(ChangeToCString(Diffuse));
		Nomal_Eidt.SetWindowTextW(ChangeToCString(Nomal));
		ORM_Edit.SetWindowTextW(ChangeToCString(ORM));
		EmissiveName_Edit.SetWindowTextW(ChangeToCString(Emissive));

		float EmissiveF		= mMaterial->m_MaterialData->Material_SubData->EmissiveFactor;
		float RoughnessF	= mMaterial->m_MaterialData->Material_SubData->RoughnessFactor;
		float MetallicF		= mMaterial->m_MaterialData->Material_SubData->MetallicFactor;

		float AddColorR = mMaterial->m_MaterialData->Material_SubData->AddColor.x;
		float AddColorG = mMaterial->m_MaterialData->Material_SubData->AddColor.y;
		float AddColorB = mMaterial->m_MaterialData->Material_SubData->AddColor.z;

	
		Emissive_Edit.SetWindowTextW(ChangeToCString(EmissiveF));
		Roughness_Edit.SetWindowTextW(ChangeToCString(RoughnessF));
		Matallic_Edit.SetWindowTextW(ChangeToCString(MetallicF));

		Emissive_Slider.SetPos((int)(EmissiveF*10.0f));
		Roughnees_Slider.SetPos((int)(RoughnessF* 100.0f + 100.0f));
		Matallic_Slider.SetPos((int)(MetallicF*100.0f + 100.0f));

		Emissive_Slider.SetRange(0, 100);
		Matallic_Slider.SetRange(0, 200);
		Roughnees_Slider.SetRange(0, 200);

		Vector4 Add = mMaterial->m_MaterialData->Material_SubData->AddColor;
	
		Add_R_Slider.SetRange(0, 255);
		Add_G_Slider.SetRange(0, 255);
		Add_B_Slider.SetRange(0, 255);
		Add_R_Slider.SetPos((int)(Add.x * 255.0f));
		Add_G_Slider.SetPos((int)(Add.y * 255.0f));
		Add_B_Slider.SetPos((int)(Add.z * 255.0f));

		AddColor_R.SetWindowTextW(ChangeToCString(AddColorR));
		AddColor_G.SetWindowTextW(ChangeToCString(AddColorG));
		AddColor_B.SetWindowTextW(ChangeToCString(AddColorB));
	}
	else
	{
		Add_R_Slider.SetRange(0, 255);
		Add_G_Slider.SetRange(0, 255);
		Add_B_Slider.SetRange(0, 255);
		Add_R_Slider.SetPos(255);
		Add_G_Slider.SetPos(255);
		Add_B_Slider.SetPos(255);


		Emissive_Slider.SetRange(0, 100);
		Matallic_Slider.SetRange(0, 200);
		Roughnees_Slider.SetRange(0, 200);

		Emissive_Slider.SetPos(10);
		Roughnees_Slider.SetPos(100);
		Matallic_Slider.SetPos(100);
	}
}

void CTAP_MeshFilter::GetData(ObjectOption& Option)
{
	CString Diffuse;
	CString Normal;
	CString Emissive;
	CString ORM;
	Diffuse_Edit.GetWindowTextW(Diffuse);
	Nomal_Eidt.GetWindowTextW(Normal);
	EmissiveName_Edit.GetWindowTextW(Emissive);
	ORM_Edit.GetWindowTextW(ORM);

	Option.Diffuse	= ChangeToString(Diffuse);
	Option.Normal	= ChangeToString(Normal);
	Option.Emissive = ChangeToString(Emissive);
	Option.ORM		= ChangeToString(ORM);

	CString Roughness;
	Roughness_Edit.GetWindowTextW(Roughness);
	Option.Roughness = ChangeToFloat(Roughness);

	CString Metallic;
	Matallic_Edit.GetWindowTextW(Metallic);
	Option.Metallic = ChangeToFloat(Metallic);

	CString Add_R;
	CString Add_G;
	CString Add_B;
	AddColor_R.GetWindowTextW(Add_R);
	AddColor_G.GetWindowTextW(Add_G);
	AddColor_B.GetWindowTextW(Add_B);

	Option.AddColor = { ChangeToFloat(Add_R),ChangeToFloat(Add_G) ,ChangeToFloat(Add_B) };
}

void CTAP_MeshFilter::UpdateGameObject()
{
	CString Name00;
	CString Name01;
	CString Name02;
	Emissive_Edit.GetWindowTextW(Name00);
	Roughness_Edit.GetWindowTextW(Name01);
	Matallic_Edit.GetWindowTextW(Name02);

	if (Name00 != "")
	{
		float Emissive = ChangeToFloat(Name00);
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
	Vector3 AddColor;

	AddColor_R.GetWindowTextW(GetNumber);
	AddColor.x = ChangeToFloat(GetNumber);

	AddColor_G.GetWindowTextW(GetNumber);
	AddColor.y = ChangeToFloat(GetNumber);

	AddColor_B.GetWindowTextW(GetNumber);
	AddColor.z = ChangeToFloat(GetNumber);


	if (mMaterial != nullptr)
	{
		mMaterial->SetAddColor(AddColor);
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
	DDX_Control(pDX, IDC_EDIT21, AddColor_R);
	DDX_Control(pDX, IDC_EDIT22, AddColor_G);
	DDX_Control(pDX, IDC_EDIT23, AddColor_B);
	DDX_Control(pDX, IDC_SLIDER7, Add_R_Slider);
	DDX_Control(pDX, IDC_SLIDER8, Add_G_Slider);
	DDX_Control(pDX, IDC_SLIDER9, Add_B_Slider);
	DDX_Control(pDX, IDC_EDIT15, ModelName_Edit);
	DDX_Control(pDX, IDC_EDIT16, MaterialName_Edit);
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

	AddColor_R.SetWindowTextW(L"255");
	AddColor_G.SetWindowTextW(L"255");
	AddColor_B.SetWindowTextW(L"255");

	return 0;
}

void CTAP_MeshFilter::SetSlider(CSliderCtrl& Silder,int Number)
{
	CString str;
	str.Format(_T("%d"), Number); //Format을 이용하여 int값을 변경
	Silder.SetWindowTextW(str);
	UpdateData(false); //set을 해주기 위해서 UpdateData(false); 를 사용

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
	ON_BN_CLICKED(IDC_BUTTON5, &CTAP_MeshFilter::OnMaterialName_Button)
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

	if (DropRect(EditRect[ModelName_Index]))
	{
		if (Type == EATER)
		{
			ModelName_Edit.SetWindowTextW(strString);
			mMeshFilter->SetModelName(FileName);
		}
		else
		{
			AfxMessageBox(L"Error : Eater 파일만 가능합니다");
		}
	}
	
	if (DropRect(EditRect[MeshName_Index]))
	{
		if (Type == EMESH)
		{
			MeshName_Edit.SetWindowTextW(strString);
			mMeshFilter->SetMeshName(FileName);
		}
		else
		{
			AfxMessageBox(L"Error : Emesh 파일만 가능합니다");
		}
	}
	
	if (DropRect(EditRect[Material_Index]))
	{
		if (Type == EMAT)
		{
			MaterialName_Edit.SetWindowTextW(strString);
			mMeshFilter->SetMaterialName(FileName);
		}
		else
		{
			AfxMessageBox(L"Error : Emat 파일만 가능합니다");
		}
	}
	
	
	for (int i = 0; i <= 3; i++)
	{
		if (DropRect(EditRect[i]))
		{
			if (Type == PNG || Type == DDS)
			{
				switch (i)
				{
				case 0:
					Diffuse_Edit.SetWindowTextW(strString);
					mMeshFilter->SetDiffuseTextureName(FileName);
					break;
				case 1:
					Nomal_Eidt.SetWindowTextW(strString);
					mMeshFilter->SetNormalTextureName(FileName);
					break;
				case 2:
					ORM_Edit.SetWindowTextW(strString);
					mMeshFilter->SetORMTextureName(FileName);
					break;
				case 3:
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
		float Emissive = (float)Emissive_Slider.GetPos();
		Emissive *= 0.1f;

		Emissive_Edit.SetWindowTextW(ChangeToCString(Emissive));
		mMaterial->SetEmissiveFactor(Emissive);
	}

	if (pScrollBar->GetDlgCtrlID() == Roughnees_Slider.GetDlgCtrlID())
	{
		float Roughnees = (float)Roughnees_Slider.GetPos();
		Roughnees -= 100;
		Roughnees *= 0.01f;
		Roughness_Edit.SetWindowTextW(ChangeToCString(Roughnees));
		mMaterial->SetRoughnessFactor(Roughnees);
	}

	if (pScrollBar->GetDlgCtrlID() == Matallic_Slider.GetDlgCtrlID())
	{
		float Matallic = (float)Matallic_Slider.GetPos();
		Matallic -= 100;
		Matallic *= 0.01f;
		Matallic_Edit.SetWindowTextW(ChangeToCString(Matallic));
		mMaterial->SetMetallicFactor(Matallic);
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

void CTAP_MeshFilter::OnMaterialName_Button()
{


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
