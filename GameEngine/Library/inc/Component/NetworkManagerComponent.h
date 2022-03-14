#pragma once

#include "MainHeader.h"
#include "Component.h"

namespace flatbuffers
{
	class FlatBufferBuilder;
}


class NetworkManagerComponent: public Component
{
public:
	EATER_ENGINEDLL  NetworkManagerComponent();
	EATER_ENGINEDLL virtual ~NetworkManagerComponent();

	EATER_ENGINEDLL virtual void RECV(void* Data, int type) = 0;
	
	flatbuffers::FlatBufferBuilder* _Builder;
};