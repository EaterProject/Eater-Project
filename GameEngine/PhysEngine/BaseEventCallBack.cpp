#include "PxPhysicsAPI.h"
#include "BaseEventCallBack.h"
#include <algorithm>
#include "PhysData.h"
BaseEventCallBack::BaseEventCallBack()
{
}

BaseEventCallBack::~BaseEventCallBack()
{
}

void BaseEventCallBack::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{
	int num = 0;
}

void BaseEventCallBack::onWake(PxActor** actors, PxU32 count)
{
	int num = 0;
}

void BaseEventCallBack::onSleep(PxActor** actors, PxU32 count)
{
	int num = 0;
}

void BaseEventCallBack::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	int num = 0;
}

void BaseEventCallBack::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (PxU32 i = 0; i < count; i++)
	{
		PhysData* Other = reinterpret_cast<PhysData*>(pairs[i].otherActor->userData);
		PhysData* Target = reinterpret_cast<PhysData*>(pairs[i].triggerActor->userData);

		if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			Other->OnTriggerEnter = true;
			Other->OnTriggerStay = true;
			Other->TriggerCount++;

			Target->OnTriggerEnter = true;
			Target->OnTriggerStay = true;
			Target->TriggerCount++;

			Other->TriggerList.push_back(Target);
			Target->TriggerList.push_back(Other);
		}
		else if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			Other->OnTriggerStay = false;
			Other->OnTriggerExit = true;
			Other->TriggerCount--;

			int count = (int)Other->TriggerList.size();
			for (int i = 0; i < count; i++)
			{
				if (Other->TriggerList[i] == Target)
				{
					Other->TriggerList.erase(Other->TriggerList.begin() + i);
					break;
				}
			}


			Target->OnTriggerStay = false;
			Target->OnTriggerExit = true;
			Target->TriggerCount--;
			count = (int)Target->TriggerList.size();
			for (int i = 0; i < count; i++)
			{
				if (Target->TriggerList[i] == Other)
				{
					Target->TriggerList.erase(Target->TriggerList.begin() + i);
					break;
				}
			}
		}
	}
}

void BaseEventCallBack::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
	int num = 0;
}
