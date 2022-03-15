// OptionView.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "OptionView.h"
// OptionView

IMPLEMENT_DYNCREATE(OptionView, CView)

OptionView::OptionView()
{

}

OptionView::~OptionView()
{
}

BEGIN_MESSAGE_MAP(OptionView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void OptionView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}
// OptionView 진단
#ifdef _DEBUG
void OptionView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void OptionView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// OptionView 메시지 처리기
int OptionView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	mOption.Create(IDD_RIGHT_OPTION, this);
	mOption.ShowWindow(SW_SHOW);

	mLoading.Create(IDD_LOADING, this);
	mLoading.ShowWindow(SW_SHOW);

	mOption.mLoading = &mLoading;
	mLoading.mOption = &mOption;

	mLoading.SetData(0, mOption.LoadMaxCount);
	return 0;
}

void OptionView::OnSize(UINT nType, int cx, int cy)
{
	//int Offset = 100;
	if (mLoading.IsWindowVisible() == true)
	{
		//부모의 클라이언트 사이즈 구하기
		CRect rect;
		this->GetParent()->GetClientRect(rect);
		int ParectSizeX = rect.Width() / 2;
		int ParectSizeY = rect.Height() / 2;

		//나의 클라이언트 사이즈 구하기
		this->GetClientRect(rect);
		int ChildSizeX = rect.Width() / 2;
		int ChildSizeY = rect.Height() / 2;


		mLoading.MoveWindow(ParectSizeX - ChildSizeX, ParectSizeY, cx, 150);
		mOption.MoveWindow(0, 0, cx, cy);
	}
	CView::OnSize(nType, cx, cy);
}
