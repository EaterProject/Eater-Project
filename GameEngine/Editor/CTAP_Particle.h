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

	CEdit MaxCount_Edit;
	CEdit DelayTime_Edit;
	CEdit ShapeRadius_Edit;
	CEdit TextureTiling_X_Edit , TextureTiling_Y_Edit;
	CEdit ShapeRadius_X_Edit , ShapeRadius_Y_Edit , ShapeRadius_Z_Edit;
	CEdit StartForce_X, StartForce_Y, StartForce_Z;
	CEdit StartForce_X_R, StartForce_Y_R, StartForce_Z_R;
	CEdit StartColor_min_R , StartColor_min_G, StartColor_min_B , StartColor_min_A;
	CEdit StartColor_max_R, StartColor_max_G, StartColor_max_B, StartColor_max_A;
	CEdit StartLifeTime , StartLifeTime_min , StartLifeTime_max;
	CEdit StartScale, StartScale_min , StartScale_max;
	CEdit StartRotation , StartRotation_min , StartRotation_max;

	CEdit LifeForce_X, LifeForce_Y, LifeForce_Z;
	CEdit LifeForce_X_max, LifeForce_Y_max, LifeForce_Z_max;

	CButton ShapeRadius_Check;
	CButton StartForce_Check;
	CButton StartColor_Check;
	CButton StartLife_Check;
	CButton StartScale_Check;
	CButton StartRotation_Check;
	CButton LifeForce_Check;
	CEdit LifeRotation , LifeRotation_min , LifeRotation_max;
	
	CButton LifeRotation_Check;
	CEdit LifeScale_min;
	CEdit LifeScale_max;
	CComboBox LifeScale_Combo;

	CEdit LifeColor_min_R;
	CEdit LifeColor_min_G;
	CEdit LifeColor_min_B;
	CEdit LifeColor_min_A;
	CEdit LifeColor_max_R;
	CEdit LifeColor_max_G;
	CEdit LifeColor_max_B;
	CEdit LifeColor_max_A;
	CComboBox LifeColor_Combo;
	CEdit RateOverTime_Edit;

	afx_msg void OnStartColor_Button();
	afx_msg void OnShapeRadius_Button();
	afx_msg void OnStartForce_Button();
	afx_msg void OnStartLifeTime_Check();
	afx_msg void OnStartScale_Button();
	afx_msg void OnStartRotationCheck();
	afx_msg void OnLifeForceCheck();
	afx_msg void OnBnClickedCheck8();
};
