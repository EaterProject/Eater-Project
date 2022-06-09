#pragma once


// CTAP_Particle 대화 상자
#include "CustomDialog.h"
class ParticleSystem;
class CTAP_Particle : public CustomDialog
{
	DECLARE_DYNAMIC(CTAP_Particle)

public:
	CTAP_Particle(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTAP_Particle();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAP_PARTICLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	void UpDateSetting();
	void UpDateStartSetting();
	void UpDateLifeSetting();
	void SetGameObject(ParticleSystem* ObjectParticleSystem);
	void GetObjectData();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CComboBox RenderType_Combo;
	ParticleSystem* mParticleSystem;

	CEdit StartForce_X, StartForce_Y, StartForce_Z;
	CEdit StartForce_X_R, StartForce_Y_R, StartForce_Z_R;

	CEdit StartLifeTime_min,StartLifeTime_max;
	CEdit StartScale_min,StartScale_max;
	CEdit StartRotation_min,StartRotation_max;

	CEdit LifeForce_X, LifeForce_Y, LifeForce_Z;
	CEdit LifeForce_X_max, LifeForce_Y_max, LifeForce_Z_max;
	CEdit LifeRotation_min , LifeRotation_max;
	
	CEdit LifeScale_min;
	CEdit LifeScale_max;
	CComboBox LifeScale_Combo;
	
	CComboBox LifeColor_Combo;

	///시작 셋팅 변수
	int MaxParticle;
	float DelayTime;
	float ShapeRadius_X;
	float ShapeRadius_Y;
	float ShapeRadius_Z;
	int Tiling_X;
	int Tiling_Y;
	float RateOverTime;
	CSliderCtrl Strength_Slider;
	CString Strength_Str;


	int StartColor_Min_Alpha;
	int StartColor_Max_Alpha;
	int LifeColor_Min_Alpha;
	int LifeColor_Max_Alpha;

	Vector4 StartColorMin = {0,0,0,255};
	Vector4 StartColorMax = {0,0,0,255};
	Vector4 LifeColorMin = {0,0,0,255};
	Vector4 LifeColorMax = {0,0,0,255};
	CMFCColorButton Custom_StartColor_Min_Button;
	CMFCColorButton Custom_StartColor_Max_Button;
	CMFCColorButton Custom_LifeColor_Min_Button;
	CMFCColorButton Custom_LifeColor_Max_Button;
	afx_msg void OnSaveFile();
	afx_msg void OnCustom_StartColor_Min_Button();
	afx_msg void OnCustom_StartColor_Max_Button();
	afx_msg void OnCustom_LifeColor_Min_Button();
	afx_msg void OnCustom_LifeColor_Max_Button();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLifeScaleOption();
	afx_msg void OnLifeColorOption();
};
