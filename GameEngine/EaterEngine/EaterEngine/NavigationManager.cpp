#include "NavigationManager.h"
#include "LoadManager.h"
#include "EaterEngineAPI.h"
#include "EngineData.h"
#include "LoadManager.h"

NavigationManager::NavigationManager()
{

}

NavigationManager::~NavigationManager()
{

}

void NavigationManager::Initialize()
{

}

//void NavigationManager::Update()
//{
//	if (NavigationData != nullptr)
//	{
//		for (int i = 1; i < NavigationData->size(); i++)
//		{
//			DebugDrawLine((*NavigationData)[i]->VertexPos[0], (*NavigationData)[i]->VertexPos[1], Vector3(0.5, 0.5, 0));
//			DebugDrawLine((*NavigationData)[i]->VertexPos[1], (*NavigationData)[i]->VertexPos[2], Vector3(0.5, 0.5, 0));
//			DebugDrawLine((*NavigationData)[i]->VertexPos[2], (*NavigationData)[i]->VertexPos[0], Vector3(0.5, 0.5, 0));
//			DebugDrawSphere(0.1f, (*NavigationData)[i]->CenterPoint, Vector3(0, 0, 1));
//
//			
//			//DebugMoveLine(0, (*NavigationData)[0]->FriendFace[1]);
//			//DebugMoveLine(0, 2000);
//		}
//		DebugMoveLine(0, 799);
//	}
//	
//	if (GetKeyDown(VK_SPACE))
//	{
//		NavigationData = LoadManager::GetNavMeshData();
//		//CreateHeap(0, 2000);
//	}
//}
//
//void NavigationManager::DebugFriendFace(int FaceIndex)
//{
//	int index01 = (*NavigationData)[FaceIndex]->FriendFace[0];
//	int index02 = (*NavigationData)[FaceIndex]->FriendFace[1];
//	int index03 = (*NavigationData)[FaceIndex]->FriendFace[2];
//
//	if (index01 != -1)
//	{
//		DebugDrawLine((*NavigationData)[index01]->VertexPos[0], (*NavigationData)[index01]->VertexPos[1], Vector3(0, 1, 0));
//		DebugDrawLine((*NavigationData)[index01]->VertexPos[1], (*NavigationData)[index01]->VertexPos[2], Vector3(0, 1, 0));
//		DebugDrawLine((*NavigationData)[index01]->VertexPos[2], (*NavigationData)[index01]->VertexPos[0], Vector3(0, 1, 0));
//
//	}
//	if (index02 != -1)
//	{
//		DebugDrawLine((*NavigationData)[index02]->VertexPos[0], (*NavigationData)[index02]->VertexPos[1], Vector3(0, 1, 0));
//		DebugDrawLine((*NavigationData)[index02]->VertexPos[1], (*NavigationData)[index02]->VertexPos[2], Vector3(0, 1, 0));
//		DebugDrawLine((*NavigationData)[index02]->VertexPos[2], (*NavigationData)[index02]->VertexPos[0], Vector3(0, 1, 0));
//	}
//	if (index03 != -1)
//	{
//		DebugDrawLine((*NavigationData)[index03]->VertexPos[0], (*NavigationData)[index03]->VertexPos[1], Vector3(0, 1, 0));
//		DebugDrawLine((*NavigationData)[index03]->VertexPos[1], (*NavigationData)[index03]->VertexPos[2], Vector3(0, 1, 0));
//		DebugDrawLine((*NavigationData)[index03]->VertexPos[2], (*NavigationData)[index03]->VertexPos[0], Vector3(0, 1, 0));
//	}
//}
//
//void NavigationManager::DebugMoveLine(int StartIndex, int EndIndex)
//{
//	DebugDrawLine((*NavigationData)[StartIndex]->CenterPoint, (*NavigationData)[EndIndex]->CenterPoint, Vector3(1, 0, 0));
//}
//
//void NavigationManager::CreateHeap(unsigned int StartFaceIndex, unsigned int EndFaceIndex)
//{
//	Vector3 EndPointCenter		= (*NavigationData)[EndFaceIndex]->CenterPoint;
//
//	//처음 나의 시작위치를 리스트에 넣어준다
//	Point.push_back(StartFaceIndex);
//
//	int Startindex = StartFaceIndex;
//	while (true)
//	{
//		if (Startindex == EndFaceIndex)
//		{
//			break;
//		}
//
//
//		//나와 인접한 삼각형들을 가져온다
//		int Index01 = (*NavigationData)[Startindex]->FriendFace[0];
//		int Index02 = (*NavigationData)[Startindex]->FriendFace[1];
//		int Index03 = (*NavigationData)[Startindex]->FriendFace[2];
//
//		//가져온 삼각형들에서 비용이적은 삼각형을 가져온다
//		float MoveCost = 1000;
//		int MoveIndex = -1;
//		if (Index01 != -1 && Index01 != Startindex)
//		{
//			float Data = FindCost(EndPointCenter.x, EndPointCenter.y, EndPointCenter.z, Index01);
//			if (MoveCost > Data)
//			{
//				MoveCost = Data;
//				MoveIndex = Index01;
//			}
//		}
//
//		if (Index02 != -1 && Index02 != Startindex)
//		{
//			float Data = FindCost(EndPointCenter.x, EndPointCenter.y, EndPointCenter.z, Index02);
//			if (MoveCost > Data)
//			{
//				MoveCost = Data;
//				MoveIndex = Index01;
//			}
//		}
//
//		if (Index03 != -1 && Index03 != Startindex)
//		{
//			float Data = FindCost(EndPointCenter.x, EndPointCenter.y, EndPointCenter.z, Index03);
//			if (MoveCost > Data)
//			{
//				MoveCost = Data;
//				MoveIndex = Index01;
//			}
//		}
//
//		Startindex = MoveIndex;
//		for (int i = 0; i < Point.size(); i++)
//		{
//			if (Point[i] == MoveIndex)
//			{
//				break;
//			}
//		}
//		Point.push_back(MoveIndex);
//	}
//}
//
//float NavigationManager::FindCost(float& EndX, float& EndY, float& EndZ, int Index)
//{
//	float x = EndX - (*NavigationData)[Index]->CenterPoint.x;
//	float y = EndY - (*NavigationData)[Index]->CenterPoint.y;
//	float z = EndZ - (*NavigationData)[Index]->CenterPoint.z;
//	return max(max(x, y), z);
//}
//
