#pragma once


// CTAP_Light 대화 상자

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

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
