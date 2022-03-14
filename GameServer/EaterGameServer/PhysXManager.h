#pragma once

class PhysEngine;
class PhysRayCast;
class PhysData;

// PhysX Wrapper Class

class PhysXManager
{
public:
	PhysXManager();
	~PhysXManager();

	void Initialize();
	void Release();
	void Update(float _dTime);
	void Create_Actor(PhysData* _Data);
	void Update_Actor(PhysData* _Data);
	void Delete_Actor(PhysData* _Data);
	void RayCast(PhysRayCast* _Ray);
};

