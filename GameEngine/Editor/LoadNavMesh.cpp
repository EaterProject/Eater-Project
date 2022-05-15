// LoadNavMesh.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "LoadNavMesh.h"
#include "afxdialogex.h"
#include "EditorToolScene.h"
#include "RightOption.h"
#include "EditorManager.h"


// LoadNavMesh 대화 상자

IMPLEMENT_DYNAMIC(LoadNavMesh, CustomDialog)

void LoadNavMesh::SetLoadPath(std::string&& Path)
{

}

void LoadNavMesh::SetPoint(POINT point)
{

}

LoadNavMesh::LoadNavMesh(CWnd* pParent /*=nullptr*/)
	: CustomDialog(IDD_LOAD_NAVMESH, pParent)
{

}

LoadNavMesh::~LoadNavMesh()
{
}

void LoadNavMesh::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, FIleName_Eidt);
	DDX_Control(pDX, IDC_EDIT3, NewFileName_Edit);
}


BEGIN_MESSAGE_MAP(LoadNavMesh, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDOK, &LoadNavMesh::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &LoadNavMesh::OnBnClickedCancel)
END_MESSAGE_MAP()


// LoadNavMesh 메시지 처리기


void LoadNavMesh::OnDropFiles(HDROP hDropInfo)
{
	TCHAR FileName[256] = { 0, };
	UINT count = DragQueryFile(hDropInfo, 0xFFFFFFFF, FileName, 256);
	for (UINT i = 0; i < count; i++)
	{
		DragQueryFile(hDropInfo, i, FileName, 256);

		CString Change = FileName;
		CT2CA convertedString(Change);
		std::string Name = convertedString;
		for (int i = 0; i < Name.size(); i++)
		{
			if (Name[i] == '\\')
			{
				Name[i] = '/';
			}
		}

		//이름 찾기
		std::size_t Start = Name.rfind("/") + 1;
		std::size_t End = Name.rfind(".") - Start;
		std::string MeshName = Name.substr(Start, End);
		std::string ChangeName = MeshName;
		//타입찾기
		Start = Name.rfind(".");
		End = Name.length() - Start;
		std::string MeshType = Name.substr(Start, End);

		FilePath = Name;
		FIleName_Eidt.SetWindowTextW(ChangeToCString(FilePath));
	}
	CDialogEx::OnDropFiles(hDropInfo);
}

void LoadNavMesh::OnBnClickedOk()
{
	if (FilePath == "")
	{
		MessageBox(L"로드할 파일이름이 없습니다");
	}

	CString NewFileName;
	NewFileName_Edit.GetWindowTextW(NewFileName);

	//RightOption::GetThis()->m_EditorManager->ConvertData(FilePath,ORIGIN_ONLY);
	
	FilePath = "";
	FIleName_Eidt.SetWindowTextW(L"");
	CDialogEx::OnOK();
}

void LoadNavMesh::OnBnClickedCancel()
{
	FilePath = "";
	FIleName_Eidt.SetWindowTextW(L"");
	CDialogEx::OnCancel();
}
