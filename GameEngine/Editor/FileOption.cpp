// FileOption.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "FileOption.h"
#include "afxdialogex.h"
#include "EditorData.h"
#include "EditorManager.h"


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
	DDX_Control(pDX, IDC_EDIT2, FileName_Edit);
	DDX_Control(pDX, IDC_COMBO1, FileType_Combo);
	DDX_Control(pDX, IDC_EDIT6, PosX);
	DDX_Control(pDX, IDC_EDIT7, PosY);
	DDX_Control(pDX, IDC_EDIT8, PosZ);
	DDX_Control(pDX, IDC_EDIT16, SclX);
	DDX_Control(pDX, IDC_EDIT17, SclY);
	DDX_Control(pDX, IDC_EDIT18, SclZ);
	DDX_Control(pDX, IDC_EDIT9, RotX);
	DDX_Control(pDX, IDC_EDIT10, RotY);
	DDX_Control(pDX, IDC_EDIT15, RotZ);

	ChangeDataFormat();
	FileType_Combo.AddString(_T("1_BaseMesh"));
	FileType_Combo.AddString(_T("2_SkinMesh"));
	FileType_Combo.AddString(_T("3_TerrainMesh"));
	FileType_Combo.AddString(_T("4_Animation"));
	FileType_Combo.SetCurSel(0);

	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, DinamicButton);
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
	ON_CBN_EDITUPDATE(IDC_COMBO1, &FileOption::OnCbnEditupdateCombo1)
END_MESSAGE_MAP()


void FileOption::ChangeDataFormat()
{
	std::string Name = ChoiceMeshName;
	Name = Name.substr(0, Name.rfind('.'));
	FileName_Edit.SetWindowTextW(GetDataCString(Name));

	PosX.SetWindowTextW(L"0.00");
	PosY.SetWindowTextW(L"0.00");
	PosZ.SetWindowTextW(L"0.00");
	RotX.SetWindowTextW(L"0.00");
	RotY.SetWindowTextW(L"0.00");
	RotZ.SetWindowTextW(L"0.00");
	SclX.SetWindowTextW(L"1.00");
	SclY.SetWindowTextW(L"1.00");
	SclZ.SetWindowTextW(L"1.00");
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

	FileName_Edit.GetWindowTextW(Number);
	Data.Name		= GetDataString(Number);
	Data.SaveType	= FileType_Combo.GetCurSel();

	PosX.GetWindowTextW(Number);
	Data.Position.x = std::stof(GetDataString(Number));
	PosY.GetWindowTextW(Number);
	Data.Position.y = std::stof(GetDataString(Number));
	PosZ.GetWindowTextW(Number);
	Data.Position.z = std::stof(GetDataString(Number));

	RotX.GetWindowTextW(Number);
	Data.Rotation.x = std::stof(GetDataString(Number));
	RotY.GetWindowTextW(Number);
	Data.Rotation.y = std::stof(GetDataString(Number));
	RotZ.GetWindowTextW(Number);
	Data.Rotation.z = std::stof(GetDataString(Number));


	SclX.GetWindowTextW(Number);
	Data.Scale.x = std::stof(GetDataString(Number));
	SclY.GetWindowTextW(Number);
	Data.Scale.y = std::stof(GetDataString(Number));
	SclZ.GetWindowTextW(Number);
	Data.Scale.z = std::stof(GetDataString(Number));

	Data.DinamicObject = DinamicButton.GetCheck();
	
	mEditor->SetPath(FilePath, &Data);
	::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
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


void FileOption::OnCbnEditupdateCombo1()
{
	int num = 0;
}
