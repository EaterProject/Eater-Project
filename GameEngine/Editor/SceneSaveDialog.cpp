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
END_MESSAGE_MAP()


// SceneSaveDialog 메시지 처리기


void SceneSaveDialog::Initialize(RightOption* mRight)
{
	mRight = mRightOption;
}

void SceneSaveDialog::OnBnClickedOk()
{
	SceneName_Edit.GetWindowTextW(Name);
	CDialogEx::OnOK();
}
