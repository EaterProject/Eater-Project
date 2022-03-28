// CRightView.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "RenderView.h"
#include "MainHeader.h"
#include "Demo.h"

// CRightView

IMPLEMENT_DYNCREATE(RenderView, CView)

RenderView::RenderView()
{

}

RenderView::~RenderView()
{

}

BEGIN_MESSAGE_MAP(RenderView, CView)
	ON_WM_SIZE()
	//	ON_WM_GETMINMAXINFO()
	//	ON_WM_SIZING()
	//ON_WM_MOVE()
	ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()


// CRightView 그리기

void RenderView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}

// CRightView 진단
#ifdef _DEBUG
void RenderView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void RenderView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif
#endif //_DEBUG


void RenderView::Update()
{
	if (GetFocus()->GetSafeHwnd() == hwnd)
	{
		WindowFocus(true);
	}
	else
	{
		WindowFocus(false);
	}

	UpdateEngine();
}

void RenderView::OnInitialUpdate()
{
	//컨퍼넌트 엔진을 초기화 시키고 씬을 적용시킨다
	hwnd = this->GetSafeHwnd();

	EngineInitialize(hwnd,false);
	EditorSetting();

	CreateScene<Demo>("Demo");
	ChoiceScene("Demo");
}

void RenderView::OnSize(UINT nType, int cx, int cy)
{
	OnReSize(cx, cy);
	CView::OnSize(nType, cx, cy);
}
