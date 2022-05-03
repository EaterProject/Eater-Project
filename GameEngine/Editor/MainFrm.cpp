
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


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
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
	m_RightView	= nullptr;
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
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));


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

	int Right_pane		= m_wndSplitter[0].IdFromRowCol(0, 0);
	int Left_pane		= m_wndSplitter[0].IdFromRowCol(0, 1);
	
	if (m_wndSplitter[1].CreateStatic(&m_wndSplitter[0], 2, 1, WS_CHILD | WS_VISIBLE , Right_pane) == false)
	{
		MessageBox(_T("윈도우 생성실패"));
		return false;
	}
	
	int ClientSizeX = (int)GetSystemMetrics(SM_CXSCREEN);
	int ClientSizeY = (int)GetSystemMetrics(SM_CYSCREEN);
	int RenderSize	= ClientSizeX -750;
	
	if (m_wndSplitter[1].CreateView(0, 0, RUNTIME_CLASS(RenderView), CSize(0, ClientSizeY - 550), pContext) == false)
	{
		MessageBox(_T("윈도우 VIEW 생성실패"));
		return false;
	}
	if (m_wndSplitter[1].CreateView(1, 0, RUNTIME_CLASS(AssetView), CSize(0,0), pContext) == false)
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
	m_AssetView  = static_cast<AssetView*>(m_wndSplitter[0].GetPane(0, 1));
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
	if( !CFrameWnd::PreCreateWindow(cs) )
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

	Loading * mLoading = new Loading();
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


		int TextureCount	= GetLoadTextureCount();
		int ModelCount		= GetLoadMeshCount();
		int AnimationCount	= GetLoadAnimationCount();
		int MaterialCount	= GetLoadMaterialCount();
		int MeshBufferCount = GetLoadBufferCount();
		int AllAssetsCount  = 0;

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



void CMainFrame::OnClose()
{
	DialogFactory::GetFactory()->Release();
	CFrameWnd::OnClose();
}
