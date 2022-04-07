// AssetView.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "AssetView.h"


// AssetView

IMPLEMENT_DYNCREATE(AssetView, CView)

AssetView::AssetView()
{

}

AssetView::~AssetView()
{
}

BEGIN_MESSAGE_MAP(AssetView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DROPFILES()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// AssetView 그리기
void AssetView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// AssetView 진단

#ifdef _DEBUG
void AssetView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void AssetView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif
#endif //_DEBUG

int AssetView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	mAssets.Create(IDD_ASSETS, this);
	mAssets.ShowWindow(SW_SHOW);
	return 0;
}
// AssetView 메시지 처리기


void AssetView::OnSize(UINT nType, int cx, int cy)
{
	mAssets.MoveWindow(0, 0, cx, cy);
	CView::OnSize(nType, cx, cy);
}


void AssetView::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnDropFiles(hDropInfo);
}


BOOL AssetView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (nFlags & (MK_SHIFT | MK_CONTROL))
	{
		return false;
	}

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
