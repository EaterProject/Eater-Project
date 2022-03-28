// FileOption.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "FileOption.h"
#include "afxdialogex.h"
#include "EditorData.h"
#include "EditorManager.h"
#include "RightOption.h"
#include "GrobalFunction.h"
#include "Demo.h"
#include "CamAnimation.h"
#include "SceneSaveDialog.h"

#include "GameObject.h"
#include "MeshFilter.h"
#include "Light.h"
#include "Collider.h"
#include "Rigidbody.h"

// FileOption 대화 상자

IMPLEMENT_DYNAMIC(FileOption, CDialogEx)

FileOption::FileOption(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BUTTON_OPTION, pParent)
{

}

FileOption::~FileOption()
{

}

BOOL FileOption::OnInitDialog()
{
	CDialog::OnInitDialog();
	return 0;
}

void FileOption::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, AddComponent_ObjectName_Edit);
}

BEGIN_MESSAGE_MAP(FileOption, CDialogEx)
	ON_WM_TIMER()
	ON_MESSAGE(M_MSG_FILE_CHANGE, &FileOption::OnUserFunc)
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON1, &FileOption::OnCreateTerrain)
	ON_BN_CLICKED(IDC_BUTTON2, &FileOption::OnCreateLight)
	ON_BN_CLICKED(IDC_BUTTON3, &FileOption::OnCreateMaterial)
	ON_BN_CLICKED(IDC_BUTTON4, &FileOption::OnCreateParticle)
	ON_BN_CLICKED(IDC_BUTTON11, &FileOption::OnSceneSave)
	ON_BN_CLICKED(IDC_BUTTON12, &FileOption::OnOpenAssetsFolder)
	ON_BN_CLICKED(IDC_BUTTON27, &FileOption::OnOpenExe)
	ON_BN_CLICKED(IDC_BUTTON26, &FileOption::OnCreateCamera)
	ON_BN_CLICKED(IDC_BUTTON9, &FileOption::OnCreateGameObject)
	ON_BN_CLICKED(IDC_BUTTON6, &FileOption::OnAddCollider)
	ON_BN_CLICKED(IDC_BUTTON8, &FileOption::OnAddRigidbody)
	ON_BN_CLICKED(IDC_BUTTON16, &FileOption::OnAddLight)
END_MESSAGE_MAP()


void FileOption::Initialize(RightOption* mOption)
{
	mRightOption = mOption;
	mScene = new SceneSaveDialog();
	mScene->Initialize(mRightOption);
}

void FileOption::SetChoiceGameObjectName(std::string Name, GameObject* Obj)
{
	ChoiceObject = Obj;
	AddComponent_ObjectName_Edit.SetWindowTextW(ChangeToCString(Name));
}

void FileOption::OnTimer(UINT_PTR nIDEvent)
{

	CDialogEx::OnTimer(nIDEvent);
}


LRESULT FileOption::OnUserFunc(WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}

void FileOption::OnCreateTerrain()
{
	GameObject* Object = Demo::Create_Terrain("");
}

void FileOption::OnCreateLight()
{
	GameObject* Object = Demo::Create_Light();
	HTREEITEM Top = mRightOption->HirearchyTree.InsertItem(ChangeToCString(Object->Name));
	mRightOption->Create_Hirearchy_Item(Object, Top);
}

void FileOption::OnCreateMaterial()
{

}

void FileOption::OnCreateParticle()
{
	GameObject* Object = Demo::Create_Particle();
	HTREEITEM Top = mRightOption->HirearchyTree.InsertItem(ChangeToCString(Object->Name));
	mRightOption->Create_Hirearchy_Item(Object, Top);
}


void FileOption::OnSceneSave()
{
	mScene->DoModal();
	std::string SaveName = ChangeToString(mScene->Name);
	std::string SavePath = "../Assets/Scene/";
	Demo::SaveScene(SavePath, SaveName);
	AfxMessageBox(L"저장 완료");
}


void FileOption::OnOpenAssetsFolder()
{
	//에셋폴더를 연다
	TCHAR chFilePath[256] = { 0, };
	GetModuleFileName(NULL, chFilePath, 256);

	CString strFolderPath(chFilePath);
	for (int i = 0; i < 2; i++)
	{
		strFolderPath = strFolderPath.Left(strFolderPath.ReverseFind('\\'));
	}
	strFolderPath += _T("\\Assets");
	ShellExecute(NULL, _T("open"), _T("explorer"), strFolderPath, NULL, SW_SHOW);
}


void FileOption::OnOpenExe()
{
	//exe 파일을 실행시킨다
	wchar_t path[256] = { 0 };
	GetModuleFileName(NULL, path, 256);

	USES_CONVERSION;
	std::string str = W2A(path);
	std::size_t Start = 0;
	std::size_t End = str.rfind('\\');
	str = str.substr(Start, End);
	str += "\\GameClient.exe";
	CString FileName;
	FileName = str.c_str();

	STARTUPINFO Startupinfo = { 0 };
	PROCESS_INFORMATION processInfo;
	Startupinfo.cb = sizeof(STARTUPINFO);
	::CreateProcess
	(
		FileName,
		NULL, NULL, NULL,
		false, 0, NULL, NULL,
		&Startupinfo, &processInfo
	);
}

BOOL FileOption::PreTranslateMessage(MSG* pMsg)
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


void FileOption::OnCreateCamera()
{
	GameObject* Cam = Demo::Create_Camera();
	HTREEITEM Top = mRightOption->HirearchyTree.InsertItem(ChangeToCString(Cam->Name));
	mRightOption->Create_Hirearchy_Item(Cam, Top);
}

void FileOption::OnCreateGameObject()
{
	GameObject* Obj = Demo::Create_GameObject();
	HTREEITEM Top = mRightOption->HirearchyTree.InsertItem(ChangeToCString(Obj->Name));
	mRightOption->Create_Hirearchy_Item(Obj, Top);
}

void FileOption::OnAddCollider()
{
	if (ChoiceObject != nullptr)
	{
		ChoiceObject->AddComponent<Collider>();
	}
}

void FileOption::OnAddRigidbody()
{
	if (ChoiceObject != nullptr)
	{
		ChoiceObject->AddComponent<Rigidbody>();
	}
}

void FileOption::OnAddLight()
{
	if (ChoiceObject != nullptr)
	{
		ChoiceObject->AddComponent<Light>();
	}
}
