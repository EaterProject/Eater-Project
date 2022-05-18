#pragma once

#ifdef _WIN64
	#ifdef _DEBUG
		#pragma comment( lib, "./DirectXTK/lib/DirectXTK_x64d.lib" )
		#pragma comment( lib, "./DirectXTex/lib/DirectXTex_x64d.lib" )
	#else
		#pragma comment( lib, "./DirectXTK/lib/DirectXTK_x64r.lib" )
		#pragma comment( lib, "./DirectXTex/lib/DirectXTex_x64r.lib" )
	#endif
#endif

#include <wincodec.h>

#include "./DirectXTK/inc/ScreenGrab.h"
#include "./DirectXTK/inc/DDSTextureLoader.h"
#include "./DirectXTK/inc/WICTextureLoader.h"

#include "./DirectXTex/inc/DirectXTex.h"