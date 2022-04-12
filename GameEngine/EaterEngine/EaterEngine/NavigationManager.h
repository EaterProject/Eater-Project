#pragma once
#include <string>
class ColliderBuffer;
class NavigationManager
{
public:
	NavigationManager();
	~NavigationManager();
	void Initialize();
	void Update();
	void SetNavMeshCollider(std::string&& ColliderName);
	void SetNavMeshCollider(std::string& ColliderName);
	void SettingNavMesh();
private:



	ColliderBuffer* mColliderBuffer;
};

