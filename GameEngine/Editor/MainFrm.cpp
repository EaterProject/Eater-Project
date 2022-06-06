
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "Editor.h"

#include "MainFrm.h"
#include "RenderView.h"
#include "EaterEngineAPI.h"
#include "OptionView.h"
#include "AssetView.h"
#include "Loading.h"
#include "EditorToolScene.h"
#include "DialogFactory.h"
#include "SceneSetting.h"
#include "CamAnimation.h"
#include "GameObject.h"
#include "SceneSaveDialog.h"
#include "SkySetting.h"
#include "EditorManager.h"
#include <string>
#include <filesystem>
#include <stdio.h>
#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_SCENE_SETTING, &CMainFrame::OnSceneSetting)
	ON_COMMAND(ID_32774, &CMainFrame::OnOpenCameraAnimation)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CREATEOBJECT_LIGHT, &CMainFrame::OnCreateobjectLight)
	ON_COMMAND(ID_CREATEOBJECT_CAMERA, &CMainFrame::OnCreateobjectCamera)
	ON_COMMAND(ID_CREATEOBJECT_PARTICLE, &CMainFrame::OnCreateobjectParticle)
	ON_COMMAND(ID_CREATEOBJECT_TERRAIN, &CMainFrame::OnCreateobjectTerrain)
	ON_COMMAND(ID_GAMEOBJECT_POINT, &CMainFrame::OnGameobjectPoint)
	ON_COMMAND(ID_GAMEOBJECT_SPHERE, &CMainFrame::OnGameobjectSphere)
	ON_COMMAND(ID_GAMEOBJECT_BOX, &CMainFrame::OnGameobjectBox)
	ON_COMMAND(ID_32784, &CMainFrame::OnPlayerGame)
	ON_COMMAND(ID_32785, &CMainFrame::OpenAssetsFile)
	ON_COMMAND(ID_32783, &CMainFrame::SceneSaveFile)
	ON_COMMAND(ID_32786, &CMainFrame::OnCreateBuildFile)
	ON_COMMAND(ID_32787, &CMainFrame::OnSkySetting)
	ON_COMMAND(ID_GAMEOBJECT_CYLINDER, &CMainFrame::OnGameobjectCylinder)
	ON_COMMAND(ID_GAMEOBJECT_GRID, &CMainFrame::OnGameobjectGrid)
	ON_COMMAND(ID_GAMEOBJECT_MONKEY, &CMainFrame::OnGameobjectMonkey)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸
CMainFrame::CMainFrame() noexcept
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	m_RenderView = nullptr;
	m_RightView = nullptr;
}

CMainFrame::~CMainFrame()
{

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));


	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	if (m_wndSplitter[0].CreateStatic(this, 1, 2) == false)
	{
		MessageBox(_T("윈도우 생성실패"));
		return false;
	}

	int Right_pane = m_wndSplitter[0].IdFromRowCol(0, 0);
	int Left_pane = m_wndSplitter[0].IdFromRowCol(0, 1);

	if (m_wndSplitter[1].CreateStatic(&m_wndSplitter[0], 2, 1, WS_CHILD | WS_VISIBLE, Right_pane) == false)
	{
		MessageBox(_T("윈도우 생성실패"));
		return false;
	}

	int ClientSizeX = (int)GetSystemMetrics(SM_CXSCREEN);
	int ClientSizeY = (int)GetSystemMetrics(SM_CYSCREEN);
	int RenderSize = ClientSizeX - 750;

	if (m_wndSplitter[1].CreateView(0, 0, RUNTIME_CLASS(RenderView), CSize(0, ClientSizeY - 550), pContext) == false)
	{
		MessageBox(_T("윈도우 VIEW 생성실패"));
		return false;
	}
	if (m_wndSplitter[1].CreateView(1, 0, RUNTIME_CLASS(AssetView), CSize(0, 0), pContext) == false)
	{
		MessageBox(_T("윈도우 VIEW 생성실패"));
		return false;
	}

	if (m_wndSplitter[0].CreateView(0, 1, RUNTIME_CLASS(OptionView), CSize(RenderSize, 0), pContext) == false)
	{
		MessageBox(_T("윈도우 VIEW 생성실패"));
		return false;
	}

	m_RenderView = static_cast<RenderView*>(m_wndSplitter[1].GetPane(0, 0));
	m_OptionView = static_cast<OptionView*>(m_wndSplitter[1].GetPane(1, 0));
	m_AssetView = static_cast<AssetView*>(m_wndSplitter[0].GetPane(0, 1));
	m_wndSplitter[0].SetColumnInfo(0, RenderSize, 10);


	mThread = AfxBeginThread(ThreadFunction, this);
	mThread->m_bAutoDelete = FALSE;
	if (mThread == NULL)
	{
		AfxMessageBox(L"쓰레드 오류");
	}


	return true;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
//  Window 클래스 또는 스타일을 수정합니다.
cs.cx = (int)GetSystemMetrics(SM_CXSCREEN);
cs.cy = (int)GetSystemMetrics(SM_CYSCREEN);
return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG
UINT CMainFrame::ThreadFunction(LPVOID _mothod)
{
	Sleep(1000);
	CMainFrame* Main = (CMainFrame*)_mothod;

	Loading* mLoading = new Loading();
	mLoading->Create(IDD_LOADING);
	mLoading->ShowWindow(SW_SHOW);
	//mLoading = DialogFactory::GetFactory()->GetLoading();




	CString TextureCount_Str;
	CString ModelCount_Str;
	CString AnimationCount_Str;
	CString MeshBufferCount_Str;
	CString MaterialCount_Str;
	CString AllAssetsCount_Str;


	//Main->mLoading->LoadFileList.InsertString(0, L"Texture Count..");
	//Main->mLoading->LoadFileList.InsertString(1, L"Model Count..");

	while (EditorToolScene::GetThreadLoading() == false)
	{
		Sleep(100);

		mLoading->UpdateWindow();
		mLoading->LoadFileList.DeleteString(0);
		mLoading->LoadFileList.DeleteString(0);
		mLoading->LoadFileList.DeleteString(0);
		mLoading->LoadFileList.DeleteString(0);
		mLoading->LoadFileList.DeleteString(0);


		int TextureCount = GetLoadTextureCount();
		int ModelCount = GetLoadMeshCount();
		int AnimationCount = GetLoadAnimationCount();
		int MaterialCount = GetLoadMaterialCount();
		int MeshBufferCount = GetLoadBufferCount();
		int AllAssetsCount = 0;

		if (TextureCount != 0 || ModelCount != 0)
		{
			mLoading->LoadingTypeEdit.SetWindowTextW(L"리소스 로드중...");
		}

		TextureCount_Str.Format(_T("Texture ... %d"), TextureCount);
		ModelCount_Str.Format(_T("Model  ... %d"), ModelCount);
		AnimationCount_Str.Format(_T("Animation ... %d"), AnimationCount);
		MaterialCount_Str.Format(_T("Material ... %d"), MaterialCount);
		MeshBufferCount_Str.Format(_T("Buffer ... %d"), MeshBufferCount);

		mLoading->LoadFileList.InsertString(0, TextureCount_Str);
		mLoading->LoadFileList.InsertString(1, ModelCount_Str);
		mLoading->LoadFileList.InsertString(2, AnimationCount_Str);
		mLoading->LoadFileList.InsertString(3, MeshBufferCount_Str);
		mLoading->LoadFileList.InsertString(4, MaterialCount_Str);

		AllAssetsCount_Str.Format(_T("Load Assets Count : %d"), TextureCount + ModelCount + AnimationCount + MaterialCount + MeshBufferCount);
		mLoading->AllAssetsCount.SetWindowTextW(AllAssetsCount_Str);
		mLoading->UpdateWindow();
	}

	mLoading->DestroyWindow();
	delete mLoading;
	mLoading = nullptr;

	if (::TerminateThread(Main->mThread->m_hThread, 1))
	{
		::CloseHandle(Main->mThread->m_hThread);
		Main->mThread = NULL;
	}

	return 0;
}

void CMainFrame::CopyAssets()
{
	
}

bool CMainFrame::CheckFolder(std::string& Path)
{
	//경로에서 .을 찾지못했다면 폴더
	std::size_t Chick = Path.rfind('.');
	int length = (int)Path.length();
	if (Chick < 2 || Chick == std::string::npos)
	{
		return true;
	}
	else
	{
		return false;
	}
}



void CMainFrame::CopyEditorFile(std::string& Path, std::string& CopyPath)
{


}



void CMainFrame::OnClose()
{
	DialogFactory::GetFactory()->Release();
	CFrameWnd::OnClose();
}

void CMainFrame::OnSceneSetting()
{
	CRect rectParent;
	CRect rect;
	
	((CMainFrame*)AfxGetMainWnd())->GetWindowRect(&rectParent);
	SceneSetting* mScene = DialogFactory::GetFactory()->GetSceneSetting();
	mScene->GetClientRect(rect);
	
	
	float PosX = rectParent.left + (rectParent.right - rectParent.left) / 2 - rect.Width() / 2;
	float PosY = rectParent.top + (rectParent.bottom - rectParent.top) / 2 - rect.Height() / 2;
	
	mScene->SetWindowPos(NULL, PosX, PosY, 0, 0, SWP_NOSIZE);
	mScene->ShowWindow(SW_SHOW);
	mScene->Setting();
}


void CMainFrame::OnOpenCameraAnimation()
{
	CamAnimation* mCam = DialogFactory::GetFactory()->GetCamAnimation();
	mCam->ShowWindow(SW_SHOW);
}


void CMainFrame::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	RightOption* mRight = DialogFactory::GetFactory()->GetRightOption();

	CRect rect;
	mRight->HirearchyTree.GetWindowRect(rect);

	if (rect.left <= point.x && rect.right >= point.x &&
		rect.top <= point.y && rect.bottom >= point.y)
	{
		CMenu popup;
		CMenu* pMenu;
		popup.LoadMenuW(IDR_CREATE_OBJECT);
		pMenu = popup.GetSubMenu(0);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN || TPM_RIGHTBUTTON, point.x, point.y, this);
	}

}

void CMainFrame::OnCreateobjectLight()
{
	RightOption* mRightOption = DialogFactory::GetFactory()->GetRightOption();
	GameObject* Obj = EditorToolScene::Create_Light();

	CString Data;
	Data = Obj->Name.c_str();
	HTREEITEM Top = mRightOption->HirearchyTree.InsertItem(Data);

	mRightOption->Create_Hirearchy_Item(Obj, Top);
}


void CMainFrame::OnCreateobjectCamera()
{
	RightOption* mRightOption = DialogFactory::GetFactory()->GetRightOption();
	GameObject* Obj = EditorToolScene::Create_Camera();

	CString Data;
	Data = Obj->Name.c_str();
	HTREEITEM Top = mRightOption->HirearchyTree.InsertItem(Data);

	mRightOption->Create_Hirearchy_Item(Obj, Top);
}


void CMainFrame::OnCreateobjectParticle()
{
	RightOption* mRightOption = DialogFactory::GetFactory()->GetRightOption();
	GameObject* Obj = EditorToolScene::Create_BaseParticle();

	CString Data;
	Data = Obj->Name.c_str();
	HTREEITEM Top = mRightOption->HirearchyTree.InsertItem(Data);

	mRightOption->Create_Hirearchy_Item(Obj, Top);
}


void CMainFrame::OnCreateobjectTerrain()
{
	RightOption* mRightOption = DialogFactory::GetFactory()->GetRightOption();
	GameObject* Obj = EditorToolScene::Create_Terrain("", "", "");
	HTREEITEM Top = mRightOption->HirearchyTree.InsertItem(L"Terrain");
}


void CMainFrame::OnGameobjectPoint()
{
	RightOption* mRightOption = DialogFactory::GetFactory()->GetRightOption();
	GameObject* Obj = EditorToolScene::Create_GameObject();

	CString Data;
	Data = Obj->Name.c_str();
	HTREEITEM Top = mRightOption->HirearchyTree.InsertItem(Data);

	mRightOption->Create_Hirearchy_Item(Obj, Top);
}


void CMainFrame::OnGameobjectSphere()
{
	RightOption* mRightOption = DialogFactory::GetFactory()->GetRightOption();
	GameObject* Object = EditorToolScene::Create_Sphere();
	CString Data;
	Data = Object->Name.c_str();
	HTREEITEM Top = mRightOption->HirearchyTree.InsertItem(Data);

	mRightOption->Create_Hirearchy_Item(Object, Top);
}


void CMainFrame::OnGameobjectBox()
{
	RightOption* mRightOption = DialogFactory::GetFactory()->GetRightOption();
	GameObject* Object = EditorToolScene::Create_Box();
	CString Data;
	Data = Object->Name.c_str();
	HTREEITEM Top = mRightOption->HirearchyTree.InsertItem(Data);

	mRightOption->Create_Hirearchy_Item(Object, Top);
}


void CMainFrame::OnPlayerGame()
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

void CMainFrame::OpenAssetsFile()
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


void CMainFrame::SceneSaveFile()
{
	SceneSaveDialog* mScene = DialogFactory::GetFactory()->GetSceneSaveDialog();
	mScene->DoModal();
	if (mScene->isOK == true)
	{
		CT2CA convertedString = mScene->Name;
		std::string SaveName = (std::string)convertedString;
		std::string SavePath = "../Assets/Scene/";
		EditorToolScene::SaveScene(SavePath, SaveName);
		AfxMessageBox(L"저장 완료");
	}
}


void CMainFrame::OnCreateBuildFile()
{
	BROWSEINFO BrInfo;
	TCHAR szBuffer[512];                                      // 경로저장 버퍼 

	::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 512);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.lpszTitle = _T("파일이 저장될 폴더를 선택하세요");
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);
	::SHGetPathFromIDList(pItemIdList, szBuffer);               // 파일경로 읽어오기

	// 경로를 가져와 사용할 경우, Edit Control 에 값 저장
	CString str;
	str.Format(_T("%s"), szBuffer);

	// 취소 버튼을 눌렀을 경우..
	if (str.IsEmpty()) return;

	SetDlgItemText(IDC_EDIT2, str);

	CT2CA convertedString = str;
	std::string FilePath = (std::string)convertedString;

	for (int i = 0; i < FilePath.size(); i++)
	{
		if (FilePath[i] == '\\')
		{
			FilePath[i] = '/';
		}
	}
	//저장할 경로 
	FilePath += "/Editor";
	////그아래 폴더 생성
	std::filesystem::create_directory(FilePath);
	std::string AssetsFilePath = FilePath + "/Assets";
	std::string ExeFilePath = FilePath + "/Exe";
	std::filesystem::create_directory(AssetsFilePath);
	std::filesystem::create_directory(ExeFilePath);
	


	std::string OriginalFilePath = "../Assets";
#ifdef _DEBUG
	std::string OriginalExePath = "../x64/Debug";
#else
	std::string OriginalExePath = "../x64/Release";
#endif

	std::filesystem::copy(OriginalFilePath, AssetsFilePath, std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
	std::filesystem::copy(OriginalExePath, ExeFilePath, std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);

	for (auto& path : std::filesystem::recursive_directory_iterator(ExeFilePath.c_str()))
	{
		auto exe = std::filesystem::path(path).extension();

		if (exe != ".dll" && exe != ".exe" && exe != "")
		{
			std::filesystem::remove(path);
		}
	}

	AfxMessageBox(L"빌드파일 생성완료");
}

void CMainFrame::OnSkySetting()
{
	SkySetting* mSky = DialogFactory::GetFactory()->GetSkySetting();
	mSky->ShowWindow(SW_SHOW);
}


void CMainFrame::OnGameobjectCylinder()
{
	RightOption* mRightOption = DialogFactory::GetFactory()->GetRightOption();
	GameObject* Object = EditorToolScene::Create_Cylinder();
	CString Data;
	Data = Object->Name.c_str();
	HTREEITEM Top = mRightOption->HirearchyTree.InsertItem(Data);

	mRightOption->Create_Hirearchy_Item(Object, Top);
}


void CMainFrame::OnGameobjectGrid()
{
	RightOption* mRightOption = DialogFactory::GetFactory()->GetRightOption();
	GameObject* Object = EditorToolScene::Create_Grid();
	CString Data;
	Data = Object->Name.c_str();
	HTREEITEM Top = mRightOption->HirearchyTree.InsertItem(Data);

	mRightOption->Create_Hirearchy_Item(Object, Top);
}


void CMainFrame::OnGameobjectMonkey()
{
	RightOption* mRightOption = DialogFactory::GetFactory()->GetRightOption();
	GameObject* Object = EditorToolScene::Create_MonKey();
	CString Data;
	Data = Object->Name.c_str();
	HTREEITEM Top = mRightOption->HirearchyTree.InsertItem(Data);

	mRightOption->Create_Hirearchy_Item(Object, Top);
}
