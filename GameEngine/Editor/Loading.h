#pragma once


// Loading 대화 상자
#include <string>
#include "RightOption.h"
class Loading : public CDialogEx
{
	DECLARE_DYNAMIC(Loading)

public:
	Loading(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Loading();
	void SetData(int Min, int Max);
	void SetUpdate(int Number);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOADING };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	static void SetMessage(std::string Msg);
public:
	CProgressCtrl LoadingBar;
	int LoadingMax;
	static std::string mMsg;
	CEdit LoadingTypeEdit;
	CListBox LoadFileList;
	CEdit AllAssetsCount;
};
