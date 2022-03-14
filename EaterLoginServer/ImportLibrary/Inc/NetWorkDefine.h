#pragma once

/// SETTING COUNT.
// SERVER
#define SERVER_OVERLAPPED_COUNT			10000	// �� �������� ����.
#define SERVER_MEMORYPOOL_COUNT			2000	// �޸�Ǯ ��� ����.
#define SERVER_TIME_EVENT_s			10	// Release ��� �� ���� �ð�(s ���� ����) �ֱ�� �������� ���.				
// CLIENT
#define CLIENT_THREAD_COUNT				1		// Ŭ���̾�Ʈ�� ������ ���� (Ŭ���̾�Ʈ�� ������� �����ϸ�.. 1������ ���!)
#define CLIENT_IOCP_THREAD_COUNT		2		// Ŭ���̾�Ʈ�� IOCP ������ ����
#define CLIENT_OVERLAPPED_COUNT			1000	// Ŭ���̾�Ʈ�� IOCP ������ ����

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
#define PACKET_TYPE_NONE		0				// ��Ŷ�� Ÿ���� �ƹ��͵� ���ǵ��� �ʾ��� ��

/// ���� API ���ǵ�.
#include "DHNetWorkAPIDefine.h"

/// ���� ���
// ��Ÿ
#include <tchar.h>

// ������/�Լ�����
#include <functional>
#include <thread>

// �޸𸮰���/�����
#include <memory>
#include <assert.h>

// �ڷᱸ��
#include <vector>
#include <list>
#include <map>
#include <concurrent_queue.h>
