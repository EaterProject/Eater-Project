#pragma once


// Loading 대화 상자
#include <string>
#include "RightOption.h"
#include "CustomDialog.h"
class Loading : public CustomDialog
{
	DECLARE_DYNAMIC(Loading)

public:
	Loading(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Loading();
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOADING };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit LoadingTypeEdit;
	CListBox LoadFileList;
	CEdit AllAssetsCount;
};
