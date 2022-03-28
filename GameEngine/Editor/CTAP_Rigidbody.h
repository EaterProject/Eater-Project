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
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAP_RIGIDBODY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
