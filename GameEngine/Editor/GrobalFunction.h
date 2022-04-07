#pragma once
#include <string>
#include <atlstr.h>

#define FBX		0
#define PNG		1
#define EATER	2
#define DDS		3
#define SCENE	4
#define PREFAB	5
#define EMAT	6
#define EMESH	7


inline CString ChangeToCString(float Data);			//float		-> Cstring
inline CString ChangeToCString(int Data);			//int		-> Cstring
inline CString ChangeToCString(std::string Data);	//string	-> Cstring
inline std::string ChangeToString(CString Data);	//Cstring	-> string
inline std::string ChangeToString(int Data);		//int		-> string
inline std::string ChangeToString(float Data);		//float		-> string
inline float ChangeToFloat(std::string Data);		//string	-> float
inline float ChangeToFloat(CString Data);			//Cstring	-> float
inline int ChangeToInt(CString Data);				//Cstring	-> int
inline int ChangeToInt(std::string Data);			//string	-> int

//파일의 타입을 가져온다 (.png , .fbx)
inline int GetFileNameType(std::string Name);
inline int GetFileNameType(CString Name);


CString ChangeToCString(float Data)
{
	//float -> Cstring
	CString Cstr = L"";
	std::string str = std::to_string(Data);
	Cstr = str.c_str();
	return Cstr;
}

CString ChangeToCString(int Data)
{
	//int -> Cstring
	CString Cstr = L"";
	std::string str = std::to_string(Data);
	Cstr = str.c_str();
	return Cstr;
}

CString ChangeToCString(std::string Data)
{
	//string -> Cstring
	CString Cstr = L"";
	Cstr = Data.c_str();
	return Cstr;
}

std::string ChangeToString(CString Data)
{
	//Cstring -> string
	CT2CA convertedString = Data;
	return (std::string)convertedString;
}

std::string ChangeToString(int Data)
{
	//int -> string
	return std::to_string(Data);
}

std::string ChangeToString(float Data)
{
	//float -> string
	return std::to_string(Data);
}

float ChangeToFloat(std::string Data)
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

float ChangeToFloat(CString Data)
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

int ChangeToInt(CString Data)
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

int ChangeToInt(std::string Data)
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

inline int GetFileNameType(std::string Name)
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

inline int GetFileNameType(CString Name)
{
	std::string str = ChangeToString(Name);
	return GetFileNameType(str);
}


