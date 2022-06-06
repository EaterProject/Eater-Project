#include "E_interface.h"

E_Interface::E_Interface()
{

}

E_Interface::~E_Interface()
{

}

void E_Interface::SetFileName(std::string mPath)
{
	FileName = GetFileName(mPath);
	FileType = GetFileType(mPath);
	FilePath = GetFilePath(mPath);
}

std::string E_Interface::GetFileName(std::string& mPath)
{
	size_t start	= mPath.rfind("/") + 1;
	size_t end		= mPath.rfind(".") - start;
	return mPath.substr(start, end);
}

std::string E_Interface::GetFileType(std::string& mPath)
{
	size_t start	= mPath.rfind(".")+1;
	size_t end		= mPath.length() - start;
	std::string Type = mPath.substr(start, end);
	std::string Point;
	
	//만약 점이 었다면
	if (Type.find('.') == std::string::npos)
	{
		Point += "." + Type;
	}
	else
	{
		Point = Type;
	}
	return Point;
}

std::string E_Interface::GetFilePath(std::string& mPath)
{
	size_t start	= 0;
	size_t end		= mPath.rfind("/")+1;
	return mPath.substr(start, end);
}


