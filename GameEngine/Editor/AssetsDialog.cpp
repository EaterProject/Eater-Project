﻿// AssetsDialog.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "AssetsDialog.h"
#include "afxdialogex.h"
#include "GrobalFunction.h"
#include <stack>
#include "Demo.h"
#include "RightOption.h"
#define MAXPATH 256
// AssetsDialog 대화 상자

IMPLEMENT_DYNAMIC(AssetsDialog, CDialogEx)

AssetsDialog::AssetsDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ASSETS, pParent)
{

}

AssetsDialog::~AssetsDialog()
{
}

BOOL AssetsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	//Assets 에 필요한 이미지들을 생성후 연결
	CBitmap bit01;
	bit01.LoadBitmap(IDB_BITMAP5);
	ImgList_Folder.Create(15, 15, ILC_COLOR8 | ILC_MASK, 2, 1);
	ImgList_Folder.Add(&bit01, RGB(255, 0, 255));
	AssetsTree.SetImageList(&ImgList_Folder, TVSIL_NORMAL);

	CBitmap bit02;
	bit02.LoadBitmap(IDB_BITMAP3);
	ImgList_Big.Create(50, 50, ILC_COLOR8 | ILC_MASK, 6, 1);
	ImgList_Big.Add(&bit02, RGB(255, 0, 255));

	AssetsTreeTopParent = AssetsTree.InsertItem(_T("Assets"), 0, 0, NULL);
	AssetsFile.SetImageList(&ImgList_Big, TVSIL_NORMAL);
	
	FindChildFile(AssetsTreeTopParent, _T("../Assets"));
	AssetsFile.DeleteAllItems();

	//ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);
	//ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
	DragAcceptFiles(TRUE);

	
	return 0;
}

void AssetsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, AssetsTree);
	DDX_Control(pDX, IDC_LIST2, AssetsFile);
}


BEGIN_MESSAGE_MAP(AssetsDialog, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &AssetsDialog::OnAssetsClick)
	ON_WM_DROPFILES()
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST2, &AssetsDialog::OnLvnBegindragList2)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, &AssetsDialog::OnNMClickList2)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	END_MESSAGE_MAP()


// AssetsDialog 메시지 처리기


void AssetsDialog::Initialize(RightOption* mRight)
{
	mRightOption = mRight;
}

int AssetsDialog::FindChildFile(HTREEITEM hParentItem, CString str)
{
	int Count = 0;
	int ChildCount = 0;
	str += _T("/*.*");

	CFileFind FileFind;
	BOOL bFound = FileFind.FindFile(str);
	while (bFound)
	{
		bFound = FileFind.FindNextFile();
		if (FileFind.IsDots())
		{
			continue;
		}
		if (FileFind.IsDirectory())
		{
			ChildCount += FindChildFile(AssetsTree.InsertItem(FileFind.GetFileName(), 0, 0, hParentItem), FileFind.GetFilePath());
		}
		else
		{
			std::string Name = ChangeToString(FileFind.GetFileName());
			AssetsFile.InsertItem(0, FileFind.GetFileName(), GetFileNameType(Name));
			Count++;
		}
	}
	
	return Count + ChildCount;
}

void AssetsDialog::OnSize(UINT nType, int cx, int cy)
{
	if (AssetsTree)
	{
		AssetsTree.MoveWindow(0, 0, 250, cy);
	}

	if (AssetsFile)
	{
		AssetsFile.MoveWindow(250, 0, cx- 250, cy);
	}

	CDialogEx::OnSize(nType, cx, cy);
}

void AssetsDialog::OnAssetsClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	AssetsFile.DeleteAllItems();
	//클릭한 아이템으로 Assets로부터의 경로를 알아온다
	HTREEITEM ChoiceItem = AssetsTree.GetSelectedItem();
	CString Name = AssetsTree.GetItemText(ChoiceItem);
	CString StartName = Name;
	std::stack<CString> StrList;
	std::string ClickItemPath = "..";
	while (true)
	{
		if (ChoiceItem == NULL || Name == L"Assets")
		{
			break;
		}
		else
		{
			ChoiceItem = AssetsTree.GetParentItem(ChoiceItem);
			Name = AssetsTree.GetItemText(ChoiceItem);
			StrList.push(Name);
		}
	}

	while (StrList.size() != 0)
	{
		CString TOP = StrList.top();
		StrList.pop();
		CT2CA convertedString(TOP);
		ClickItemPath += "/";
		ClickItemPath += convertedString;
	}
	CT2CA convertedString(StartName);
	ClickItemPath += "/";
	ClickItemPath += convertedString;
	ClickAssetsPath = ClickItemPath.c_str();
	ClickItemPath += "/*.*";


	CFileFind FileFind;
	CString FilePath = ChangeToCString(ClickItemPath);
	//RightOption::GetThis()->ClickAssetsPath = ClickAssetsPath;
	BOOL bFound = FileFind.FindFile(FilePath);
	while (bFound)
	{
		bFound = FileFind.FindNextFile();
		if (FileFind.IsDirectory() == FALSE)
		{
			std::string Name = ChangeToString(FileFind.GetFileName());
			AssetsFile.InsertItem(0, FileFind.GetFileName(), GetFileNameType(Name));
		}
	}
}

void AssetsDialog::OnDropFiles(HDROP hDropInfo)
{
	//외부 폴더에서 Tool쪽으로 파일을 옮겼을때 처리
	TCHAR FileName[MAXPATH] = { 0, };
	UINT count = DragQueryFile(hDropInfo, 0xFFFFFFFF, FileName, MAXPATH);
	for (UINT i = 0; i < count; i++)
	{
		DragQueryFile(hDropInfo, i, FileName, MAXPATH);

		CString Change = FileName;
		CT2CA convertedString(Change);
		std::string Name = convertedString;
		for (int i = 0; i < Name.size(); i++)
		{
			if (Name[i] == '\\')
			{
				Name[i] = '/';
			}
		}

		//이름 찾기
		std::size_t Start = Name.rfind("/") + 1;
		std::size_t End = Name.rfind(".") - Start;
		std::string MeshName = Name.substr(Start, End);

		//타입찾기
		Start = Name.rfind(".");
		End = Name.length() - Start;
		std::string MeshType = Name.substr(Start, End);
		MeshName += MeshType;

		//파일을 Assets폴더로 복사한다
		std::string CopyFilePath = ChangeToString(ClickAssetsPath) + "/" + MeshName;
		CString ChangeCopyFile;
		ChangeCopyFile = CopyFilePath.c_str();
		bool Move = CopyFile(FileName, ChangeCopyFile, TRUE);
		if (Move == false) { AfxMessageBox(_T("Error : 파일 이동 실패")); return; }

		//아이템 이름으로 UI 보여주기
		CString ItemName;
		ItemName = MeshName.c_str();
		AssetsFile.InsertItem(0, ItemName, 1);

		//메쉬 로드
		Demo::MeshLoad(CopyFilePath);
	}
	CDialogEx::OnDropFiles(hDropInfo);
}


void AssetsDialog::OnLvnBegindragList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	//이미지 컨트롤 에서 드레그가 시작되었을때
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	CPoint ptDrag, ptAction;

	DragItemPosOffset = pNMLV->ptAction;
	DragItemIndex = pNMLV->iItem;

	DragItemName = AssetsFile.GetItemText(DragItemIndex, 0);

	//이동시킬 아이템 랜더링
	DragImg = AssetsFile.CreateDragImage(DragItemIndex, &ptDrag);
	DragImg->SetBkColor(RGB(0, 0, 0));
	SetCapture();

	DragImg->BeginDrag(0, ptAction - ptDrag);
	AssetsFile.ClientToScreen(&ptAction);
	DragImg->DragEnter(NULL, ptAction);

	*pResult = 0;
}


void AssetsDialog::OnNMClickList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int ClickItemIndex = pNMListView->iItem;
	ClickItemName = AssetsFile.GetItemText(ClickItemIndex, 0);
	RightOption::GetThis()->ClickItemName = ClickItemName;
	*pResult = 0;
}


void AssetsDialog::OnMouseMove(UINT nFlags, CPoint point)
{
	//마우스가 이동 중 일때
	if (DragImg != NULL)
	{
		ClientToScreen(&point);
		CWnd* PWnd = CWnd::WindowFromPoint(point);
		if (PWnd != NULL)
		{
			DragImg->DragEnter(NULL, point);
			point.x -= DragItemPosOffset.x;
			point.y -= DragItemPosOffset.y;
			DragImg->DragMove(point);
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL AssetsDialog::PreTranslateMessage(MSG* pMsg)
{
	//키눌린 메시지가 들어올때 esc이거나 return  값이면
   //걍 리턴 시켜준다.
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
	return CDialog::PreTranslateMessage(pMsg);
}


void AssetsDialog::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (DragImg != NULL)
	{
		//아이템을 드레그 하고있을떄 처리
		ClientToScreen(&point);
		DragImg->EndDrag();
		ReleaseCapture();

		HWND hwnd = AfxGetMainWnd()->GetSafeHwnd();
		 GetDlgItem(IDD_RIGHT_OPTION, &hwnd);
	
		 mRightOption = RightOption::GetThis();

		 mRightOption->DragItemName = DragItemName;
		 mRightOption->ChickHirearchyDarg(point);
		 mRightOption->ChickTapDrag(point);
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}
