// SceneSaveDialog.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "SceneSaveDialog.h"
#include "afxdialogex.h"
#include "RightOption.h"


// SceneSaveDialog 대화 상자

IMPLEMENT_DYNAMIC(SceneSaveDialog, CDialogEx)

SceneSaveDialog::SceneSaveDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SCENE_SAVE, pParent)
{

}

SceneSaveDialog::~SceneSaveDialog()
{

}



void SceneSaveDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, SceneName_Edit);
}


BEGIN_MESSAGE_MAP(SceneSaveDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &SceneSaveDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &SceneSaveDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// SceneSaveDialog 메시지 처리기


void SceneSaveDialog::Initialize(RightOption* mRight)
{
	mRight = mRightOption;
}

void SceneSaveDialog::OnBnClickedOk()
{
	SceneName_Edit.GetWindowTextW(Name);
	isOK = true;
	CDialogEx::OnOK();
}


void SceneSaveDialog::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	isOK = false;
	CDialogEx::OnCancel();
}
