#pragma once


// CTAP_Collider 대화 상자
class Collider;
class PhysCollider;
class CTAP_Collider : public CDialogEx
{
	DECLARE_DYNAMIC(CTAP_Collider)

public:
	CTAP_Collider(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTAP_Collider();
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAP_COLLIDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	PhysCollider*	mPhysCollider;
	Collider*		mCollider;
	DECLARE_MESSAGE_MAP()
public:
	void SetGameObject(Collider* collider);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	void UpdateObject();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTriggerCheck();


	CComboBox ColliderType_Combo;
	CEdit BoxType_X_Eidt;
	CEdit BoxType_Y_Eidt;
	CEdit BoxType_Z_Eidt;
	CEdit SphereType_R_Eidt;
	CEdit Capsule_H_Edit;
	CEdit Capsule_R_Edit;
	CEdit Center_X_Edit;
	CEdit Center_Y_Edit;
	CEdit Center_Z_Edit;
	CSliderCtrl StaticFriction_Slider;
	CSliderCtrl DynamicFriction_Slider;
	CSliderCtrl Restitution_Slider;
	afx_msg void OnCreatePhyCollider();
	afx_msg void OnCbnSelchangeCombo1();
	CEdit Dynamic_Eidt;
	CEdit Static_Eidt;
	CEdit Restition_Edit;
	CButton Trigger_Check;
};
