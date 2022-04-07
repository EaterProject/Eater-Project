#pragma warning(disable: 4996)

#include "ProfilerDLL.h"
#include "Debugger.h"

#include <iostream>
#include <stdarg.h>
#include <tchar.h>
#include <iomanip>
#include <sstream>
#include <comdef.h>

const std::string Debugger::TOKEN::Error = "[ERROR] ";
const std::string Debugger::TOKEN::File  = "[FILE] ";
const std::string Debugger::TOKEN::Func  = "[FUNC] ";
const std::string Debugger::TOKEN::Line  = "[LINE] ";
const std::string Debugger::TOKEN::Enter = "\n";
const std::string Debugger::TOKEN::Tab	 = "\t";
const std::string Debugger::TOKEN::Space = " ";

LOG_DESC Debugger::g_Log;
HANDLE Debugger::g_Console;

Debugger::Debugger()
{

}

void Debugger::Create()
{
	AllocConsole();
	system("cls");
	g_Console = GetStdHandle(STD_OUTPUT_HANDLE);
	m_LogCount = 0;

	if ((_waccess(_T("./Log"), 0)) == -1) //여기에 LOG폴더가 없으면...
		CreateDirectory(_T("./Log"), NULL);

	g_Log = LOG_DESC();
	g_Log.LogName = "./Log/";
	time_t currentSec = time(NULL);
	tm* t = localtime(&currentSec);

	std::ostringstream oss;

	oss.clear();
	int year = t->tm_year + 1900;
	int mon = t->tm_mon + 1;
	int day = t->tm_mday;

	oss << std::setfill('0') << std::setw(2) << year << "." << mon / 10 << mon % 10 << "." << day / 10 << day % 10 << ".log";

	g_Log.LogName += oss.str();

	g_Log.LogFile = fopen(g_Log.LogName.c_str(), "a+");
}

void Debugger::TimerStart(PROFILE_OUTPUT outputType, const char* file, const char* func, int& line, int& totalFrame, std::string&& timerKey)
{
	// 해당 문자열 Hash 추출..
	Hash hashKey = std::hash<std::string>()(timerKey);

	// Hash Key 검색..
	m_NowKey = m_Timer.find(hashKey);

	// 만약 등록된 Key가 아니라면 등록..
	if (m_NowKey == m_Timer.end())
	{
		TIME_DESC timer;
		timer.KeyName = timerKey;
		timer.Location = GetFileName(file) + " " + std::string(func) + " - " + std::to_string(line);
		timer.TotalTime = std::chrono::seconds::zero();
		timer.OutputType = outputType;
		timer.NowFrame = totalFrame;
		timer.TotalFrame = totalFrame;

		m_NowKey = m_Timer.insert(std::pair<Hash, TIME_DESC>(hashKey, timer)).first;
	}

	// 현재 Timer Desc..
	TIME_DESC& nowTimer = m_NowKey->second;

	// Timer End 설정이 안되있을 경우..
	if (nowTimer.TotalTime == std::chrono::seconds::zero() && nowTimer.NowFrame < nowTimer.TotalFrame)
	{
		nowTimer.NowFrame = 500;
		Log(nowTimer.OutputType, "[ ERROR ] %s\n '%s' Timer의 'PROFILE_TIMER_END' 함수가 없습니다.\n", nowTimer.Location.c_str(), nowTimer.KeyName.c_str());
	}

	// 현재 시작 시간 설정..
	nowTimer.Start = std::chrono::system_clock::now();

	// 측정 프레임 카운트 감소..
	nowTimer.NowFrame--;
}

void Debugger::TimerEnd(std::string&& timerKey)
{
	// Hash Key 검색..
	Hash hashKey = std::hash<std::string>()(timerKey);

	m_NowKey = m_Timer.find(hashKey);

	// 등록된 Hash Key가 아니라면 처리하지 않음.. 사용자의 실수
	if (m_NowKey == m_Timer.end()) return;

	// 검색된 Timer 추출..
	TIME_DESC& nowTimer = m_NowKey->second;

	// 출력 전까지 시간 누적..
	nowTimer.TotalTime += std::chrono::system_clock::now() - nowTimer.Start;

	if (nowTimer.NowFrame <= 0)
	{
		// 해당 측정 결과 출력..
		Log(nowTimer.OutputType, 
			"-----------------------[ Timer ]-----------------------\n %s\n [ Locate ] \t\t: %s\n [ TotalFrame ] \t: %d\n [ TotalTime ] \t\t: %.6f sec\n [ AverageTime ] \t: %.6f sec\n-------------------------------------------------------\n\n",
			timerKey.c_str(),
			nowTimer.Location.c_str(),
			nowTimer.TotalFrame,
			nowTimer.TotalTime.count(),
			nowTimer.TotalTime.count() / (double)nowTimer.TotalFrame);

		// 측정이 끝난 Timer 제거..
		m_Timer.erase(m_NowKey);
	}
}

void Debugger::Log(PROFILE_OUTPUT& outputType, const char* message, ...)
{
	int length;
	va_list args;

	va_start(args, message);

	// 가변인자 문자열 길이 측정..
	length = _vscprintf(message, args) + 1;

	// 가변인자 문자열 변환..
	char* cBuf = new char[length];
	memset(cBuf, NULL, length);

	vsprintf(cBuf, message, args);

	va_end(args);

	// 해당 Log 출력..
	Log(outputType, 1, nullptr, cBuf, length);

	delete[] cBuf;
}

void Debugger::Log(PROFILE_OUTPUT& outputType, HRESULT result, const char* fileInfo, char* message, int length)
{
	std::string errMessage;

	if (FAILED(result))
	{
		_com_error error(result);
		std::wstring errMsg_w = error.ErrorMessage();

		int errMsg_Size = WideCharToMultiByte(CP_ACP, 0, errMsg_w.c_str(), -1, NULL, 0, NULL, NULL);

		char* errMsg_c = new char[errMsg_Size];
		memset(errMsg_c, NULL, errMsg_Size);

		WideCharToMultiByte(CP_ACP, 0, errMsg_w.c_str(), -1, errMsg_c, errMsg_Size, NULL, NULL);

		errMessage += TOKEN::Enter + TOKEN::Error;
		errMessage += errMsg_c;

		delete[] errMsg_c;
	}

	// 출력 타입에 따른 처리..
	switch (outputType)
	{
	case PROFILE_OUTPUT::LOG_FILE:
	{
		std::string output = GetTime();
		output += fileInfo;
		output += message;
		output += errMessage;
		output += "\n\n";

		fputs(output.c_str(), g_Log.LogFile);
	}
		break;
	case PROFILE_OUTPUT::CONSOLE:
	{
		std::string output = message;
		output += errMessage;
		output += TOKEN::Enter;

		WriteFile(g_Console, output.c_str(), (DWORD)(output.size()), NULL, NULL);

		//m_LogCount++;
		//if (m_LogCount > 30)
		//{
		//	system("cls");
		//	m_LogCount = 0;
		//}
	}
	break;
	case PROFILE_OUTPUT::VS_CODE:
	{
		std::string output;
		output += message;
		output += errMessage;
		output += "\n";

		OutputDebugStringA(output.c_str());
	}
	break;
	default:
		break;
	}
}

std::string Debugger::GetFileInfo(const char* file, const char* func, int& line)
{
	std::string outinfo;

	outinfo += TOKEN::File;
	outinfo += GetFileName(file) + TOKEN::Space;
	outinfo += TOKEN::Func;
	outinfo += func + TOKEN::Space;
	outinfo += TOKEN::Line;
	outinfo += std::to_string(line) + TOKEN::Enter;

	return outinfo;
}

std::string Debugger::GetTime()
{
	time_t currentSec = time(NULL);
	tm* t = localtime(&currentSec);

	std::ostringstream oss;

	oss.clear();
	int hour = t->tm_hour;
	int min = t->tm_min;
	int sec = t->tm_sec;

	oss << "[TIME] " << hour / 10 << hour % 10;
	oss << ":" << min / 10 << min % 10;
	oss << ":" << sec / 10 << sec % 10 << " ";

	return oss.str();
}

std::string Debugger::GetFileName(const char* file)
{
	std::string fileinfo = file;

	size_t fileStart = fileinfo.rfind('\\') + 1;
	size_t fileEnd = fileinfo.size();
	fileinfo = fileinfo.substr(fileStart, fileEnd);

	return fileinfo;
}
