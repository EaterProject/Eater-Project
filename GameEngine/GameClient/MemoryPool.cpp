#include "MemoryPool.h"
#include "GameObject.h"

void MemoryPool::Set(GameObject* obj)
{
	ObjectList.push_back(obj);
	obj->SetActive(false);
}

GameObject* MemoryPool::Get()
{
	return nullptr;
}
