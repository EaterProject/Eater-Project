#pragma once


// SceneSetting 대화 상자
#include "CustomDialog.h"
class SceneSetting : public CustomDialog
{
	DECLARE_DYNAMIC(SceneSetting)

public:
	SceneSetting(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~SceneSetting();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum {IDD = IDD_SCENE_SETTING};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void SSAO_DataSetting();
	void FOG_DataSetting();
	void Bloom_DataSetting();
	//void IBL_DataSetting();
	//void Environment_DataSetting();

	void SSAO_Slider_Setting(RenderOption* Option,CScrollBar* pScrollBar);
	void FOG_Slider_Setting(RenderOption* Option, CScrollBar* pScrollBar);
	//void Environment_Slider_Setting(RenderOption* Option, CScrollBar* pScrollBar);
	//void IBL_Slider_Setting(RenderOption* Option, CScrollBar* pScrollBar);
	void Bloom_Slider_Setting(RenderOption* Option, CScrollBar* pScrollBar);

	void Setting();


	/// 체크박스
	CButton Debug_Check;
	CButton SSAO_Check;
	CButton FOG_Check;
	CButton HDR_Check;
	CButton Shadow_Check;
	CButton IBL_Check;
	CButton BLOOM_Check;
	CButton FXAA_Check;

	///SSAO
	CSliderCtrl SSAO_Radius_Slider;
	CSliderCtrl SSAO_SurfaceEpsiln_Slider;
	CSliderCtrl SSAO_BlurCount_Slider;
	CEdit SSAO_Radius_Edit;
	CEdit SSAO_SurfaceEpsiln_Edit;
	CEdit SSAO_Blur_Edit;

	///FOG
	CSliderCtrl FOG_ColorR_Slider;
	CSliderCtrl FOG_ColorG_Slider;
	CSliderCtrl FOG_ColorB_Slider;
	CSliderCtrl FOG_MoveSpeed_Slider;
	CSliderCtrl FOG_StartDistance_Slider;
	CSliderCtrl FOG_DistanceValue_Slider;
	CSliderCtrl FOG_HeightOffset_Slider;
	CSliderCtrl FOG_HeightValue_Slider;
	CSliderCtrl Fog_DistanceOffset_Slider;
	CEdit FOG_HeightOffset_Edit;
	CEdit FOG_HeightValue_Edit;
	CEdit FOG_DistanceValue_Edit;
	CEdit FOG_StartDistance_Edit;
	CEdit FOG_MoveSpeed_Edit;
	CEdit Fog_DistanceOffset_Edit;
	CEdit FOG_ColorR_Edit;
	CEdit FOG_ColorG_Edit;
	CEdit FOG_ColorB_Edit;

	///Bloom
	CSliderCtrl Bloom_Threhold_Slider;
	CEdit Bloom_Threhold_Edit;
	CSliderCtrl Bloom_Factor_Slider;
	CEdit Bloom_Factor_Edit;


	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);


	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDebugButton();
	afx_msg void OnHDR_Button();
	afx_msg void OnSSAO_Button();
	afx_msg void OnFOG_Button();
	afx_msg void OnShadow_Button();
	afx_msg void OnIBL_Button();
	afx_msg void OnBloom_Button();
	afx_msg void OnFXAA_Button();
	CButton RenderTarget_Check;
	afx_msg void OnBnClickedCheck12();
};
