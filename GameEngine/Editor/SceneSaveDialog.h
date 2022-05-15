#pragma once
#include "CustomDialog.h"

// SceneSaveDialog 대화 상자
class RightOption;
class SceneSaveDialog : public CustomDialog
{
	DECLARE_DYNAMIC(SceneSaveDialog)

public:
	SceneSaveDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~SceneSaveDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCENE_SAVE };
#endif


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit SceneName_Edit;
	CString Name;
	bool isOK = false;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
