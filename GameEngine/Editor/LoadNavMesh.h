#pragma once


// LoadNavMesh 대화 상자

class LoadNavMesh : public CDialogEx
{
	DECLARE_DYNAMIC(LoadNavMesh)

public:
	LoadNavMesh(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~LoadNavMesh();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOAD_NAVMESH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
