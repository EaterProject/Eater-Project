// LoadTerrain.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "LoadTerrain.h"
#include "afxdialogex.h"
#include "GrobalFunction.h"
#include "RightOption.h"
#include "EditorManager.h"
#include "Demo.h"



// LoadTerrain 대화 상자

IMPLEMENT_DYNAMIC(LoadTerrain, CDialogEx)

LoadTerrain::LoadTerrain(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOAD_TERRAIN, pParent)
{

}

LoadTerrain::~LoadTerrain()
{
}

BOOL LoadTerrain::OnInitDialog()
{
	CDialog::OnInitDialog();
	Type_Combo.InsertString(0, L"Terrain");
	Type_Combo.InsertString(1, L"Collider");
	Type_Combo.InsertString(2, L"NevMesh");
	Type_Combo.SetCurSel(0);
	OnTerrain(true);
	FBXName_Edit.SetWindowTextW(L"FBX파일을 드래그");
	return 0;
}

void LoadTerrain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, FBXName_Edit);
	DDX_Control(pDX, IDC_EDIT3, Mask01Name_Edit);
	DDX_Control(pDX, IDC_EDIT4, Mask02Name_Edit);
	DDX_Control(pDX, IDC_EDIT5, NewFileName_Edit);
	DDX_Control(pDX, IDC_COMBO1, Type_Combo);
}


BEGIN_MESSAGE_MAP(LoadTerrain, CDialogEx)
	ON_BN_CLICKED(IDOK, &LoadTerrain::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &LoadTerrain::OnBnClickedCancel)
	ON_WM_DROPFILES()
	ON_CBN_SELCHANGE(IDC_COMBO1, &LoadTerrain::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// LoadTerrain 메시지 처리기


void LoadTerrain::OnTerrain(bool On)
{
	if (On == true)
	{
		Mask01Name_Edit.ShowWindow(SW_SHOW);
		Mask02Name_Edit.ShowWindow(SW_SHOW);
		Mask01Name_Edit.SetWindowTextW(L"Mask01_텍스쳐를 드래그 해주세요");
		Mask02Name_Edit.SetWindowTextW(L"Mask02_텍스쳐를 드래그 해주세요");
	}
	else
	{
		Mask01Name_Edit.ShowWindow(SW_HIDE);
		Mask02Name_Edit.ShowWindow(SW_HIDE);
	}
}

void LoadTerrain::OnNavMesh(bool On)
{

}

bool LoadTerrain::TerrainErrorChack()
{
	if (ModelName == "")
	{
		MessageBox(L"모델이 없습니다");
		return false;
	}

	if (Mask01Name == "")
	{
		MessageBox(L"Mask 01 없습니다");
		return false;
	}

	if (Mask02Name == "")
	{
		MessageBox(L"Mask 02 없습니다");
		return false;
	}
	return true;
}

std::string LoadTerrain::FileDropName(HDROP hDropInfo)
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
		return Name;
	}

}

std::string LoadTerrain::GetFileName(std::string filePath)
{
	std::size_t Start = filePath.rfind("/") + 1;
	std::size_t End = filePath.rfind(".") - Start;
	std::string MeshName = filePath.substr(Start, End);
	return MeshName;
}

std::string LoadTerrain::GetFileType(std::string filePath)
{

	std::size_t Start	= filePath.rfind(".");
	std::size_t End		= filePath.length() - Start;
	std::string MeshType = filePath.substr(Start, End);
	return MeshType;
}

bool LoadTerrain::GetDragRect(int EditIndex)
{
	POINT Mouse;
	::GetCursorPos(&Mouse);

	if (EditRect[EditIndex].left <= Mouse.x &&
		EditRect[EditIndex].right >= Mouse.x &&
		EditRect[EditIndex].top <= Mouse.y &&
		EditRect[EditIndex].bottom >= Mouse.y)
	{
		return true;
	}
	else
	{
		return false;
	}

	
}

void LoadTerrain::OnBnClickedOk()
{
	switch(Type_Combo.GetCurSel())
	{
	case 0: //Terrain
		if (TerrainErrorChack() == false) { return; }
		Demo::Create_Terrain(ModelPath,Mask01Name,Mask02Name);
		break;
	case 1: //Collider
		{
			CString Name;
			NewFileName_Edit.GetWindowTextW(Name);
			std::string ChangeName = ChangeToString(Name);
			if (ChangeName == "") { MessageBox(L"이름이 없습니다"); return;}
			RightOption::GetThis()->m_EditorManager->ConvertData(ModelPath, ChangeName, CHANGE_TYPE::COLLIDER);
		}
		break;
	case 2: //Nav매쉬
		{
			CString Name;
			NewFileName_Edit.GetWindowTextW(Name);
			std::string ChangeName = ChangeToString(Name);
			if (ChangeName == "") { MessageBox(L"이름이 없습니다"); return; }
			RightOption::GetThis()->m_EditorManager->ConvertData(ModelPath, ChangeName, CHANGE_TYPE::NAVMESH);
		}
		break;
	}


	CDialogEx::OnOK();
}


void LoadTerrain::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


void LoadTerrain::OnDropFiles(HDROP hDropInfo)
{
	bool isModel = false;
	bool isMask01 = false;
	bool isMask02 = false;


	POINT Mouse;
	::GetCursorPos(&Mouse);
	FBXName_Edit.GetWindowRect(&EditRect[ModelName_index]);
	Mask01Name_Edit.GetWindowRect(&EditRect[Mask01Name_index]);
	Mask02Name_Edit.GetWindowRect(&EditRect[Mask02Name_index]);
	

	
	std::string FileName = FileDropName(hDropInfo);
	std::string OriginFileName = GetFileName(FileName);
	std::string OriginFileType = GetFileType(FileName);
	//이름 찾기
	
	//터레인일때
	if (Type_Combo.GetCurSel() == 0)
	{
		if (GetDragRect(Mask01Name_index))
		{
			Mask01Name = FileName;
			Mask01Name_Edit.SetWindowTextW(ChangeToCString(Mask01Name));
		}
		else if (GetDragRect(Mask02Name_index))
		{
			Mask02Name = FileName;
			Mask02Name_Edit.SetWindowTextW(ChangeToCString(Mask02Name));
		}
	}
	
	if (GetDragRect(ModelName_index))
	{
		ModelName = OriginFileName;
		ModelPath = FileName;
		FBXName_Edit.SetWindowTextW(ChangeToCString(ModelName));
	}

	CDialogEx::OnDropFiles(hDropInfo);
}

void LoadTerrain::OnCbnSelchangeCombo1()
{
	int TypeData = Type_Combo.GetCurSel();
	switch (TypeData)
	{
	case 0:
		OnTerrain(true);
		break;
	case 1:
		OnTerrain(false);
		break;
	}
}
