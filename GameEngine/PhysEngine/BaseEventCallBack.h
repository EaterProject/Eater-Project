#pragma once
using namespace physx;
#include "PxSimulationEventCallback.h"
class PhysData;
class BaseEventCallBack : public PxSimulationEventCallback
{
public:
	BaseEventCallBack();
	~BaseEventCallBack();


	// PxSimulationEventCallback��(��) ���� ��ӵ�
	virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override;

	virtual void onWake(PxActor** actors, PxU32 count) override;

	virtual void onSleep(PxActor** actors, PxU32 count) override;

	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;

	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override;

	virtual void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override;

private:
	bool isTargetPush = false;
	bool isOtherPush = false;
	void PushExitObject(PhysData* Other , PhysData* Target);
};

