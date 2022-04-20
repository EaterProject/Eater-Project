#pragma once


// CTAP_1 대화 상자
#include "EditorData.h"
#include "GrobalFunction.h"
#include "SimpleMath.h"
class Transform;
class CTAP_Transform : public CDialogEx
{
	DECLARE_DYNAMIC(CTAP_Transform)

public:
	CTAP_Transform(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTAP_Transform();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAP_TRANSFORM };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void UpdateData(BOOL bSaveAndValidate /* = TRUE */);
	DECLARE_MESSAGE_MAP()
	
public:
	void SetGameObject(Transform* ObjectTransform);
	void UpdateGameObject();
	void GetData(ObjectOption& Obj);
	void Reset();

	
	Transform* ObjectTransform;
	CEdit Pos_X,Pos_Y,Pos_Z;
	CEdit Rot_X,Rot_Y,Rot_Z;
	CEdit Scl_X,Scl_Y,Scl_Z;
	CSliderCtrl Rotation_X_Slider;
	CSliderCtrl Rotation_Y_Slider;
	CSliderCtrl Rotation_Z_Slider;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnAllScaleDown_Button();
	afx_msg void OnAllScaleReset_Button();
	afx_msg void OnAllScaleUp_Button();
	afx_msg void OnResetRot_Button();
	afx_msg void OnUpPosition_Y_Button();
	afx_msg void OnDownPosition_Y_Button();
	afx_msg void OnUpPosition_Z_Button();
	afx_msg void OnDownPosition_Z_Button();
	afx_msg void OnUpPosition_X_Button();
	afx_msg void OnDownPosition_X_Button();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton25();
	afx_msg void OnBnClickedButton23();
};
