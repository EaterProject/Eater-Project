#include "GameObject.h"
#include "EngineData.h"
#include "BaseManager.h"
#include "ObjectManager.h"
#include "Transform.h"
#include "MeshFilter.h"
#include "Material.h"
#include "Profiler/Profiler.h"

std::vector<int> GameObject::TagList;
GameObject::GameObject()
{
	Tag	= 0;			//�ױ�
	IsActive = true;	//��� ���۳�Ʈ ��뿩��
	Name = "GameObject";

	OneMeshData = new MeshData();
	OneMeshData->Object_Data = new ObjectData();
	OneMeshData->Object_Data->Object = this;

	transform = nullptr;
	DontDestroy = false;

	// ���� ������ GameObject�� List�� ����..
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

size_t GameObject::GetTag()
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
	//���� �ε����� �´� ���۳�Ʈ�� ���������ش�
	Component* temp = ComponentList[i];
	return temp;
}

int GameObject::GetComponentCount()
{
	//���۳�Ʈ ����Ʈ�� ����� ��ȯ
	return (int)ComponentList.size();
}

void GameObject::PushChildList(GameObject* obj)
{
	//�ڽ� ������Ʈ�� �ִ´�
	ChildMeshList.push_back(obj);
}

void GameObject::ChoiceParent(GameObject* obj)
{
	//���ڽ��� ������ ������Ʈ�� �ڽ����� �ִ´�
	transform->SetParent(obj->transform);
	obj->transform->SetChild(transform);
}

void GameObject::ChoiceChild(GameObject* obj)
{
	//���ڽ��� ������ ������Ʈ�� �ڽ����� �ִ´�
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
	std::string ComponentFunction = typeid(*con).name();
	ComponentFunction = ComponentFunction.substr(ComponentFunction.find(" ") + 1, ComponentFunction.size());

	switch (type)
	{
	case AWAKE:
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Push ][ %s ] 1.Awake", ComponentFunction.c_str());
		ObjectManager::PushAwake(con,con->Awake_Order);
		con->FUNCTION_MASK |= AWAKE;
		break;
	case START:
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Push ][ %s ] 3.Start", ComponentFunction.c_str());
		ObjectManager::PushStart(con, con->Start_Order);
		con->FUNCTION_MASK |= START;
		break;
	case SETUP:
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Push ][ %s ] 2.Setup", ComponentFunction.c_str());
		ObjectManager::PushStartPlay(con, con->Start_Order);
		con->FUNCTION_MASK |= SETUP;
		break;
	case START_UPDATE:
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Push ][ %s ] 4.StartUpdate", ComponentFunction.c_str());
		ObjectManager::PushStartUpdate(con, con->StartUpdate_Order);
		con->FUNCTION_MASK |= START_UPDATE;
		break;
	case Transform_UPDATE:
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Push ][ %s ] 5.TransformUpdate", ComponentFunction.c_str());
		ObjectManager::PushTransformUpdate(con, con->TransformUpdate_Order);
		con->FUNCTION_MASK |= Transform_UPDATE;
		break;
	case Physics_UPDATE:
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Push ][ %s ] 6.PhysicsUpdate", ComponentFunction.c_str());
		ObjectManager::PushPhysicsUpdate(con, con->PhysicsUpdate_Order);
		con->FUNCTION_MASK |= Physics_UPDATE;
		break;
	case UPDATE:
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Push ][ %s ] 7.Update", ComponentFunction.c_str());
		ObjectManager::PushUpdate(con, con->DefaultUpdate_Order);
		con->FUNCTION_MASK |= UPDATE;
		break;
	case END_UPDATE:
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Push ][ %s ] 8.EndUpdate", ComponentFunction.c_str());
		ObjectManager::PushEndUpdate(con, con->EndUpdate_Order);
		con->FUNCTION_MASK |= END_UPDATE;
		break;
	}
}
