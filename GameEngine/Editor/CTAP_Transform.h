#pragma once


// CTAP_1 대화 상자
#include "EditorData.h"
#include "GrobalFunction.h"
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void UpdateData(BOOL bSaveAndValidate /* = TRUE */);
	DECLARE_MESSAGE_MAP()
	
public:
	void SetGameObject(Transform* ObjectTransform);
	TransformData GetObjectData();
	void Reset();
	
	Transform* ObjectTransform;
	CEdit Pos_X,Pos_Y,Pos_Z;
	CEdit Rot_X,Rot_Y,Rot_Z;
	CEdit Scl_X,Scl_Y,Scl_Z;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLeftRot_Button();
	afx_msg void OnRightRot_Button();
	afx_msg void OnAllScaleDown_Button();
	afx_msg void OnAllScaleReset_Button();
	afx_msg void OnAllScaleUp_Button();
	afx_msg void OnUpRot_Button();
	afx_msg void OnDownRot_Button();
	afx_msg void OnResetRot_Button();
};
