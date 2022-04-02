#pragma warning(disable: 4996)

#include "ProfilerDLL.h"
#include "Debugger.h"
#include "Timer.h"
#include "Logger.h"

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

Debugger::Debugger()
{
	AllocConsole();
	system("cls");
	m_Console = GetStdHandle(STD_OUTPUT_HANDLE);
	m_LogCount = 0;

	if ((_waccess(_T("./Log"), 0)) == -1) //���⿡ LOG������ ������...
		CreateDirectory(_T("./Log"), NULL);

	m_Log.LogName = "./Log/";
	time_t currentSec = time(NULL);
	tm* t = localtime(&currentSec);

	std::ostringstream oss;

	oss.clear();
	int year = t->tm_year + 1900;
	int mon = t->tm_mon + 1;
	int day = t->tm_mday;

	oss << std::setfill('0') << std::setw(2) << year << "." << mon / 10 << mon % 10 << "." << day / 10 << day % 10 << ".log";
	
	m_Log.LogName += oss.str();

	m_Log.LogFile = fopen(m_Log.LogName.c_str(), "a+");
}

void Debugger::TimerStart(PROFILE_OUTPUT outputType, const char* func, int& line, int& totalFrame, std::string&& timerKey)
{
	// �ش� ���ڿ� Hash ����..
	Hash hashKey = std::hash<std::string>()(timerKey);

	// Hash Key �˻�..
	m_NowKey = m_Timer.find(hashKey);

	// ���� ��ϵ� Key�� �ƴ϶�� ���..
	if (m_NowKey == m_Timer.end())
	{
		TIME_DESC timer;
		timer.Location = std::string(func) + " - " + std::to_string(line);
		timer.TotalTime = std::chrono::seconds::zero();
		timer.OutputType = outputType;
		timer.NowFrame = totalFrame;
		timer.TotalFrame = totalFrame;

		m_NowKey = m_Timer.insert(std::pair<Hash, TIME_DESC>(hashKey, timer)).first;
	}

	// ���� ���� �ð� ����..
	m_NowKey->second.Start = std::chrono::system_clock::now();

	// ���� ������ ī��Ʈ ����..
	m_NowKey->second.NowFrame--;
}

void Debugger::TimerEnd(std::string&& timerKey)
{
	// Hash Key �˻�..
	Hash hashKey = std::hash<std::string>()(timerKey);

	m_NowKey = m_Timer.find(hashKey);

	// ��ϵ� Hash Key�� �ƴ϶�� ó������ ����.. ������� �Ǽ�
	if (m_NowKey == m_Timer.end()) return;

	// �˻��� Timer ����..
	TIME_DESC& nowTimer = m_NowKey->second;

	// ��� ������ �ð� ����..
	nowTimer.TotalTime += std::chrono::system_clock::now() - nowTimer.Start;

	if (nowTimer.NowFrame <= 0)
	{
		// �ش� ���� ��� ���..
		Log(nowTimer.OutputType, 
			"-----------------------[Timer]-----------------------\n %s\n [Locate] \t: %s\n [TotalFrame] \t: %d\n [TotalTime] \t: %.6f sec\n [AverageTime] \t: %.6f sec\n-----------------------------------------------------\n\n",
			timerKey.c_str(),
			nowTimer.Location.c_str(),
			nowTimer.TotalFrame,
			nowTimer.TotalTime.count(),
			nowTimer.TotalTime.count() / (double)nowTimer.TotalFrame);

		// ������ ���� Timer ����..
		m_Timer.erase(m_NowKey);
	}
}

void Debugger::Log(PROFILE_OUTPUT& outputType, const char* message, ...)
{
	int length;
	va_list args;

	va_start(args, message);

	// �������� ���ڿ� ���� ����..
	length = _vscprintf(message, args) + 1;

	// �������� ���ڿ� ��ȯ..
	char* cBuf = new char[length];
	memset(cBuf, NULL, length);

	vsprintf(cBuf, message, args);

	va_end(args);

	// �ش� Log ���..
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

	// ��� Ÿ�Կ� ���� ó��..
	switch (outputType)
	{
	case PROFILE_OUTPUT::LOG_FILE:
	{
		std::string output = GetTime();
		output += fileInfo;
		output += message;
		output += errMessage;
		output += "\n\n";

		fputs(output.c_str(), m_Log.LogFile);
	}
		break;
	case PROFILE_OUTPUT::CONSOLE:
	{
		std::string output = message;
		output += errMessage;
		output += TOKEN::Enter;

		WriteFile(m_Console, output.c_str(), (DWORD)(output.size()), NULL, NULL);

		m_LogCount++;
		if (m_LogCount > 30)
		{
			system("cls");
			m_LogCount = 0;
		}
	}
	break;
	case PROFILE_OUTPUT::VS_CODE:
	{
		std::string output = GetTime();
		output += fileInfo;
		output += message;
		output += errMessage;
		output += "\n\n";

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
	std::string fileinfo = file;

	size_t fileStart = fileinfo.rfind('\\') + 1;
	size_t fileEnd = fileinfo.size();
	fileinfo = fileinfo.substr(fileStart, fileEnd);

	outinfo += TOKEN::File;
	outinfo += fileinfo + TOKEN::Space;
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
