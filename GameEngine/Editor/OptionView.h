#pragma once


// OptionView 보기
#include "RightOption.h"
#include "Loading.h"
class OptionView : public CView
{
	DECLARE_DYNCREATE(OptionView)

protected:
	OptionView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~OptionView();

public:
	RightOption mOption;
	Loading		mLoading;

	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


