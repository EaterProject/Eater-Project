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
#include "EditorToolScene.h"
#include "CamAnimation.h"
#include "SceneSaveDialog.h"
#include "CreateMaterial.h"
#include "MainHeader.h"
#include "TypeOptionHeader.h"

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
	DDX_Control(pDX, IDC_CHECK1, Debug_Check);
	DDX_Control(pDX, IDC_CHECK3, SSAO_Check);
	DDX_Control(pDX, IDC_CHECK4, FOG_Check);
	DDX_Control(pDX, IDC_CHECK2, Shadow_Check);
	DDX_Control(pDX, IDC_CHECK9, IBL_Check);
	DDX_Control(pDX, IDC_CHECK5, Bloom_Check);
	DDX_Control(pDX, IDC_CHECK10, FXAA_Check);
	DDX_Control(pDX, IDC_CHECK6, HDR_Check);
}

BEGIN_MESSAGE_MAP(FileOption, CDialogEx)
	ON_WM_TIMER()
	ON_MESSAGE(M_MSG_FILE_CHANGE, &FileOption::OnUserFunc)
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON1, &FileOption::OnCreateTerrain)
	ON_BN_CLICKED(IDC_BUTTON2, &FileOption::OnCreateLight)
	ON_BN_CLICKED(IDC_BUTTON4, &FileOption::OnCreateParticle)
	ON_BN_CLICKED(IDC_BUTTON11, &FileOption::OnSceneSave)
	ON_BN_CLICKED(IDC_BUTTON12, &FileOption::OnOpenAssetsFolder)
	ON_BN_CLICKED(IDC_BUTTON27, &FileOption::OnOpenExe)
	ON_BN_CLICKED(IDC_BUTTON26, &FileOption::OnCreateCamera)
	ON_BN_CLICKED(IDC_BUTTON9, &FileOption::OnCreateGameObject)
	ON_BN_CLICKED(IDC_BUTTON6, &FileOption::OnAddCollider)
	ON_BN_CLICKED(IDC_BUTTON8, &FileOption::OnAddRigidbody)
	ON_BN_CLICKED(IDC_BUTTON16, &FileOption::OnAddLight)
	ON_BN_CLICKED(IDC_BUTTON7, &FileOption::OnCreateMaterial)
	ON_BN_CLICKED(IDC_CHECK1, &FileOption::OnDebugButton)
	ON_BN_CLICKED(IDC_CHECK6, &FileOption::OnHDR_Button)
	ON_BN_CLICKED(IDC_CHECK3, &FileOption::OnSSAO_Button)
	ON_BN_CLICKED(IDC_CHECK4, &FileOption::OnFOG_Button)
	ON_BN_CLICKED(IDC_CHECK2, &FileOption::OnShadow_Button)
	ON_BN_CLICKED(IDC_CHECK9, &FileOption::OnIBL_Button)
	ON_BN_CLICKED(IDC_CHECK5, &FileOption::OnBloom_Button)
	ON_BN_CLICKED(IDC_CHECK10, &FileOption::OnFXAA_Button)
END_MESSAGE_MAP()


void FileOption::Initialize(RightOption* mOption)
{
	mRightOption = mOption;
	mScene = new SceneSaveDialog();
	mScene->Initialize(mRightOption);

	mMaterial = new CreateMaterial();
	mMaterial->Create(IDD_CREATE_MATERIAL);
	mMaterial->ShowWindow(SW_HIDE);

	//mRenderOption = GetRenderOptionData();
	//mRenderOption->DebugOption ^= DEBUG_EDITOR;
	//
	//mRenderOption->RenderingOption ^= RENDER_DEBUG;
	//mRenderOption->RenderingOption ^= RENDER_SHADOW;
	//mRenderOption->RenderingOption ^= RENDER_SSAO;
	//mRenderOption->RenderingOption ^= RENDER_IBL;
	//
	//mRenderOption->PostProcessOption ^= RENDER_FOG;
	//mRenderOption->PostProcessOption ^= RENDER_BLOOM;
	//mRenderOption->PostProcessOption ^= RENDER_HDR;
	//mRenderOption->PostProcessOption ^= RENDER_FXAA;

	Debug_Check.SetCheck(true);
	SSAO_Check.SetCheck(true);
	FOG_Check.SetCheck(true);
	Shadow_Check.SetCheck(true);
	IBL_Check.SetCheck(true);
	Bloom_Check.SetCheck(true);
	FXAA_Check.SetCheck(true);
	HDR_Check.SetCheck(true);

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
	EditorToolScene::Create_Terrain("","","");
	mRightOption->HirearchyTree.InsertItem(L"Terrain");
}

void FileOption::OnCreateLight()
{
	GameObject* Object = EditorToolScene::Create_Light();
	HTREEITEM Top = mRightOption->HirearchyTree.InsertItem(ChangeToCString(Object->Name));
	mRightOption->Create_Hirearchy_Item(Object, Top);
}


void FileOption::OnCreateParticle()
{
	GameObject* Object = EditorToolScene::Create_Particle();
	HTREEITEM Top = mRightOption->HirearchyTree.InsertItem(ChangeToCString(Object->Name));
	mRightOption->Create_Hirearchy_Item(Object, Top);
}


void FileOption::OnSceneSave()
{
	mScene->DoModal();
	if(mScene->isOK == true)
	{
		std::string SaveName = ChangeToString(mScene->Name);
		std::string SavePath = "../Assets/Scene/";
		EditorToolScene::SaveScene(SavePath, SaveName);
		AfxMessageBox(L"저장 완료");
	}
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
	GameObject* Cam = EditorToolScene::Create_Camera();
	HTREEITEM Top = mRightOption->HirearchyTree.InsertItem(ChangeToCString(Cam->Name));
	mRightOption->Create_Hirearchy_Item(Cam, Top);
}

void FileOption::OnCreateGameObject()
{
	GameObject* Obj = EditorToolScene::Create_GameObject();
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


void FileOption::OnCreateMaterial()
{
	mMaterial->ShowWindow(SW_SHOW);

	RECT MyDig;
	RECT Temp;
	this->GetWindowRect(&MyDig);

	Temp= MyDig;
	Temp.left = MyDig.right;
	mMaterial->SetWindowPos(NULL, Temp.left, Temp.top, Temp.right, Temp.bottom, SWP_NOSIZE);
	//mLoadNavMesh->MoveWindow(&NavMeshWindow);
}


void FileOption::OnDebugButton()
{
	RenderOption* Option = GetRenderOptionData();
	Option->DebugOption ^= RENDER_DEBUG;
	RenderSetting();
}


void FileOption::OnHDR_Button()
{
	RenderOption* Option = GetRenderOptionData();
	Option->PostProcessOption ^= POSTPROCESS_OPTION::RENDER_HDR;
	RenderSetting();
}


void FileOption::OnSSAO_Button()
{
	RenderOption* Option = GetRenderOptionData();
	Option->RenderingOption ^= RENDER_OPTION::RENDER_SSAO;
	RenderSetting();
}


void FileOption::OnFOG_Button()
{
	RenderOption* Option = GetRenderOptionData();
	Option->PostProcessOption ^= POSTPROCESS_OPTION::RENDER_FOG;
	RenderSetting();
}


void FileOption::OnShadow_Button()
{
	RenderOption* Option = GetRenderOptionData();
	Option->RenderingOption ^= RENDER_OPTION::RENDER_SHADOW;
	RenderSetting();
}


void FileOption::OnIBL_Button()
{
	RenderOption* Option = GetRenderOptionData();
	Option->RenderingOption ^= RENDER_OPTION::RENDER_IBL;
	RenderSetting();
}


void FileOption::OnBloom_Button()
{
	RenderOption* Option = GetRenderOptionData();
	Option->PostProcessOption ^= POSTPROCESS_OPTION::RENDER_BLOOM;
	RenderSetting();
}


void FileOption::OnFXAA_Button()
{
	RenderOption* Option = GetRenderOptionData();
	Option->PostProcessOption ^= POSTPROCESS_OPTION::RENDER_FXAA;
	RenderSetting();
}
