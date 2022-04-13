#pragma once
#include <string>
#include <vector>
#include "SimpleMath.h"

class ColliderBuffer;
class OneTriangle;

class NavigationManager
{
public:
	NavigationManager();
	~NavigationManager();
	void Initialize();
	void Update();

	void DebugFriendFace(int FaceIndex);
	void DebugMoveLine(int StartIndex, int EndIndex);
	void CreateHeap(unsigned int  StartFaceIndex, unsigned int EndFaceIndex);
	float FindCost(float& EndX,float& EndY,float& EndZ, int Index);
private:
	std::vector<OneTriangle*>* NavigationData;

	bool isGet = false;

	std::vector<int> Point;
};

