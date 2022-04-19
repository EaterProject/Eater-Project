#pragma once


// CRightView 보기

class RenderView : public CView
{
	DECLARE_DYNCREATE(RenderView)

protected:
	RenderView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~RenderView();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
	void Update();
	void OnInitialUpdate();

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	int SizeX;
	int SizeY;
	HWND hwnd;
	afx_msg void OnClose();
	afx_msg void OnNcDestroy();
};


