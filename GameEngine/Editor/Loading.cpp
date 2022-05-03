// Loading.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "afxdialogex.h"
#include "MainHeader.h"
#include "Loading.h"

IMPLEMENT_DYNAMIC(Loading, CustomDialog)

// Loading 대화 상자
Loading::Loading(CWnd* pParent /*=nullptr*/)
	: CustomDialog(IDD_LOADING, pParent)
{

}

Loading::~Loading()
{

}

BOOL Loading::OnInitDialog()
{
	CDialog::OnInitDialog();
	mfont.CreatePointFont(200, L"나눔고딕");
	LoadFileList.SetFont(&mfont);
	LoadingTypeEdit.SetFont(&mfont);
	AllAssetsCount.SetFont(&mfont);
	LoadingTypeEdit.SetWindowTextW(L"MFC 초기화 중...");
	return 0;
}

void Loading::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, LoadingTypeEdit);
	DDX_Control(pDX, IDC_LIST2, LoadFileList);
	DDX_Control(pDX, IDC_EDIT3, AllAssetsCount);
}

BEGIN_MESSAGE_MAP(Loading, CDialogEx)
END_MESSAGE_MAP()





