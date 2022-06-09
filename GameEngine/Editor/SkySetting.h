#pragma once
#include "CustomDialog.h"
#include <string>

// SkySetting 대화 상자

class RenderOption;
class SkySetting : public CustomDialog
{
	DECLARE_DYNAMIC(SkySetting)

public:
	SkySetting(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~SkySetting();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SKY_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	void SetCheck(CString FileName);

	RenderOption* mRenderOption;

	CSliderCtrl SkyCube_Size_Slider;
	CSliderCtrl SkyCube_Threhold_Slider;
	CSliderCtrl SkyCube_Angle_Slider;
	CEdit SkyCube_Size_Edit;
	CEdit SkyCube_Threshold_Edit;
	CEdit SkyCube_Angle_Edit;
	CButton SkyCube_HDRI_Check;


	CSliderCtrl SkyLight_Factor_Slider;
	CEdit SkyLight_Factor_Edit;
	CSliderCtrl SkyLight_Threshold_Slider;
	CEdit SkyLight_Threshold_Edit;
	CSliderCtrl SkyLight_Angle_Slider;
	CEdit SkyLight_Angle_Edit;
	CButton SkyLight_HDRI_Check;

	CEdit SkyCube_Map_Edit;
	CEdit SkyLight_Map_Edit;


	std::string SkyCubeName;
	float SkyCubeAngle;
	float SkyCubeThreshold;
	bool SkyCubeHDRI;

	std::string SkyLightName;
	float SkyLightAngle;
	float SkyLightThreshold;
	bool SkyLightHDRI;

	CString SkyCube_SaveName_Edit;
	CString SkyLight_SaveName_Edit;
	
	afx_msg void OnSkyCubeHDRI();
	afx_msg void OnSkyCubeBakeButton();
	afx_msg void OnSkyLightBakeButton();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnSkyLightHDRI();
};
