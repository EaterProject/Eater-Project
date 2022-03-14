#pragma once
#include "EaterEngineDLL.h"
#include "Component.h"

class NetworkManager;
struct NetworkData;
struct S2C_Packet;
struct C2S_Packet;

namespace flatbuffers
{
	class FlatBufferBuilder;
}


///��Ʈ��ũ ���õ� ���۳�Ʈ ���� ��Ŭ������ ��ӹ޾� ���۵�
class NetworkComponent : public Component
{
public:
	EATER_ENGINEDLL NetworkComponent();
	EATER_ENGINEDLL ~NetworkComponent();


	flatbuffers::FlatBufferBuilder* _Builder;
};


