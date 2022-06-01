#pragma once

#ifdef EATER_INTERFACE
#define FORMAT_PARSER_DLL __declspec(dllexport)
#else
#define FORMAT_PARSER_DLL __declspec(dllimport)

	#ifdef _DEBUG
	#pragma comment(lib,"EaterParser_x64d")
	#else
	#pragma comment(lib,"EaterParser_x64r")
	#endif

#endif
#include <string>