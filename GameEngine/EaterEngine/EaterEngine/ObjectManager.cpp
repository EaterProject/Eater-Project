#include "Component.h"
#include "GameObject.h"
#include "BaseManager.h"
#include "ObjectManager.h"
#include "LoadManager.h"
#include "Camera.h"
#include "EngineData.h"
#include "KeyinputManager.h"
#include "MeshFilter.h"
#include "Light.h"
#include "GraphicsEngine.h"

//함수포인터 리스트들
Delegate_Map ObjectManager::AwakeFunction;
Delegate_Map ObjectManager::StartFunction;
Delegate_Map ObjectManager::SetUpFunction;
Delegate_Map ObjectManager::StartUpdate;
Delegate_Map ObjectManager::TransformUpdate;
Delegate_Map ObjectManager::PhysicsUpdate;
Delegate_Map ObjectManager::Update;
Delegate_Map ObjectManager::EndUpdate;

//오브젝트 리스트
std::vector<GameObject*>			ObjectManager::ObjectList;
std::map<int, std::string>			ObjectManager::TagList;
ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{

}

void ObjectManager::PushCreateObject(GameObject* obj)
{
	ObjectData* objData = obj->OneMeshData->Object_Data;

	//오브젝트를 넣어줄때 빈곳이 있는지부터 확인
	int ObjectListSize = (int)ObjectList.size();
	for (int i = 0; i < ObjectListSize; i++)
	{
		if (ObjectList[i] == nullptr)
		{
			ObjectList[i] = obj;
			objData->ObjectIndex = i;
			return;
		}
	}
	//빈곳이없다면 그냥 넣어줌
	ObjectList.push_back(obj);

	//해당 오브젝트 고유의 Hash Color 설정
	objData->ObjectIndex = ObjectListSize;
}

void ObjectManager::PushDeleteObject(GameObject* obj)
{
	std::vector<GameObject*>::iterator it = ObjectList.begin();
	for (it; it != ObjectList.end(); it++)
	{
		//삭제할 오브젝트를 찾았다면 그오브젝트를 일단 리스트에서 뺀다
		if ((*it) == obj)
		{
			GameObject* temp = (*it);
			(*it) = nullptr;

			//삭제할 리스트에 넣어준다
			DeleteList.push(temp);
		}
	}
}

void ObjectManager::AllDeleteObject()
{
	GraphicEngine* Graphic = GraphicEngine::Get();
	
	int index = 0;
	while (true)
	{
		//삭제 종료 조건
		int DeleteObjectCount = (int)ObjectList.size();
		if (DeleteObjectCount <= index)
		{
			break;
		}
		//비어있는 객체 넘기기
		if (ObjectList[index] == nullptr) { index++; continue; }

		GameObject* DeleteObject = ObjectList[index];

		//삭제하면 안되는 오브젝트 옮기기
		if (ObjectList[index]->GetDontDestroy() == true)
		{
			DontDestroyObjectList.push_back(DeleteObject);
			ObjectList[index] = nullptr;
			index++;
			continue;
		}

		// Renderer 측 Mesh Data 제거..
		Graphic->DeleteInstance(DeleteObject->OneMeshData);

		int ChildMeshCount = DeleteObject->GetChildMeshCount();
		int ChildBoneCount = DeleteObject->GetChildBoneCount();

		//나의 바로 밑에 오브젝트들 을 삭제 할 리스트에 넣는다 
		for (int i = 0; i < ChildMeshCount; i++)
		{
			ObjectList.push_back(DeleteObject->GetChildMesh(i));
		}

		for (int i = 0; i < ChildBoneCount; i++)
		{
			ObjectList.push_back(DeleteObject->GetChildBone(i));
		}

		//현재 오브젝트의 컨퍼넌트를 삭제
		int Componentcount = DeleteObject->GetComponentCount();
		for (int j = 0; j < Componentcount; j++)
		{
			Component* cpt = DeleteObject->GetDeleteComponent(j);
			DeleteComponent(cpt);
		}

		//모두 삭제 했으니 나자신도 삭제
		delete DeleteObject;
		DeleteObject = nullptr;

		ObjectList[index] = nullptr;
		index++;
	}
	ObjectList.clear();

	//삭제하면 안되는 오브젝트들을 다시 셋팅해준다
	//DontDestroyObjectSetting();
}

void ObjectManager::Initialize()
{

}

void ObjectManager::PushAwake(Component* mComponent, int Order)
{
	ComponentFunctionData data;
	//활성화 여부
	data.Enabled = &mComponent->Enabled;
	//함수 포인터
	data.FunctionPointer = std::bind(&Component::Awake, mComponent);
	//컨퍼넌트 포인터
	data.ComponentPointer = mComponent;
	//컨퍼넌트 순서
	data.OrderCount = Order;

	AwakeFunction.Push(data);
}
void ObjectManager::PushStartUpdate(Component* mComponent, int Order)
{
	ComponentFunctionData data;
	//활성화 여부
	data.Enabled = &mComponent->Enabled;
	//함수 포인터
	data.FunctionPointer = std::bind(&Component::StartUpdate, mComponent);
	//컨퍼넌트 포인터
	data.ComponentPointer = mComponent;
	//컨퍼넌트 순서
	data.OrderCount = Order;
	StartUpdate.Push(data);
}
void ObjectManager::PushTransformUpdate(Component* mComponent, int Order)
{
	ComponentFunctionData data;
	//활성화 여부
	data.Enabled = &mComponent->Enabled;
	//함수 포인터
	data.FunctionPointer = std::bind(&Component::TransformUpdate, mComponent);
	//컨퍼넌트 포인터
	data.ComponentPointer = mComponent;
	//컨퍼넌트 순서
	data.OrderCount = Order;

	TransformUpdate.Push(data);
}
void ObjectManager::PushPhysicsUpdate(Component* mComponent, int Order)
{
	ComponentFunctionData data;
	//활성화 여부
	data.Enabled = &mComponent->Enabled;
	//함수 포인터
	data.FunctionPointer = std::bind(&Component::PhysicsUpdate, mComponent);
	//컨퍼넌트 포인터
	data.ComponentPointer = mComponent;
	//컨퍼넌트 순서
	data.OrderCount = Order;

	PhysicsUpdate.Push(data);
}
void ObjectManager::PushUpdate(Component* mComponent, int Order)
{
	ComponentFunctionData data;
	//활성화 여부
	data.Enabled = &mComponent->Enabled;
	//함수 포인터
	data.FunctionPointer = std::bind(&Component::Update, mComponent);
	//컨퍼넌트 포인터
	data.ComponentPointer = mComponent;
	//컨퍼넌트 순서
	data.OrderCount = Order;

	Update.Push(data);
}

int ObjectManager::FindTag(std::string TagName)
{
	//들어온 이름에 따라 태그의 번호를찾고 해당하는 오브젝트를 가져온다
	std::map<int, std::string>::iterator Start_it = TagList.begin();
	std::map<int, std::string>::iterator End_it = TagList.begin();
	int FindIndex = -1;

	//들어온 이름으로 태그의 인덱스 번호를 찾는다
	for (Start_it; Start_it != End_it; Start_it++)
	{
		if (TagName == Start_it->second)
		{
			FindIndex = Start_it->first;
		}
	}

	return FindIndex;
}

void ObjectManager::PushEndUpdate(Component* mComponent, int Order)
{
	ComponentFunctionData data;
	//활성화 여부
	data.Enabled = &mComponent->Enabled;
	//함수 포인터
	data.FunctionPointer = std::bind(&Component::EndUpdate, mComponent);
	//컨퍼넌트 포인터
	data.ComponentPointer = mComponent;
	//컨퍼넌트 순서
	data.OrderCount = Order;

	EndUpdate.Push(data);
}

void ObjectManager::PushStart(Component* mComponent, int Order)
{
	ComponentFunctionData data;
	//활성화 여부
	data.Enabled = &mComponent->Enabled;
	//함수 포인터
	data.FunctionPointer = std::bind(&Component::Start, mComponent);
	//컨퍼넌트 포인터
	data.ComponentPointer = mComponent;
	//컨퍼넌트 순서
	data.OrderCount = Order;

	StartFunction.Push(data);
}

void ObjectManager::PushStartPlay(Component* mComponent, int Order)
{
	ComponentFunctionData data;
	//활성화 여부
	data.Enabled = &mComponent->Enabled;
	//함수 포인터
	data.FunctionPointer = std::bind(&Component::SetUp, mComponent);
	//컨퍼넌트 포인터
	data.ComponentPointer = mComponent;
	//컨퍼넌트 순서
	data.OrderCount = Order;

	SetUpFunction.Push(data);
}


void ObjectManager::InitializeFuntion()
{

}

void ObjectManager::PlayUpdate()
{
	///각각의 함수포인터들을 재생시킨다
	//컨퍼넌트들을 가져오는 함수 포인터
	AwakeFunction.PlayOnce();
	//컨퍼넌트 초기화된 값을 한번 실행하는 함수포인터
	SetUpFunction.PlayOnce();
	//컨퍼넌트들을 초기화 하는 함수포인터
	StartFunction.PlayOnce();

	//가장 먼저실행되는 StartUpdate 함수 리스트(각 컨퍼넌트들의 초기화작업을 해줄때)
	StartUpdate.Play();
	//물리 충돌관련 Update 함수 리스트 (물리관련 컨퍼넌트들을 업데이트)
	PhysicsUpdate.Play();
	//이동행렬 실행되는 Update 함수 리스트
	TransformUpdate.Play();
	//중간 단계에 실행되는 Update 함수 리스트 (클라이언트쪽에서 만든 컨퍼넌트들이 업데이트될곳)
	Update.Play();
	//가장 마지막에 실행되는 Update 함수 리스트
	EndUpdate.Play();
}

void ObjectManager::ClearFunctionList()
{
	AwakeFunction.Clear();
	SetUpFunction.Clear();
	StartFunction.Clear();

	StartUpdate.Clear();
	TransformUpdate.Clear();
	PhysicsUpdate.Clear();
	Update.Clear();
	EndUpdate.Clear();
}

void ObjectManager::DeleteObject()
{
	GraphicEngine* Graphic = GraphicEngine::Get();

	while(true)
	{
		//삭제 종료 조건
		int DeleteObjectCount = (int)DeleteList.size();
		if (DeleteObjectCount == 0)
		{
			break;
		}

		GameObject* temp = DeleteList.front();
		int ChildMeshCount = temp->GetChildMeshCount();
		int ChildBoneCount = temp->GetChildBoneCount();

		// Renderer 측 Mesh Data 제거..
		Graphic->DeleteInstance(temp->OneMeshData);

		//나의 바로 밑에 오브젝트들 을 삭제 할 리스트에 넣는다 
		for (int i = 0; i < ChildMeshCount; i++) 
		{
			GameObject* obj = temp->GetChildMesh(i);
			ObjectData* objData = obj->OneMeshData->Object_Data;
			DeleteList.push(obj);
			ObjectList[objData->ObjectIndex] = nullptr;
		}

		for (int i = 0; i < ChildBoneCount; i++)
		{
			GameObject* obj = temp->GetChildBone(i);
			ObjectData* objData = obj->OneMeshData->Object_Data;
			DeleteList.push(obj);
			ObjectList[objData->ObjectIndex] = nullptr;
		}

		//현제 오브젝트의 컨퍼넌트를 삭제
		int Componentcount = temp->GetComponentCount();
		for (int j = 0; j < Componentcount; j++)
		{
			Component* cpt = temp->GetDeleteComponent(j);
			DeleteComponent(cpt);
		}

		//모두 삭제 했으니 나자신도 삭제
		delete temp;
		temp = nullptr;

		//리스트에서도 빼준다
		DeleteList.pop();
	}
}

GameObject* ObjectManager::FindGameObjectString(std::string& ObjectName)
{
	//모든 오브젝트에서 같은 이름의 오브젝트를 찾는다
	int ObjectSize = ObjectList.size();
	for (int i = 0; i < ObjectSize; i++)
	{
		if (ObjectList[i]->Name == ObjectName)
		{
			return ObjectList[i];
		}
	}

	return nullptr;
}

GameObject* ObjectManager::FindGameObjectTag(std::string& TagName)
{
	//들어온 이름에 따라 태그의 번호를찾고 해당하는 오브젝트를 가져온다
	std::map<int, std::string>::iterator Start_it	= TagList.begin();
	std::map<int, std::string>::iterator End_it		= TagList.begin();
	int FindIndex = -1;

	//들어온 이름으로 태그의 인덱스 번호를 찾는다
	for (Start_it; Start_it != End_it; Start_it++)
	{
		if (TagName == Start_it->second) 
		{
			FindIndex = Start_it->first;
		}
	}

	//인덱스 번호로 모든 오브젝트를 순환후 같은 오브젝트를 가져온다
	int ObjectMaxCount = (int)ObjectList.size();
	for (int i = 0; i < ObjectMaxCount; i++)
	{
		if (ObjectList[i]->GetTag() == FindIndex)
		{
			return ObjectList[i];
		}
	}

	return nullptr;
}

ComponentFunctionData ObjectManager::PushComponentData(Component* mComponent)
{
	ComponentFunctionData data;
	//활성화 여부
	data.Enabled = &mComponent->Enabled;
	//함수 포인터
	data.FunctionPointer = std::bind(&Component::Awake, mComponent);
	//컨퍼넌트 포인터
	data.ComponentPointer = mComponent;
	//컨퍼넌트 순서
	data.OrderCount = mComponent->OrderCount;

	return data;
}

void ObjectManager::DontDestroyObjectSetting()
{
	std::vector<GameObject*>::iterator it = DontDestroyObjectList.begin();
	for (it; it != DontDestroyObjectList.end(); it++)
	{
		if ((*it) == nullptr) { continue; }

		GameObject* obj = (*it);
		int ChildMeshCount = obj->GetChildMeshCount();
		int ChildBoneCount = obj->GetChildBoneCount();
		ObjectList.push_back(obj);

		//현재 자신의 컨퍼넌트를 넣어준다
		int ComponentMaxCount = obj->GetComponentCount();
		for (int j = 0; j < ComponentMaxCount; j++)
		{
			//컨퍼넌트들을 함수포인터로 넣어준다
			Component* cpt = obj->GetComponent(j);
			DontDestroyComponentSetting(cpt);
		}

		//자식 매쉬 오브젝트들셋팅
		for (int i = 0; i < ChildMeshCount; i++) 
		{
			GameObject* ChildMeshObject = obj->GetChildMesh(i);

			//컨퍼넌트 셋팅
			int ComponentMaxCount = ChildMeshObject->GetComponentCount();
			for (int j = 0; j < ComponentMaxCount; j++)
			{
				//컨퍼넌트들을 함수포인터로 넣어준다
				Component* cpt = ChildMeshObject->GetComponent(j);
				DontDestroyComponentSetting(cpt);
			}
		}
		
		//자식 본 오브젝트들 셋팅
		for (int i = 0; i < ChildBoneCount; i++)
		{
			GameObject* ChildBoneObject = obj->GetChildBone(i);

			//컨퍼넌트 셋팅
			int ComponentMaxCount = ChildBoneObject->GetComponentCount();
			for (int j = 0; j < ComponentMaxCount; j++)
			{
				//컨퍼넌트들을 함수포인터로 넣어준다
				Component* cpt = ChildBoneObject->GetComponent(j);
				DontDestroyComponentSetting(cpt);
			}
		}
	}

	//모두 옮겼으니 리스트는 초기화
	DontDestroyObjectList.clear();
}

void ObjectManager::DontDestroyComponentSetting(Component* cpt)
{
	//Awack,setup,Start 는 이전 씬에서 생성되었을때 실행 했으니 넣어주지않는다

	if (cpt->FUNCTION_MASK & START_UPDATE)
	{
		PushStartUpdate(cpt, cpt->Awake_Order);
	}

	if (cpt->FUNCTION_MASK & Transform_UPDATE)
	{
		PushTransformUpdate(cpt, cpt->TransformUpdate_Order);
	}

	if (cpt->FUNCTION_MASK & Physics_UPDATE)
	{
		PushPhysicsUpdate(cpt, cpt->PhysicsUpdate_Order);
	}

	if (cpt->FUNCTION_MASK & UPDATE)
	{
		PushUpdate(cpt, cpt->DefaultUpdate_Order);
	}
												
	if (cpt->FUNCTION_MASK & END_UPDATE)
	{
		PushEndUpdate(cpt, cpt->EndUpdate_Order);
	}
}

void ObjectManager::DeleteComponent(Component* cpt)
{
	//업데이트 함수포인터에 넣었던 것들 삭제
	if (cpt->FUNCTION_MASK & START_UPDATE)
	{
		StartUpdate.Pop(cpt);
	}
	
	if (cpt->FUNCTION_MASK & Transform_UPDATE)
	{
		TransformUpdate.Pop(cpt);
	}
	
	if (cpt->FUNCTION_MASK & Physics_UPDATE)
	{
		PhysicsUpdate.Pop(cpt);
	}
	
	if (cpt->FUNCTION_MASK & UPDATE)
	{
		Update.Pop(cpt);
	}
	
	if (cpt->FUNCTION_MASK & END_UPDATE)
	{
		EndUpdate.Pop(cpt);
	}


	///진짜 컨퍼넌트 삭제
	delete cpt;
	cpt = nullptr;
}
