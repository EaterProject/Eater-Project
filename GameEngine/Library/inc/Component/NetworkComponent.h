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


///네트워크 관련된 컨퍼넌트 들은 이클래스를 상속받아 제작됨
class NetworkComponent : public Component
{
public:
	EATER_ENGINEDLL NetworkComponent();
	EATER_ENGINEDLL ~NetworkComponent();


	flatbuffers::FlatBufferBuilder* _Builder;
};


