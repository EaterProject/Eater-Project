// FileOption.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "FileOption.h"
#include "afxdialogex.h"
#include "EditorData.h"
#include "EditorManager.h"
#include "RightOption.h"
#include "GrobalFunction.h"


// FileOption 대화 상자

IMPLEMENT_DYNAMIC(FileOption, CDialogEx)

FileOption::FileOption(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILE_OPTION, pParent)
{

}

FileOption::~FileOption()
{

}

void FileOption::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, FileName_Edit);
	DDX_Control(pDX, IDC_EDIT6, PosX);
	DDX_Control(pDX, IDC_EDIT7, PosY);
	DDX_Control(pDX, IDC_EDIT8, PosZ);
	DDX_Control(pDX, IDC_EDIT16, SclX);
	DDX_Control(pDX, IDC_EDIT17, SclY);
	DDX_Control(pDX, IDC_EDIT18, SclZ);
	DDX_Control(pDX, IDC_EDIT9, RotX);
	DDX_Control(pDX, IDC_EDIT10, RotY);
	DDX_Control(pDX, IDC_EDIT15, RotZ);


	DDX_Control(pDX, IDC_EDIT22, PosX_Bone);
	DDX_Control(pDX, IDC_EDIT23, PosY_Bone);
	DDX_Control(pDX, IDC_EDIT24, PosZ_Bone);
	DDX_Control(pDX, IDC_EDIT25, RotX_Bone);
	DDX_Control(pDX, IDC_EDIT26, RotY_Bone);
	DDX_Control(pDX, IDC_EDIT27, RotZ_Bone);
	DDX_Control(pDX, IDC_EDIT28, SclX_Bone);
	DDX_Control(pDX, IDC_EDIT29, SclY_Bone);
	DDX_Control(pDX, IDC_EDIT30, SclZ_Bone);

	ChangeDataFormat();
	OnEater(false);
	DDX_Control(pDX, IDC_EDIT11, FileType_Edit);
	DDX_Control(pDX, IDC_RADIO2, Eater_ON);
}

void FileOption::OnEater(bool On)
{
	if (On == true)
	{
		CButton* pButton;
		pButton = (CButton*)GetDlgItem(IDC_RADIO2);
		pButton->SetCheck(true);

		GetDlgItem(IDC_EDIT22)->SendMessage(EM_SETREADONLY, false, 0);
		GetDlgItem(IDC_EDIT23)->SendMessage(EM_SETREADONLY, false, 0);
		GetDlgItem(IDC_EDIT24)->SendMessage(EM_SETREADONLY, false, 0);
		GetDlgItem(IDC_EDIT25)->SendMessage(EM_SETREADONLY, false, 0);
		GetDlgItem(IDC_EDIT26)->SendMessage(EM_SETREADONLY, false, 0);
		GetDlgItem(IDC_EDIT27)->SendMessage(EM_SETREADONLY, false, 0);
		GetDlgItem(IDC_EDIT28)->SendMessage(EM_SETREADONLY, false, 0);
		GetDlgItem(IDC_EDIT29)->SendMessage(EM_SETREADONLY, false, 0);
		GetDlgItem(IDC_EDIT30)->SendMessage(EM_SETREADONLY, false, 0);
		//Local
		GetDlgItem(IDC_EDIT6)->SendMessage(EM_SETREADONLY,	false, 0);
		GetDlgItem(IDC_EDIT7)->SendMessage(EM_SETREADONLY,	false, 0);
		GetDlgItem(IDC_EDIT8)->SendMessage(EM_SETREADONLY,	false, 0);
		GetDlgItem(IDC_EDIT16)->SendMessage(EM_SETREADONLY, false, 0);
		GetDlgItem(IDC_EDIT17)->SendMessage(EM_SETREADONLY, false, 0);
		GetDlgItem(IDC_EDIT18)->SendMessage(EM_SETREADONLY, false, 0);
		GetDlgItem(IDC_EDIT9)->SendMessage(EM_SETREADONLY,	false, 0);
		GetDlgItem(IDC_EDIT10)->SendMessage(EM_SETREADONLY, false, 0);
		GetDlgItem(IDC_EDIT15)->SendMessage(EM_SETREADONLY, false, 0);
	}
	else 
	{
		CButton* pButton;
		pButton = (CButton*)GetDlgItem(IDC_RADIO2);
		pButton->SetCheck(false);

		GetDlgItem(IDC_EDIT22)->SendMessage(EM_SETREADONLY, true, 0);
		GetDlgItem(IDC_EDIT23)->SendMessage(EM_SETREADONLY, true, 0);
		GetDlgItem(IDC_EDIT24)->SendMessage(EM_SETREADONLY, true, 0);
		GetDlgItem(IDC_EDIT25)->SendMessage(EM_SETREADONLY, true, 0);
		GetDlgItem(IDC_EDIT26)->SendMessage(EM_SETREADONLY, true, 0);
		GetDlgItem(IDC_EDIT27)->SendMessage(EM_SETREADONLY, true, 0);
		GetDlgItem(IDC_EDIT28)->SendMessage(EM_SETREADONLY, true, 0);
		GetDlgItem(IDC_EDIT29)->SendMessage(EM_SETREADONLY, true, 0);
		GetDlgItem(IDC_EDIT30)->SendMessage(EM_SETREADONLY, true, 0);

		GetDlgItem(IDC_EDIT6)-> SendMessage(EM_SETREADONLY, true, 0);
		GetDlgItem(IDC_EDIT7)-> SendMessage(EM_SETREADONLY, true, 0);
		GetDlgItem(IDC_EDIT8)-> SendMessage(EM_SETREADONLY, true, 0);
		GetDlgItem(IDC_EDIT16)->SendMessage(EM_SETREADONLY, true, 0);
		GetDlgItem(IDC_EDIT17)->SendMessage(EM_SETREADONLY, true, 0);
		GetDlgItem(IDC_EDIT18)->SendMessage(EM_SETREADONLY, true, 0);
		GetDlgItem(IDC_EDIT9)-> SendMessage(EM_SETREADONLY, true, 0);
		GetDlgItem(IDC_EDIT10)->SendMessage(EM_SETREADONLY, true, 0);
		GetDlgItem(IDC_EDIT15)->SendMessage(EM_SETREADONLY, true, 0);
	}
}

void FileOption::ChickDrag(CPoint point)
{
	CRect rect;
	GetWindowRect(rect);

	

	if (rect.left	<= point.x &&
		rect.right	>= point.x &&
		rect.top	<= point.y &&
		rect.bottom >= point.y)
	{
		CString DragName = RightOption::GetThis()->DragItemName;
		int Type = GetFileNameType(DragName);
		std::string Path;
		switch (Type)
		{
		case DDS:
			AfxMessageBox(L"DDS");
			break;
		case EATER:
			Path = "../Assets/Model/ModelData/" + ChangeToString(DragName);
			mEditor->OpenEaterFile(Path,EATER);

			DragFileOpen_Eater(DragName);
			OnEater(true);
			break;
		case EMAT:
			Path = "../Assets/Texture/ModelTexture/" + ChangeToString(DragName);
			mEditor->OpenEaterFile(Path,EMAT);

			DragFileOpen_Material(DragName);
			OnEater(false);
			break;
		case EMESH:
			DragFileOpen_Mesh(DragName);
			OnEater(false);
			break;
		}
	}
}

void FileOption::Initialize(EditorManager* _Editor)
{
	mEditor = _Editor;
}


BEGIN_MESSAGE_MAP(FileOption, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &FileOption::OnChange_Button)
	ON_BN_CLICKED(IDC_BUTTON2, &FileOption::OnNoChange_Button)
	ON_BN_CLICKED(IDC_BUTTON8, &FileOption::OnScaleDown_Button)
	ON_CBN_SELCHANGE(IDC_COMBO1, &FileOption::OnCbnSelchangeCombo1)
	ON_MESSAGE(M_MSG_FILE_CHANGE, &FileOption::OnUserFunc)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


void FileOption::ChangeDataFormat()
{
	std::string Name = ChoiceMeshName;
	Name = Name.substr(0, Name.rfind('.'));
	FileName_Edit.SetWindowTextW(ChangeToCString(Name));

	PosX.SetWindowTextW(L"0.00");
	PosY.SetWindowTextW(L"0.00");
	PosZ.SetWindowTextW(L"0.00");
	RotX.SetWindowTextW(L"0.00");
	RotY.SetWindowTextW(L"0.00");
	RotZ.SetWindowTextW(L"0.00");
	SclX.SetWindowTextW(L"1.00");
	SclY.SetWindowTextW(L"1.00");
	SclZ.SetWindowTextW(L"1.00");


	PosX_Bone.SetWindowTextW(L"0");
	PosY_Bone.SetWindowTextW(L"0");
	PosZ_Bone.SetWindowTextW(L"0");

	RotX_Bone.SetWindowTextW(L"0");
	RotY_Bone.SetWindowTextW(L"0");
	RotZ_Bone.SetWindowTextW(L"0");

	SclX_Bone.SetWindowTextW(L"1");
	SclY_Bone.SetWindowTextW(L"1");
	SclZ_Bone.SetWindowTextW(L"1");
}

void FileOption::DragFileOpen_Material(CString Name)
{
	FileName_Edit.SetWindowTextW(Name);
	FileType_Edit.SetWindowTextW(L"Material 파일");
}

void FileOption::DragFileOpen_Buffer(CString Name)
{
	FileName_Edit.SetWindowTextW(Name);
	FileType_Edit.SetWindowTextW(L"Buffer 파일");
}

void FileOption::DragFileOpen_Mesh(CString Name)
{
	FileName_Edit.SetWindowTextW(Name);
	FileType_Edit.SetWindowTextW(L"Mesh 파일");
}

void FileOption::DragFileOpen_Eater(CString Name)
{
	FileName_Edit.SetWindowTextW(Name);
	FileType_Edit.SetWindowTextW(L"Eater 파일");
}

void FileOption::SetName(std::string Name, std::string Path)
{
	ChoiceMeshName = Name;
	ChoiceMeshPath = Path;
}

void FileOption::OnTimer(UINT_PTR nIDEvent)
{

	CDialogEx::OnTimer(nIDEvent);
}

void FileOption::OnChange_Button()
{
	MeshOption Data;
	std::string FilePath = ChoiceMeshPath;
	FilePath += "/";
	FilePath += ChoiceMeshName;
	CString Number;

	//FileName_Edit.GetWindowTextW(Number);
	//Data.Name		= GetDataString(Number);
	//Data.SaveType	= FileType_Combo.GetCurSel();
	//
	//PosX.GetWindowTextW(Number);
	//Data.Position.x = std::stof(GetDataString(Number));
	//PosY.GetWindowTextW(Number);
	//Data.Position.y = std::stof(GetDataString(Number));
	//PosZ.GetWindowTextW(Number);
	//Data.Position.z = std::stof(GetDataString(Number));
	//
	//RotX.GetWindowTextW(Number);
	//Data.Rotation.x = std::stof(GetDataString(Number));
	//RotY.GetWindowTextW(Number);
	//Data.Rotation.y = std::stof(GetDataString(Number));
	//RotZ.GetWindowTextW(Number);
	//Data.Rotation.z = std::stof(GetDataString(Number));
	//
	//
	//SclX.GetWindowTextW(Number);
	//Data.Scale.x = std::stof(GetDataString(Number));
	//SclY.GetWindowTextW(Number);
	//Data.Scale.y = std::stof(GetDataString(Number));
	//SclZ.GetWindowTextW(Number);
	//Data.Scale.z = std::stof(GetDataString(Number));
	//
	//PosX_Bone.GetWindowTextW(Number);
	//Data.Position_Bone.x = std::stof(GetDataString(Number));
	//PosY_Bone.GetWindowTextW(Number);
	//Data.Position_Bone.y = std::stof(GetDataString(Number));
	//PosZ_Bone.GetWindowTextW(Number);
	//Data.Position_Bone.z = std::stof(GetDataString(Number));
	//
	//RotX_Bone.GetWindowTextW(Number);
	//Data.Rotation_Bone.x = std::stof(GetDataString(Number));
	//RotY_Bone.GetWindowTextW(Number);
	//Data.Rotation_Bone.y = std::stof(GetDataString(Number));
	//RotZ_Bone.GetWindowTextW(Number);
	//Data.Rotation_Bone.z = std::stof(GetDataString(Number));
	//
	//
	//SclX_Bone.GetWindowTextW(Number);
	//Data.Scale_Bone.x = std::stof(GetDataString(Number));
	//SclY_Bone.GetWindowTextW(Number);
	//Data.Scale_Bone.y = std::stof(GetDataString(Number));
	//SclZ_Bone.GetWindowTextW(Number);
	//Data.Scale_Bone.z = std::stof(GetDataString(Number));
	//
	//Data.DinamicObject = DinamicButton.GetCheck();
	//
	//mEditor->SetPath(FilePath, &Data);
	//::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
}


void FileOption::OnNoChange_Button()
{
	::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
}


void FileOption::OnScaleDown_Button()
{
	SclX.SetWindowTextW(L"0.01");
	SclY.SetWindowTextW(L"0.01");
	SclZ.SetWindowTextW(L"0.01");
}

void FileOption::OnCbnSelchangeCombo1()
{
	//int Type = FileType_Combo.GetCurSel();
	//switch (Type)
	//{
	//case 0:
	//	OnBoneWorld(false);
	//	break;
	//case 1:
	//	OnBoneWorld(true);
	//	break;
	//case 2:
	//	OnBoneWorld(false);
	//	break;
	//case 3:
	//	OnBoneWorld(false);
	//	break;
	//}
}

LRESULT FileOption::OnUserFunc(WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}


void FileOption::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (RightOption::GetThis()->isDrag == true)
	{
		RightOption::GetThis()->isDrag = false;
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}
