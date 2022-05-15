#pragma once


// LoadNavMesh 대화 상자
#include <string>
#include "CustomDialog.h"
class LoadNavMesh : public CustomDialog
{
	DECLARE_DYNAMIC(LoadNavMesh)

public:
	void SetLoadPath(std::string&& Path);
	void SetPoint(POINT point);
	RECT EditRect;
	std::string FilePath;


	LoadNavMesh(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~LoadNavMesh();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOAD_NAVMESH };
#endif

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit FIleName_Eidt;
	CEdit NewFileName_Edit;
};
