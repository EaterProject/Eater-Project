// CustomDialog.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "CustomDialog.h"
#include "afxdialogex.h"


// CustomDialog 대화 상자

IMPLEMENT_DYNAMIC(CustomDialog, CDialogEx)

CustomDialog::CustomDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CUSTOM, pParent)
{

}

CustomDialog::CustomDialog(UINT nIDTemplate, CWnd* pParent)
	: CDialogEx(nIDTemplate, pParent)
{


}

CustomDialog::~CustomDialog()
{


}

void CustomDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

CString CustomDialog::ChangeToCString(float Data)
{
	//float -> Cstring
	CString Cstr = L"";
	std::string str = std::to_string(Data);
	Cstr = str.c_str();
	return Cstr;
}

CString CustomDialog::ChangeToCString(int Data)
{
	//int -> Cstring
	CString Cstr = L"";
	std::string str = std::to_string(Data);
	Cstr = str.c_str();
	return Cstr;
}

CString CustomDialog::ChangeToCString(std::string Data)
{
	//string -> Cstring
	CString Cstr = L"";
	Cstr = Data.c_str();
	return Cstr;
}

std::string CustomDialog::ChangeToString(CString Data)
{
	//Cstring -> string
	CT2CA convertedString = Data;
	return (std::string)convertedString;
}

std::string CustomDialog::ChangeToString(int Data)
{
	//int -> string
	return std::to_string(Data);
}

std::string CustomDialog::ChangeToString(float Data)
{
	//float -> string
	return std::to_string(Data);
}

float CustomDialog::ChangeToFloat(std::string Data)
{
	//string -> float
	if (Data == "")
	{
		return 0;
	}
	else
	{
		return stof(Data);
	}
}

float CustomDialog::ChangeToFloat(CString Data)
{
	//Cstring -> float
	if (Data == L"")
	{
		return 0;
	}
	else
	{
		std::string str = ChangeToString(Data);
		return stof(str);
	}
}

int CustomDialog::ChangeToInt(CString Data)
{
	//Cstring -> int
	if (Data == L"")
	{
		return 0;
	}
	else
	{
		std::string str = ChangeToString(Data);
		return stoi(str);
	}
}

int CustomDialog::ChangeToInt(std::string Data)
{
	//string -> int
	if (Data == "")
	{
		return 0;
	}
	else
	{
		return stoi(Data);
	}
}
int CustomDialog::GetFileNameType(std::string Name)
{
	int Type = 0;
	std::size_t Start = Name.rfind('.') + 1;
	std::size_t End = Name.length() - Start;
	Name = Name.substr(Start, End);
	if (Name == "Eater")
	{
		Type = EATER;
	}
	else if (Name == "fbx")
	{
		Type = FBX;
	}
	else if (Name == "png")
	{
		Type = PNG;
	}
	else if (Name == "dds")
	{
		Type = DDS;
	}
	else if (Name == "Scene")
	{
		Type = SCENE;
	}
	else if (Name == "Emat")
	{
		Type = EMAT;
	}
	else if (Name == "Emesh")
	{
		Type = EMESH;
	}
	return Type;
}

int CustomDialog::GetFileNameType(CString Name)
{
	std::string str = ChangeToString(Name);
	return GetFileNameType(str);
}


bool CustomDialog::DropRect(RECT& rect)
{
	POINT point;
	GetCursorPos(&point);
	if (rect.left <= point.x && rect.right >= point.x &&
		rect.top <= point.y && rect.bottom >= point.y)
	{
		return true;
	}
	else
	{
		return false;
	}
	return false;
}

float CustomDialog::Lerp(float prev, float next, float time)
{
	return ((prev * (1.0f - time)) + (next * time));
}


BEGIN_MESSAGE_MAP(CustomDialog, CDialogEx)
END_MESSAGE_MAP()
// CustomDialog 메시지 처리기
