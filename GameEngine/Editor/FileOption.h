#pragma once


// FileOption 대화 상자
#include <string>
#include "EditorData.h"
class RightOption;
class EditorManager;
class SceneSaveDialog;
class FileOption : public CDialogEx
{
	DECLARE_DYNAMIC(FileOption)

public:
	FileOption(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~FileOption();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BUTTON_OPTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	RightOption* mRightOption;
	SceneSaveDialog* mScene;
	DECLARE_MESSAGE_MAP()
public:
	void Initialize(RightOption* mOption);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnUserFunc(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCreateTerrain();
	afx_msg void OnCreateLight();
	afx_msg void OnCreateMaterial();
	afx_msg void OnCreateParticle();
	afx_msg void OnSceneSave();
	afx_msg void OnOpenAssetsFolder();
	afx_msg void OnOpenExe();
};
