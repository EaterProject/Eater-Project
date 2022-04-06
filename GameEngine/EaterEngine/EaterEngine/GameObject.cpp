#include "GameObject.h"
#include "EngineData.h"
#include "BaseManager.h"
#include "ObjectManager.h"
#include "Transform.h"
#include "MeshFilter.h"
#include "Material.h"
#include "Profiler/Profiler.h"

GameObject::GameObject()
{
	Tag	= 0;			//테그
	IsActive = true;	//모든 컨퍼넌트 사용여부
	Name = "GameObject";

	OneMeshData = new MeshData();
	OneMeshData->Object_Data = new ObjectData();
	OneMeshData->Object_Data->Object = this;

	transform = nullptr;
	DontDestroy = false;

	// 현재 생성된 GameObject를 List에 삽입..
	ObjectManager::PushCreateObject(this);
}

GameObject::~GameObject()
{
	delete OneMeshData->Object_Data;
	delete OneMeshData;
	OneMeshData = nullptr;
}

void GameObject::SetActive(bool active)
{
	OneMeshData->Object_Data->IsActive = active;

	std::vector<Component*>::iterator it = ComponentList.begin();
	for (it; it != ComponentList.end(); it++)
	{
		(*it)->Enabled = active;
	}
}

void GameObject::SetDontDestroy(bool mDontDestroy)
{
	DontDestroy = mDontDestroy;
	int BoneCount = GetChildBoneCount();
	for (int i = 0; i < BoneCount; i++)
	{
		ChildBoneList[i]->SetDontDestroy(mDontDestroy);
	}

	int MeshCount = GetChildMeshCount();
	for (int i = 0; i < MeshCount; i++)
	{
		ChildMeshList[i]->SetDontDestroy(mDontDestroy);
	}
}

bool GameObject::SetTag(std::string TagName)
{
	int Number = ObjectManager::FindTag(TagName);
	if (Number == -1)
	{
		Tag = 0;
		return false;
	}
	else
	{
		Tag = Number;
		return true;
	}
}

bool GameObject::SetTag(int TagNumber)
{
	Tag = TagNumber;
	return true;
}

int GameObject::GetTag()
{
	return Tag;
}

GameObject* GameObject::GetChildBone(std::string FindName)
{
	GameObject* FindObject = nullptr;
	if (Name == FindName)
	{
		FindObject = this;
		return FindObject;
	}


	std::vector<GameObject*>::iterator it = ChildBoneList.begin();
	for (it; it != ChildBoneList.end(); it++)
	{
		FindObject = (*it)->GetChildBone(FindName);
		if (FindObject != nullptr)
		{
			return FindObject;
		}
	}

	return FindObject;
}

 GameObject* GameObject::GetChildBone(int num)
{
	 GameObject* obj = nullptr;
	 int size = (int)ChildBoneList.size();
	 if (size > num)
	 {
		 obj = ChildBoneList[num];
	 }

	 return obj;
}

GameObject* GameObject::GetChildMesh(std::string FindName)
{
	GameObject* FindObject = nullptr;
	if (Name == FindName)
	{
		FindObject = this;
		return FindObject;
	}


	std::vector<GameObject*>::iterator it = ChildMeshList.begin();
	for (it; it != ChildMeshList.end(); it++)
	{
		FindObject = (*it)->GetChildMesh(FindName);
		if (FindObject != nullptr)
		{
			return FindObject;
		}
	}

	return FindObject;
}

GameObject* GameObject::GetChildMesh(int num)
{
	GameObject* obj = nullptr;
	int size = (int)ChildMeshList.size();
	if (size > num)
	{
		obj = ChildMeshList[num];
	}

	return obj;
}

GameObject* GameObject::GetChildObject(std::string Name)
{
	GameObject* BoneObject = GetChildBone(Name);
	GameObject* MeshObject = GetChildMesh(Name);

	if (BoneObject != nullptr)
	{
		return BoneObject;
	}
	else if (MeshObject != nullptr)
	{
		return MeshObject;
	}
	else
	{
		return nullptr;
	}
}

bool GameObject::GetDontDestroy()
{
	return DontDestroy;
}

int GameObject::GetChildMeshCount()
{
	return (int)ChildMeshList.size();
}

int GameObject::GetChildBoneCount()
{
	return  (int)ChildBoneList.size();
}

Transform* GameObject::GetTransform()
{
	if (transform != nullptr)
	{
		return  transform;
	}
	else
	{
		return nullptr;
	}
}

Material* GameObject::GetMaterial()
{
	MeshFilter* meshfilter = GetComponent<MeshFilter>();

	if (meshfilter)
	{
		return meshfilter->GetMaterial();
	}
	
	return nullptr;
}

Component* GameObject::GetDeleteComponent(int i)
{
	//들어온 인덱스에 맞는 컨퍼넌트를 삭제시켜준다
	Component* temp = ComponentList[i];
	return temp;
}

int GameObject::GetComponentCount()
{
	//컨퍼넌트 리스트의 사이즈를 반환
	return (int)ComponentList.size();
}

void GameObject::PushChildList(GameObject* obj)
{
	//자식 오브젝트로 넣는다
	ChildMeshList.push_back(obj);
}

void GameObject::ChoiceParent(GameObject* obj)
{
	//나자신을 선택한 오브젝트의 자식으로 넣는다
	transform->SetParent(obj->transform);
	obj->transform->SetChild(transform);
}

void GameObject::ChoiceChild(GameObject* obj)
{
	//나자신을 선택한 오브젝트의 자식으로 넣는다
	transform->SetChild(obj->transform);
	obj->transform->SetParent(transform);
}

void GameObject::PushChildMeshObject(GameObject* obj)
{
	ChildMeshList.push_back(obj);
}

void GameObject::PushChildBoneObject(GameObject* obj)
{
	ChildBoneList.push_back(obj);
}

Component* GameObject::GetComponent(int index)
{
	return ComponentList[index];
}

void GameObject::PushComponentFunction(Component* con, unsigned int type)
{
	switch (type)
	{
	case AWAKE:
		ObjectManager::PushAwake(con,con->Awake_Order);
		con->FUNCTION_MASK |= AWAKE;
		break;
	case START:
		ObjectManager::PushStart(con, con->Start_Order);
		con->FUNCTION_MASK |= START;
		break;
	case SETUP:
		ObjectManager::PushStartPlay(con, con->Start_Order);
		con->FUNCTION_MASK |= SETUP;
		break;
	case START_UPDATE:
		ObjectManager::PushStartUpdate(con, con->StartUpdate_Order);
		con->FUNCTION_MASK |= START_UPDATE;
		break;
	case Transform_UPDATE:
		ObjectManager::PushTransformUpdate(con, con->TransformUpdate_Order);
		con->FUNCTION_MASK |= Transform_UPDATE;
		break;
	case Physics_UPDATE:
		ObjectManager::PushPhysicsUpdate(con, con->PhysicsUpdate_Order);
		con->FUNCTION_MASK |= Physics_UPDATE;
		break;
	case UPDATE:
		ObjectManager::PushUpdate(con, con->DefaultUpdate_Order);
		con->FUNCTION_MASK |= UPDATE;
		break;
	case END_UPDATE:
		ObjectManager::PushEndUpdate(con, con->EndUpdate_Order);
		con->FUNCTION_MASK |= END_UPDATE;
		break;
	}
}

void GameObject::PushPhysFunction(Component* con, unsigned int type)
{
	switch (type)
	{
	case PHYS_TRIIGER_ENTER:
		OnTrigger_Enter_Function.push_back(std::bind(&Component::OnTriggerEnter, con, std::placeholders::_1));
		break;
	case PHYS_TRIIGER_STAY:
		OnTrigger_Stay_Function.push_back(std::bind(&Component::OnTriggerStay, con, std::placeholders::_1));
		break;
	case PHYS_TRIIGER_EXIT:
		OnTrigger_Exit_Function.push_back(std::bind(&Component::OnTriggerExit, con, std::placeholders::_1));
		break;
	}

}

void GameObject::PlayPhysFunction(GameObject* Obj, unsigned int type)
{
	int Size = 0;
	switch (type)
	{
	case PHYS_TRIIGER_ENTER:
		Size = (int)OnTrigger_Enter_Function.size();
		for (int i = 0; i < Size; i++)
		{
			OnTrigger_Enter_Function[i](Obj);
		}
		break;
	case PHYS_TRIIGER_STAY:
		Size = (int)OnTrigger_Stay_Function.size();
		for (int i = 0; i < Size; i++)
		{
			OnTrigger_Stay_Function[i](Obj);
		}
		break;
	case PHYS_TRIIGER_EXIT:
		Size = (int)OnTrigger_Exit_Function.size();
		for (int i = 0; i < Size; i++)
		{
			OnTrigger_Exit_Function[i](Obj);
		}
		break;
	}
}
