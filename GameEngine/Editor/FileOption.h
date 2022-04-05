#pragma once


// FileOption 대화 상자
#include <string>
#include "EditorData.h"
#include "GameObject.h"
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
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	RightOption*		mRightOption;
	SceneSaveDialog*	mScene;
	DECLARE_MESSAGE_MAP()
public:
	void Initialize(RightOption* mOption);
	void SetChoiceGameObjectName(std::string Name, GameObject* Obj);
public:
	CEdit AddComponent_ObjectName_Edit;
	GameObject* ChoiceObject;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnUserFunc(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCreateTerrain();
	afx_msg void OnCreateLight();
	afx_msg void OnCreateParticle();
	afx_msg void OnSceneSave();
	afx_msg void OnOpenAssetsFolder();
	afx_msg void OnOpenExe();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCreateCamera();
	afx_msg void OnCreateGameObject();
	afx_msg void OnAddCollider();
	afx_msg void OnAddRigidbody();
	afx_msg void OnAddLight();
	afx_msg void OnCreateMaterial();
};
