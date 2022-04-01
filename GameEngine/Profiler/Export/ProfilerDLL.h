#pragma once

#ifdef PROFILER_EXPORT
	#define PROFILER_DLL		__declspec(dllexport)
#else
	#define PROFILER_DLL		__declspec(dllimport)		

	#ifdef _WIN64
		#ifdef _DEBUG
			#pragma comment(lib, "Profiler_x64d")		// 64bit + Debug
		#else
			#pragma comment(lib, "Profiler_x64r")		// 64bit
		#endif
	#else
		#ifdef _DEBUG
			#pragma comment(lib, "Profiler_x86d")		// 32bit + Debug
		#else
			#pragma comment(lib, "Profiler_x86r")		// 32bit
		#endif
	#endif
#endif

enum class DEBUG_OUTPUT
{
	LOG_FILE,	// Log File 출력
	CONSOLE,	// Console 출력
	VS_CODE		// Visual Studio Code 출력
};