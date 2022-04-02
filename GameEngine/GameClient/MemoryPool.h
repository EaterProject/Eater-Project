#pragma once
#include <vector>
class GameObject;
class MemoryPool
{
public:
	MemoryPool() =default;
	~MemoryPool() = default;

	void Set(GameObject* obj);
	GameObject* Get();
private:
	std::vector<GameObject*> ObjectList;
};

