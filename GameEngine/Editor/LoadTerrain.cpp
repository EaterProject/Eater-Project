// LoadTerrain.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "LoadTerrain.h"
#include "afxdialogex.h"
#include "GrobalFunction.h"
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
	

	return 0;
}

void LoadTerrain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, FBXName_Edit);
	DDX_Control(pDX, IDC_EDIT3, Mask01Name_Edit);
	DDX_Control(pDX, IDC_EDIT4, Mask02Name_Edit);
}


BEGIN_MESSAGE_MAP(LoadTerrain, CDialogEx)
	ON_BN_CLICKED(IDOK, &LoadTerrain::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &LoadTerrain::OnBnClickedCancel)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// LoadTerrain 메시지 처리기


void LoadTerrain::OnBnClickedOk()
{
	if (ModelName == "")
	{
		MessageBox(L"모델이 없습니다");
		return;
	}

	if (Mask01Name == "")
	{
		MessageBox(L"Mask 01 없습니다");
		return;
	}

	if (Mask02Name == "")
	{
		MessageBox(L"Mask 02 없습니다");
		return;
	}


	Demo::Create_Terrain(ModelPath, Mask01Name, Mask02Name);

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
	

	for (int i = 0; i < 3; i++)
	{
		if (EditRect[i].left	<= Mouse.x &&
			EditRect[i].right	>= Mouse.x &&
			EditRect[i].top		<= Mouse.y &&
			EditRect[i].bottom	>= Mouse.y)
		{
			switch (i)
			{
			case 0:
				isModel = true;
				break;
			case 1:
				isMask01 = true;
				break;
			case 2:
				isMask02 = true;
				break;
			}
		}
	}
	


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
		//MeshName += MeshType;

		if (MeshType == ".fbx")
		{
			if (isModel == true)
			{
				ModelName = MeshName;
				ModelPath = Name;
				FBXName_Edit.SetWindowTextW(ChangeToCString(ModelName));
			}
			else
			{
				MessageBox(L"FBX 파일은 fbx Edit 에 넣어주세요");
			}
		}

		if(MeshType == ".png")
		{
			if(isMask01 == true)
			{
				MeshName += MeshType;
				Mask01Name = MeshName;
				Mask01Name_Edit.SetWindowTextW(ChangeToCString(Mask01Name));
			}
			else if (isMask02 == true)
			{
				MeshName += MeshType;
				Mask02Name = MeshName;
				Mask02Name_Edit.SetWindowTextW(ChangeToCString(Mask02Name));
			}
			else
			{
				MessageBox(L"png 파일은 Mask Edit 에 넣어주세요");
			}
		}
	}
	CDialogEx::OnDropFiles(hDropInfo);
}
