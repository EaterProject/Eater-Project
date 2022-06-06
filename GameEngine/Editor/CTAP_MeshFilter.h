#pragma once
#include "EditorData.h"
#include "CustomDialog.h"

// CTAP_MeshFilter 대화 상자
class MeshFilter;
class Material;
class CTAP_MeshFilter : public CustomDialog
{
	DECLARE_DYNAMIC(CTAP_MeshFilter)

public:
	CTAP_MeshFilter(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTAP_MeshFilter();

	void SetGameObject(MeshFilter* ObjectMeshFilter);
	void GetData(ObjectOption& Option);
	void UpdateGameObject();
	

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAP_MESHFILTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	void SetSlider(CSliderCtrl& Silder, int Number);
	DECLARE_MESSAGE_MAP()
public:
	CEdit ModelName_Edit;
	CEdit MeshName_Edit;
	CEdit Diffuse_Edit;
	CEdit Nomal_Eidt;
	CEdit EmissiveName_Edit;
	CEdit ORM_Edit;

	CRect EditRect[7];

	MeshFilter* mMeshFilter;
	Material*	mMaterial;

	const int Diffuse_Index		= 0;
	const int Nomal_Index		= 1;
	const int ORM_Index			= 2;
	const int Emissive_Index	= 3;

	const int ModelName_Index	= 4;
	const int MeshName_Index	= 5;
	const int Material_Index	= 6;

	CSliderCtrl Emissive_Slider;
	CSliderCtrl Roughnees_Slider;
	CSliderCtrl Matallic_Slider;

	CEdit Emissive_Edit;
	CEdit Roughness_Edit;
	CEdit Matallic_Edit;

	afx_msg LRESULT OnUserFun(WPARAM wParam, LPARAM lparam);
	afx_msg void OnDiffuse_Button();
	afx_msg void OnNomal_Button();
	afx_msg void OnORM_Button();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEmissive_Button();
	
	CEdit AddColor_R;
	CEdit AddColor_G;
	CEdit AddColor_B;
	CSliderCtrl Add_R_Slider;
	CSliderCtrl Add_G_Slider;
	CSliderCtrl Add_B_Slider;
	CEdit MaterialName_Edit;
	CSliderCtrl LimLight_R;
	CSliderCtrl LimLight_G;
	CSliderCtrl LimLight_B;
	CEdit LimLight_R_Edit;
	CEdit LimLight_G_Edit;
	CEdit LimLight_B_Edit;
	CSliderCtrl LimLight_Factor;
	CSliderCtrl LimLight_Width;
	CEdit LimLight_Factor_Edit;
	CEdit LimLight_Width_Edit;
	CImageList ImgList_Folder;

	CMFCColorButton Custom_AddColor_Button;
	CMFCColorButton Custom_LimLightColor_Button;

	afx_msg void OnMaterialName_Button();
	afx_msg void OnBnClickedButton24();
	afx_msg void OnBnClickedButton29();
	afx_msg void OnBnClickedButton30();
	afx_msg void OnBnClickedButton31();
	afx_msg void OnCreateMaterial();
	afx_msg void OnCustom_Color_Button();
	afx_msg void OnCustom_LimLightColor_Button();
};
