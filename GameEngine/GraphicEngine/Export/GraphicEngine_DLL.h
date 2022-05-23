#pragma once

#ifdef GRAPHIC_ENGINE_EXPORT
	#define GRAPHIC_ENGINE_DLL		__declspec(dllexport)
#else
	#define GRAPHIC_ENGINE_DLL		__declspec(dllimport)		

	#ifdef _WIN64
		#ifdef _DEBUG
			#pragma comment(lib, "GraphicEngine_x64d")		// 64bit + Debug
		#else
			#pragma comment(lib, "GraphicEngine_x64r")		// 64bit
		#endif
	#else
		#ifdef _DEBUG
			#pragma comment(lib, "GraphicEngine_x86d")		// 32bit + Debug
		#else
			#pragma comment(lib, "GraphicEngine_x86r")		// 32bit
		#endif
	#endif
#endif
 