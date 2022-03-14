// CTAP_Light.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "CTAP_Light.h"
#include "afxdialogex.h"


// CTAP_Light 대화 상자

IMPLEMENT_DYNAMIC(CTAP_Light, CDialogEx)

CTAP_Light::CTAP_Light(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAP_LIGHT, pParent)
{

}

CTAP_Light::~CTAP_Light()
{
}

void CTAP_Light::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTAP_Light, CDialogEx)
END_MESSAGE_MAP()


// CTAP_Light 메시지 처리기


BOOL CTAP_Light::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:
			return TRUE;
		default:
			break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
