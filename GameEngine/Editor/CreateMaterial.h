#pragma once


// CreateMaterial 대화 상자
class Material;
class CreateMaterial : public CDialogEx
{
	DECLARE_DYNAMIC(CreateMaterial)

public:
	CreateMaterial(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CreateMaterial();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CREATE_MATERIAL };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	void Reset();
	Material* mMaterial;
	DECLARE_MESSAGE_MAP()
public:
	CEdit Diffuse_Edit;
	CEdit Nomal_Eidt;
	CEdit EmissiveName_Edit;
	CEdit ORM_Edit;
	CSliderCtrl Emissive_Slider;
	CSliderCtrl Roughnees_Slider;
	CSliderCtrl Matallic_Slider;
	CEdit Emissive_Edit;
	CEdit Roughness_Edit;
	CEdit Matallic_Edit;
	CSliderCtrl Add_R_Slider;
	CSliderCtrl Add_G_Slider;
	CSliderCtrl Add_B_Slider;
	CEdit AddColor_R;
	CEdit AddColor_G;
	CEdit AddColor_B;
	CEdit MaterialName_Edit;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
