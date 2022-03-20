// RightOption.cpp: 구현 파일

#include "pch.h"
#include "Editor.h"
#include "RightOption.h"
#include "afxdialogex.h"
#include "Demo.h"

#include "CTAP_Transform.h"
#include "CTAP_Animation.h"
#include "CTAP_MeshFilter.h"
#include "CTAP_Light.h"
#include "CTAP_Particle.h"

#include "FileOption.h"
#include "EditorManager.h"
#include "SceneSaveDialog.h"
#include "Loading.h"

#include <stack>
#include "MainHeader.h"
#include "GameObject.h"
#include "Transform.h"
#include "AnimationController.h"
#include "MeshFilter.h"
#include "ParticleSystem.h"

// RightOption 대화 상자

IMPLEMENT_DYNAMIC(RightOption, CDialogEx)
RightOption*  RightOption::thisPointer = nullptr;
RightOption::RightOption(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RIGHT_OPTION, pParent)
{

}

RightOption::~RightOption()
{

}

BOOL RightOption::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_EditorManager = new EditorManager();
	m_EditorManager->Initialize();

	mFileOption = new FileOption();
	mFileOption->Initialize(m_EditorManager);

	mScene = new SceneSaveDialog();
	mScene->Initialize(this);

	mLoading = new Loading();
	mLoading->Create(IDD_LOADING);
	mLoading->ShowWindow(SW_HIDE);


	//텝정보들 생성
	CRect rect;
	Component_TapList.GetWindowRect(&rect);

	mTransform = new CTAP_Transform();
	mTransform->Create(IDD_TAP_TRANSFORM, &Component_TapList);
	mTransform->MoveWindow(0, 25, rect.Width() - 5, rect.Height() - 25);
	mTransform->ShowWindow(SW_HIDE);

	mAnimation = new CTAP_Animation();
	mAnimation->Create(IDD_TAP_ANIMATION, &Component_TapList);
	mAnimation->MoveWindow(0, 25, rect.Width() - 5, rect.Height() - 25);
	mAnimation->ShowWindow(SW_HIDE);

	mMeshFilter = new CTAP_MeshFilter();
	mMeshFilter->Create(IDD_TAP_MESHFILTER, &Component_TapList);
	mMeshFilter->MoveWindow(0, 25, rect.Width() - 5, rect.Height() - 25);
	mMeshFilter->ShowWindow(SW_HIDE);


	mPrticle = new CTAP_Particle();
	mPrticle->Create(IDD_TAP_PARTICLE, &Component_TapList);
	mPrticle->MoveWindow(0, 25, rect.Width() - 5, rect.Height() - 25);
	mPrticle->ShowWindow(SW_HIDE);

	thisPointer = this;
	SetTimer(1, 1000, NULL);

	
	return 0;
}

void RightOption::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE2, HirearchyTree);
	DDX_Control(pDX, IDC_EDIT2, HirearchyEdit);
	DDX_Control(pDX, IDC_TAB1, Component_TapList);
	DDX_Control(pDX, IDC_EDIT9, FilePathEdit);
}

BEGIN_MESSAGE_MAP(RightOption, CDialogEx)
	ON_WM_SIZE()
	ON_WM_DROPFILES()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(NM_DBLCLK, IDC_TREE2, &RightOption::OnChoice_Hirearchy_Item)
	ON_BN_CLICKED(IDC_BUTTON8, &RightOption::OnDelteObject_Button)
	ON_BN_CLICKED(IDC_BUTTON3, &RightOption::OnOpenAssetsFolder)
	ON_BN_CLICKED(IDC_BUTTON4, &RightOption::OnDeleteFile_Button)
	ON_BN_CLICKED(IDC_BUTTON7, &RightOption::OnChange_DataFormat)
	ON_WM_TIMER()
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_BUTTON9, &RightOption::OnOpenExeFile_Button)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &RightOption::OnClickTap)
	ON_BN_CLICKED(IDC_BUTTON11, &RightOption::OnSaveScene)
	ON_BN_CLICKED(IDC_BUTTON12, &RightOption::OnCreateParticle)
	ON_BN_CLICKED(IDC_BUTTON10, &RightOption::OnCreateTerrain)
	ON_MESSAGE(M_MSG_MeshFilter,		&RightOption::OnUserFun)
	ON_WM_MOUSEHWHEEL()
END_MESSAGE_MAP()

RightOption* RightOption::GetThis()
{
	return thisPointer;
}

void RightOption::Create_Hirearchy_Item(GameObject* Obj, HTREEITEM TOP)
{
	int BoneCount = Obj->GetChildBoneCount();
	int MeshCount = Obj->GetChildMeshCount();

	for (int i = 0; i < BoneCount; i++) 
	{
		GameObject* Child = Obj->GetChildBone(i);
		HTREEITEM ChildItem = HirearchyTree.InsertItem(ChangeToCString(Child->Name),TOP);
		Create_Hirearchy_Item(Child, ChildItem);
	}

	for (int i = 0; i < MeshCount; i++)
	{
		GameObject* Child = Obj->GetChildMesh(i);
		HTREEITEM ChildItem = HirearchyTree.InsertItem(ChangeToCString(Child->Name), TOP);
		Create_Hirearchy_Item(Child, ChildItem);
	}
}

void RightOption::Delete_Hirearchy_Item(HTREEITEM TOP)
{
	if (HirearchyTree.ItemHasChildren(TOP))
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = HirearchyTree.GetChildItem(TOP);

		while (hChildItem != NULL)
		{
			hNextItem = HirearchyTree.GetNextItem(hChildItem, TVGN_NEXT);
			HirearchyTree.DeleteItem(hChildItem);
			hChildItem = hNextItem;
		}
	}
}

void RightOption::ChickTapDrag(CPoint point)
{

	//Tap컨트롤에  드래그했을때 처리
	CRect rect;
	Component_TapList.GetWindowRect(&rect);
	if (rect.left <= point.x &&
		rect.right >= point.x &&
		rect.top <= point.y &&
		rect.bottom >= point.y)
	{
		//현재 켜져있는 창을 알아온다
		int num = Component_TapList.GetCurSel();
		TCHAR buffer[256] = { 0 };
		TC_ITEM data;
		data.mask = TCIF_TEXT;
		data.pszText = buffer;
		data.cchTextMax = 256;
		Component_TapList.GetItem(num, &data);
		CString ComponentName;
		ComponentName = buffer;

		if (ComponentName == "MeshFilter")
		{
			CString* pstr = new CString(DragItemName);
			mMeshFilter->SendMessage(M_MSG_MeshFilter, 0, (LPARAM)pstr);
			delete pstr;
		}
	}
}

void RightOption::ChickHirearchyDarg(CPoint point)
{
	//하이어라키 창에 드래그했을때 처리
	CRect rect;
	HirearchyTree.GetWindowRect(&rect);

	//범위 안에 들어와있는지 체크
	if (rect.left	<= point.x &&
		rect.right	>= point.x &&
		rect.top	<= point.y &&
		rect.bottom >= point.y)
	{
		std::string Name = ChangeToString(DragItemName);
		int Type = GetFileNameType(Name);

		//드래그한 아이템 타입
		switch (Type)
		{
		case FBX:
		case EATER: 
		{
			Name = Name.substr(0, Name.rfind('.'));
			GameObject* Object = Demo::CreateObject(Name);
			HTREEITEM TopParent = HirearchyTree.InsertItem(ChangeToCString(Object->Name));
			break;
		}
		case SCENE:
		{
			Demo::LoadScene(Name);
			//HirearchyTree.DeleteAllItems();
			std::map<std::string, GameObject*>::iterator Start_it	= Demo::ObjectList.begin();
			std::map<std::string, GameObject*>::iterator End_it		= Demo::ObjectList.end();
			for (Start_it; Start_it != End_it; Start_it++)
			{
				HirearchyTree.InsertItem(ChangeToCString(Start_it->first));
			}
			break;
		}
		default:
			//나머지는 에러 
			AfxMessageBox(_T("Error : fbx, Eater,Scene 파일만 가능합니다"));
			break;
		}		
	}
}

GameObject* RightOption::FindGameObjectParent(HTREEITEM mItem)
{
	//클릭한 아이템이 부모가 있는 아이템이라면 Demo씬에서 부모안에 오브젝트를 찾아 반환
	HTREEITEM Parent	= mItem;
	CString	 MeshName	= HirearchyTree.GetItemText(mItem);
	CString ParentName;
	GameObject* Object = nullptr;
	while (true)
	{
		if (Parent == NULL)
		{
			break;
		}
		else
		{
			ParentName = HirearchyTree.GetItemText(Parent);
			Parent = HirearchyTree.GetParentItem(Parent);
		}
	}

	if (ParentName == MeshName)
	{
		Object = Demo::FindMesh(ChangeToString(MeshName));
	}
	else
	{
		Object = Demo::FindMesh(ChangeToString(MeshName), ChangeToString(ParentName));
	}


	return Object;
}

void RightOption::AssetsInitialize()
{
	
}

void RightOption::OnChoice_Hirearchy_Item(NMHDR* pNMHDR, LRESULT* pResult)
{
	//선택한 아이템의 이름을 가져온다
	HTREEITEM ChoiceItem = HirearchyTree.GetSelectedItem();

	CString Name = HirearchyTree.GetItemText(ChoiceItem);
	std::string MeshName = ChangeToString(Name);

	//에디터의 매쉬 이름을 출력
	ChoiceHirearchyName = MeshName.substr(0, MeshName.rfind('.'));
	Name = ChoiceHirearchyName.c_str();
	HirearchyEdit.SetWindowTextW(Name);

	Component_TapList.DeleteAllItems();
	GameObject* ChoiceObject = FindGameObjectParent(ChoiceItem);
	Delete_Hirearchy_Item(ChoiceItem);
	Create_Hirearchy_Item(ChoiceObject, ChoiceItem);

	Transform*			 TR	= ChoiceObject->GetTransform();
	AnimationController* AC = ChoiceObject->GetComponent<AnimationController>();
	MeshFilter*			 MF = ChoiceObject->GetComponent<MeshFilter>();
	ParticleSystem*		 PS = ChoiceObject->GetComponent<ParticleSystem>();

	int FrontCount = 0;
	if (TR != nullptr)
	{
		Component_TapList.InsertItem(FrontCount, L"Transform");
		mTransform->ShowWindow(SW_SHOW);
		mTransform->SetGameObject(TR);
		FrontCount++;
	}
	if (AC != nullptr)
	{
		Component_TapList.InsertItem(FrontCount, L"Animation");
		mAnimation->ShowWindow(SW_HIDE);
		mAnimation->SetGameObject(AC);
		FrontCount++;
	}

	if (MF != nullptr)
	{
		Component_TapList.InsertItem(FrontCount, L"MeshFilter");
		mMeshFilter->ShowWindow(SW_HIDE);
		mMeshFilter->SetGameObject(MF);
		FrontCount++;
	}

	if (PS != nullptr)
	{
		Component_TapList.InsertItem(FrontCount, L"Particle");
		mPrticle->ShowWindow(SW_HIDE);
		mPrticle->SetGameObject(PS);
		FrontCount++;
	}
	Component_TapList.SetCurSel(0);
}

void RightOption::OnDelteObject_Button()
{
	std::string Name;
	Name = ChoiceHirearchyName;
	Name = Name.substr(0, Name.rfind('.'));
	bool Delete = Demo::DeleteObject(Name);
	if (Delete == true)
	{
		HTREEITEM item = HirearchyTree.GetSelectedItem();
		if (item != NULL)
		{
			HirearchyTree.DeleteItem(item);
		}

		ChoiceHirearchyName = "";
		HirearchyEdit.SetWindowTextW(_T(""));
	}
	else
	{
		AfxMessageBox(_T("Error : 삭제 실패 \n 제대로 선택하였는지 확인"));
	}
}

void RightOption::OnOpenAssetsFolder()
{
	//에셋폴더를 연다
	TCHAR chFilePath[256] = { 0, };
	GetModuleFileName(NULL, chFilePath, 256);

	CString strFolderPath(chFilePath);
	for (int i = 0; i < 2; i++)
	{
		strFolderPath = strFolderPath.Left(strFolderPath.ReverseFind('\\'));
	}
	strFolderPath += _T("\\Assets");
	ShellExecute(NULL, _T("open"), _T("explorer"), strFolderPath, NULL, SW_SHOW);
}

void RightOption::OnDeleteFile_Button()
{
	//에셋폴더안에 파일을 삭제한다
	CString PathName = ClickAssetsPath;
	PathName += _T("/");
	PathName += ClickItemName;
	bool FileDelete = DeleteFile(PathName);
	if (FileDelete == false)
	{
		AfxMessageBox(_T("파일 지우기 실패"));
	}
	else
	{
		AfxMessageBox(_T("파일 지우기 성공"));
	}
}

void RightOption::OnChange_DataFormat()
{
	//클리한 파일을 자체포멧으로 변경한다
	std::string Name = ChangeToString(ClickItemName);
	std::string Path = ChangeToString(ClickAssetsPath);
	Path += "/";
	Path += Name;

	m_EditorManager->SetPath(Path, nullptr);
}

void RightOption::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case 1:
		//mLoading->SetUpdate(LoadAssetsCount());
		break;
	}
	
	CDialogEx::OnTimer(nIDEvent);
}

void RightOption::OnOpenExeFile_Button()
{
	//exe 파일을 실행시킨다
	wchar_t path[256] = { 0 };
	GetModuleFileName(NULL, path, 256);

	USES_CONVERSION;
	std::string str = W2A(path);
	std::size_t Start = 0;
	std::size_t End = str.rfind('\\');
	str = str.substr(Start, End);
	str += "\\GameClient.exe";
	CString FileName;
	FileName = str.c_str();

	STARTUPINFO Startupinfo = { 0 };
	PROCESS_INFORMATION processInfo;
	Startupinfo.cb = sizeof(STARTUPINFO);
	::CreateProcess
	(
		FileName,
		NULL, NULL, NULL,
		false, 0, NULL, NULL,
		&Startupinfo, &processInfo
	);
}

void RightOption::OnClickTap(NMHDR* pNMHDR, LRESULT* pResult)
{
	int num = Component_TapList.GetCurSel();

	TCHAR buffer[256] = { 0 };
	TC_ITEM data;
	data.mask = TCIF_TEXT;
	data.pszText = buffer;
	data.cchTextMax = 256;
	Component_TapList.GetItem(num, &data);

	CString ComponentName;
	ComponentName = buffer;

	if (ComponentName == L"Transform")
	{
		mTransform->ShowWindow(SW_SHOW);
		mAnimation->ShowWindow(SW_HIDE);
		mMeshFilter->ShowWindow(SW_HIDE);
		mPrticle->ShowWindow(SW_HIDE);
	}
	else if (ComponentName == L"Animation")
	{
		mTransform->ShowWindow(SW_HIDE);
		mAnimation->ShowWindow(SW_SHOW);
		mMeshFilter->ShowWindow(SW_HIDE);
		mPrticle->ShowWindow(SW_HIDE);
	}
	else if (ComponentName == L"MeshFilter")
	{
		mTransform->ShowWindow(SW_HIDE);
		mAnimation->ShowWindow(SW_HIDE);
		mMeshFilter->ShowWindow(SW_SHOW);
		mPrticle->ShowWindow(SW_HIDE);
	}
	else if (ComponentName == L"Particle")
	{
		mTransform->ShowWindow(SW_HIDE);
		mAnimation->ShowWindow(SW_HIDE);
		mMeshFilter->ShowWindow(SW_HIDE);
		mPrticle->ShowWindow(SW_SHOW);
	}
}

BOOL RightOption::PreTranslateMessage(MSG* pMsg)
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

void RightOption::OnSaveScene()
{	
	mScene->DoModal();
	std::string SaveName = ChangeToString(mScene->Name);
	std::string SavePath = "../Assets/Scene/";
	Demo::SaveScene(SavePath,SaveName);
	AfxMessageBox(L"저장 완료");
}

void RightOption::OnCreateParticle()
{
	GameObject* Obj = Demo::CreateParticle();
	HTREEITEM Top = HirearchyTree.InsertItem(ChangeToCString(Obj->Name));
	Create_Hirearchy_Item(Obj, Top);
}


void RightOption::OnCreateTerrain()
{
	Demo::CreateTerrain("");
}

LRESULT RightOption::OnUserFun(WPARAM wParam, LPARAM lparam)
{



	return LRESULT();
}


void RightOption::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (nFlags & (MK_SHIFT | MK_CONTROL))
	{
		
	}


	CDialogEx::OnMouseHWheel(nFlags, zDelta, pt);
}
