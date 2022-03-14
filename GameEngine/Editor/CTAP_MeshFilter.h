#pragma once


// CTAP_MeshFilter 대화 상자
class MeshFilter;
class CTAP_MeshFilter : public CDialogEx
{
	DECLARE_DYNAMIC(CTAP_MeshFilter)

public:
	CTAP_MeshFilter(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTAP_MeshFilter();

	void SetGameObject(MeshFilter* ObjectMeshFilter);
	

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAP_MESHFILTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CEdit Diffuse_Edit;
	CEdit MeshName_Edit;
	CEdit Nomal_Eidt;
	CEdit ORM_Edit;

	CRect EditRect[4];

	MeshFilter* mMeshFilter;

	const int MeshName_Index	= 0;
	const int Diffuse_Index		= 1;
	const int Nomal_Index		= 2;
	const int ORM_Index			= 3;

	afx_msg LRESULT OnUserFun(WPARAM wParam, LPARAM lparam);

	afx_msg void OnDiffuse_Button();
	afx_msg void OnNomal_Button();
	afx_msg void OnORM_Button();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
