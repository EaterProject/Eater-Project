#pragma once

/// SETTING COUNT.
// SERVER
#define SERVER_OVERLAPPED_COUNT			10000	// 총 오버랩드 개수.
#define SERVER_MEMORYPOOL_COUNT			2000	// 메모리풀 사용 개수.
#define SERVER_TIME_EVENT_s			10	// Release 모드 시 일정 시간(s 단위 설정) 주기로 서버정보 출력.				
// CLIENT
#define CLIENT_THREAD_COUNT				1		// 클라이언트의 쓰레드 개수 (클라이언트의 쓰레드는 웬만하면.. 1개여도 충분!)
#define CLIENT_IOCP_THREAD_COUNT		2		// 클라이언트의 IOCP 쓰레드 개수
#define CLIENT_OVERLAPPED_COUNT			1000	// 클라이언트의 IOCP 쓰레드 개수

/// BUFSIZE
#define OVERLAPPED_BUFIZE		2048
#define MAX_PACKET_SIZE			20480
#define PACKET_BUFSIZE			1024
#define ERROR_MSG_BUFIZE		128

/// LOGIC DEFINE
#define LOGIC_SUCCESS			1
#define LOGIC_FAIL				0

/// STATIC SIZE DEFINE
#define IP_SIZE					16
#define PACKET_TYPE_NONE		0				// 패킷의 타입이 아무것도 정의되지 않았을 때

/// 공용 API 정의들.
#include "DHNetWorkAPIDefine.h"

/// 공용 헤더
// 기타
#include <tchar.h>

// 쓰레드/함수관련
#include <functional>
#include <thread>

// 메모리관리/디버깅
#include <memory>
#include <assert.h>

// 자료구조
#include <vector>
#include <list>
#include <map>
#include <concurrent_queue.h>
