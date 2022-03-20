// OptionView.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "OptionView.h"
// OptionView

IMPLEMENT_DYNCREATE(OptionView, CScrollView)

OptionView::OptionView()
	:CScrollView()
{

}

OptionView::~OptionView()
{
}

BEGIN_MESSAGE_MAP(OptionView, CScrollView)
	ON_WM_CREATE()
	ON_WM_SIZE()
ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

void OptionView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	CRect rc;
	GetClientRect(&rc);
	CSize sizeViewPage;
	sizeViewPage.cx = rc.right - rc.left;
	sizeViewPage.cy = rc.bottom - rc.top;

	CSize sizeViewPageTotal;
	sizeViewPageTotal.cx = sizeViewPage.cx;
	sizeViewPageTotal.cy = sizeViewPage.cy; // 3페이지를 설정하고 싶으면 * 3을 하면 되긋지


	SetScrollSizes(MM_TEXT, sizeViewPageTotal);
}

void OptionView::DoDataExchange(CDataExchange* pDX)
{
	CScrollView::DoDataExchange(pDX);
	
}

void OptionView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}
// OptionView 진단
#ifdef _DEBUG
void OptionView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void OptionView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// OptionView 메시지 처리기
int OptionView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	mOption.Create(IDD_RIGHT_OPTION, this);
	mOption.ShowWindow(SW_SHOW);


	//mOption.mLoading = &mLoading;
	//mLoading.mOption = &mOption;
	
	//mLoading.SetData(0, mOption.LoadMaxCount);
	return 0;
}

void OptionView::OnSize(UINT nType, int cx, int cy)
{
	int Offset = 100;

	CRect rect;
	this->GetParent()->GetClientRect(rect);
	int ParectSizeX = rect.Width() / 2;
	int ParectSizeY = rect.Height() / 2;

	//나의 클라이언트 사이즈 구하기
	this->GetClientRect(rect);
	int ChildSizeX = rect.Width() / 2;
	int ChildSizeY = rect.Height() / 2;
	mOption.MoveWindow(0, 0, cx, 3000);

	CScrollView::OnSize(nType, cx, cy);






	//if (mLoading.IsWindowVisible() == true)
	//{
	//	//부모의 클라이언트 사이즈 구하기
	//	CRect rect;
	//	this->GetParent()->GetClientRect(rect);
	//	int ParectSizeX = rect.Width() / 2;
	//	int ParectSizeY = rect.Height() / 2;
	//
	//	//나의 클라이언트 사이즈 구하기
	//	this->GetClientRect(rect);
	//	int ChildSizeX = rect.Width() / 2;
	//	int ChildSizeY = rect.Height() / 2;
	//	mOption.MoveWindow(0, 0, cx, 3000);
	//
	//
	//	mLoading.MoveWindow(ParectSizeX - ChildSizeX, ParectSizeY, cx, 150);
	//}
}

BOOL OptionView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	
	return DoMouseWheel(nFlags, zDelta, pt);
}

