#pragma once
#include <string>

// LoadTerrain 대화 상자

class LoadTerrain : public CDialogEx
{
	DECLARE_DYNAMIC(LoadTerrain)

public:
	LoadTerrain(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~LoadTerrain();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOAD_TERRAIN };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	RECT EditRect[3];
	const int ModelName_index	= 0;
	const int Mask01Name_index	= 1;
	const int Mask02Name_index	= 2;

	std::string ModelName;
	std::string ModelPath;
	std::string Mask01Name;
	std::string Mask02Name;


	DECLARE_MESSAGE_MAP()
public:
	CEdit FBXName_Edit;
	CEdit Mask01Name_Edit;
	CEdit Mask02Name_Edit;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
