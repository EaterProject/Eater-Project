#pragma once
#include "ProfilerDLL.h"

/// 2022/04/02 22:26
/// SeoKyuHwang
///
/// # Profiler Header
///
/// 1. Console, VS_Output, Log_File ���·� Profiling ����
/// 2. Profile Output Type
///  - LOG_FILE : �ش� ��¥ Log File�� �̾ Log ���
///  - CONSOLE : Console â�� �ش� Log ���
///  - VS_CODE : Visual Studio ��� â�� Log ���

#if defined(DEBUG) || defined(_DEBUG)
// Log Profiling
// # �ش� Log Message ���.
// #Prameters
//  1. output	: PROFILE_OUTPUT Type
//  2. message	: Log Message
#define PROFILE_LOG(output, message, ...)		Log(output, __FILE__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)

// Timer Start Profiling
// # �ش� Key Timer ���� ����.
// # Timer Log ��µ� ���� Frame ����.
// #Prameters
//  1. output	: PROFILE_OUTPUT Type
//  2. key		: Timer Key Name
//  3. frame	: Output Timer Total Frame Rate
#define PROFILE_TIMER_START(output, key, frame)	TimerStart(output, __FUNCTION__, __LINE__, key, frame);

// - Timer Start Profiling
// # �ش� Key Timer ���� ����.
// # ������ Total Frame�� �����ϸ� ���.
// #Prameters
//  1. key		: Timer Key Name
#define PROFILE_TIMER_END(key)					TimerEnd(key);
#else
#define PROFILE_LOG(output, message, ...)
#define PROFILE_TIMER_START(output, key, frame)
#define PROFILE_TIMER_END(key)					
#endif

#if defined(DEBUG) || defined(_DEBUG)
PROFILER_DLL void Log(PROFILE_OUTPUT outputType, const char* file, const char* func, int&& line, const char* message, ...);
PROFILER_DLL void TimerStart(PROFILE_OUTPUT outputType, const char* func, int&& line, const char* timeKey, int&& totalFrame);
PROFILER_DLL void TimerEnd(const char* key);
#endif