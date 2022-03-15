// CTAP_MeshFilter.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "CTAP_MeshFilter.h"
#include "afxdialogex.h"
#include "MeshFilter.h"
#include "GrobalFunction.h"

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
	std::string MeshName = ObjectMeshFilter->GetMeshName();
	std::string Diffuse = ObjectMeshFilter->GetDiffuseTextureName();
	std::string Nomal	= ObjectMeshFilter->GetNormlaTextureName();
	std::string ORM		= ObjectMeshFilter->GetORMTextureName();
	
	MeshName_Edit.SetWindowTextW(ChangeToCString(MeshName));
	Diffuse_Edit.SetWindowTextW(ChangeToCString(Diffuse));
	Nomal_Eidt.SetWindowTextW(ChangeToCString(Nomal));
	ORM_Edit.SetWindowTextW(ChangeToCString(ORM));

	Diffuse_Edit.GetWindowRect(EditRect[Diffuse_Index]);
	MeshName_Edit.GetWindowRect(EditRect[MeshName_Index]);
	Nomal_Eidt.GetWindowRect(EditRect[Nomal_Index]);
	ORM_Edit.GetWindowRect(EditRect[ORM_Index]);

	mMeshFilter = ObjectMeshFilter;
}

void CTAP_MeshFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, Diffuse_Edit);
	DDX_Control(pDX, IDC_EDIT4, MeshName_Edit);
	DDX_Control(pDX, IDC_EDIT3, Nomal_Eidt);
	DDX_Control(pDX, IDC_EDIT5, ORM_Edit);
}

BOOL CTAP_MeshFilter::OnInitDialog()
{
	CDialog::OnInitDialog();
	//위치를 미리 시작할때 저장
	return 0;
}


BEGIN_MESSAGE_MAP(CTAP_MeshFilter, CDialogEx)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(M_MSG_MeshFilter, &CTAP_MeshFilter::OnUserFun)
	ON_BN_CLICKED(IDC_BUTTON1, &CTAP_MeshFilter::OnDiffuse_Button)
	ON_BN_CLICKED(IDC_BUTTON2, &CTAP_MeshFilter::OnNomal_Button)
	ON_BN_CLICKED(IDC_BUTTON10, &CTAP_MeshFilter::OnORM_Button)
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

	for (int i = 1; i <= 3; i++)
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
			return TRUE;
		default:
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
