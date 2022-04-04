#pragma once


// CTAP_Rigidbody 대화 상자
class Rigidbody;
class CTAP_Rigidbody : public CDialogEx
{
	DECLARE_DYNAMIC(CTAP_Rigidbody)

public:
	CTAP_Rigidbody(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTAP_Rigidbody();

// 대화 상자 데이터입니다.
	void SetGameObject(Rigidbody* rigidbody);
	void UpdateObject();
	Rigidbody* mRigidbody;
	int Freeze[6];
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAP_RIGIDBODY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CEdit Mass_Edit;
	CButton Kinematic_Click;

	CButton Freeze_PosX_Click;
	CButton Freeze_PosY_Click;
	CButton Freeze_PosZ_Click;

	CButton Freeze_RotX_Click;
	CButton Freeze_RotY_Click;
	CButton Freeze_RotZ_Click;
	CButton Gravity_Check;
	afx_msg void OnGravityButton();
	afx_msg void OnKinematicButton();
	afx_msg void OnFreezePosX_Button();
	afx_msg void OnOnFreezePosY_Button();
	afx_msg void OnFreezePosZ_Button();
	afx_msg void OnFreezeRotX_Button();
	afx_msg void OnFreezeRotY_Button();
	afx_msg void OnFreezeRotZ_Button();
};
