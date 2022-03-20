// Loading.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "Loading.h"
#include "afxdialogex.h"
#include "MainHeader.h"


// Loading 대화 상자

IMPLEMENT_DYNAMIC(Loading, CDialogEx)
std::string Loading::mMsg = "";
Loading::Loading(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOADING, pParent)
{

}

Loading::~Loading()
{

}

void Loading::SetData(int Min, int Max)
{
	//LoadingMax = Max;
	//LoadingBar.SetRange(Min, Max);
	//CString Num;
	//Num.Format(_T("0  / %d"), Max);
	//LoadingBar.SetPos(0);
	//TextEdit.SetWindowTextW(Num);

}

void Loading::SetUpdate(int Number)
{
	//int AnimationCount	= GetLoadAnimationCount();
	//int MeshCount		= GetLoadMeshCount();
	//int TextureCount	= GetLoadTextureCount();
	//
	//int AllCount = AnimationCount + MeshCount + TextureCount;
	//CString Num;
	//Num.Format(_T(" %d / %d"), AllCount ,mOption->LoadMaxCount - 2);
	//LoadingBar.SetPos(AllCount);
	//TextEdit.SetWindowTextW(Num);
	//
	//if (AllCount >= mOption->LoadMaxCount -2)
	//{
	//	this->ShowWindow(SW_HIDE);
	//	mOption->SetWindowPos(NULL, 0, 0, 400, 400, SWP_NOSIZE);
	//}
}

void Loading::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, LoadingBar);
	DDX_Control(pDX, IDC_EDIT1, TextEdit);
}
void Loading::SetMessage(std::string Msg)
{
	mMsg = Msg;
}
BEGIN_MESSAGE_MAP(Loading, CDialogEx)
END_MESSAGE_MAP()





