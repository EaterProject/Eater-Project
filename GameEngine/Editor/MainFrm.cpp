
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "Editor.h"

#include "MainFrm.h"
#include "RenderView.h"
#include "MainHeader.h"
#include "OptionView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
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
	if (m_wndSplitter.CreateStatic(this, 1, 2) == false)
	{
		MessageBox(_T("윈도우 생성실패"));
		return false;
	}

	int ClientSizeX = (int)GetSystemMetrics(SM_CXSCREEN);
	int ClientSizeY = (int)GetSystemMetrics(SM_CYSCREEN);
	int RenderSize	= ClientSizeX -750;

	if (m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(RenderView), CSize(RenderSize, 0), pContext) == false)
	{
		MessageBox(_T("윈도우 VIEW 생성실패"));
		return false;
	}

	if (m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(OptionView), CSize(0, 0), pContext) == false)
	{
		MessageBox(_T("윈도우 VIEW 생성실패"));
		return false;
	}


	m_RenderView = static_cast<RenderView*>(m_wndSplitter.GetPane(0, 0));
	m_OptionView = static_cast<OptionView*>(m_wndSplitter.GetPane(0, 1));

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


// CMainFrame 메시지 처리기

