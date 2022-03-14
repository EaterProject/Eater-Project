#pragma once

#ifdef ENGINE_INTERFACE
#define EATER_ENGINEDLL __declspec(dllexport)
#else
#define EATER_ENGINEDLL __declspec(dllimport)

	#ifdef _DEBUG
	#pragma comment(lib,"EaterEngine_x64d")
	#else
	#pragma comment(lib,"EaterEngine_x64r")
	#endif

#endif

#include "SimpleMath.h"
using namespace DirectX;
using namespace SimpleMath;
