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
		isTargetPush	= false;
		isOtherPush		= false;
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


			for (int i = 0; i < 10; i++)
			{
				if (Other->TriggerList[i] == nullptr)
				{
					Other->TriggerList[i] = Target;
					isOtherPush = true;
				}

				if (Target->TriggerList[i] == nullptr)
				{
					Target->TriggerList[i] = Other;
					isTargetPush = true;
				}

				if (isTargetPush == true && isOtherPush == true)
				{
					break;
				}
			}
		}

		if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			Other->OnTriggerStay = false;
			Other->OnTriggerExit = true;
			Other->TriggerCount--;

			for (int i = 0; i < 10; i++)
			{
				if (Other->TriggerList[i] == Target)
				{
					Other->TriggerList[i] = nullptr;
					PushExitObject(Other, Target);
					break;
				}
			}


			Target->OnTriggerStay = false;
			Target->OnTriggerExit = true;
			Target->TriggerCount--;
			for (int i = 0; i < 10; i++)
			{
				if (Target->TriggerList[i] == Other)
				{
					Target->TriggerList[i] = nullptr;
					PushExitObject(Target,Other);
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

void BaseEventCallBack::PushExitObject(PhysData* Other, PhysData* Target)
{
	for (int i = 0; i < 10; i++)
	{
		if (Other->TriggerExitOBJ[i] == nullptr)
		{
			Other->TriggerExitOBJ[i] = Target;
			break;
		}
	}
}
