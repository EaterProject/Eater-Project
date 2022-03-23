#pragma once


// CTAP_Light 대화 상자
class Light;
class CTAP_Light : public CDialogEx
{
	DECLARE_DYNAMIC(CTAP_Light)

public:
	CTAP_Light(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTAP_Light();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAP_LIGHT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	Light* mLight;
	DECLARE_MESSAGE_MAP()
public:
	void SetGameObject(Light* mLight);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CComboBox Light_Type;
	CEdit Color_R;
	CEdit Color_G;
	CEdit Color_B;
	CEdit Angle_Edit;
	CEdit Range_Edit;
	CEdit Power_Edit;

	CSliderCtrl Color_R_Slider;
	CSliderCtrl Color_G_Slider;
	CSliderCtrl Color_B_Slider;
	CSliderCtrl Angle_Slider;
	CSliderCtrl Range_Slider;
	CSliderCtrl Power_Slider;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCbnSelchangeCombo1();
};
