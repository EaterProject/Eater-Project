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

//�Լ������� ����Ʈ��
Delegate_Map ObjectManager::AwakeFunction;
Delegate_Map ObjectManager::StartFunction;
Delegate_Map ObjectManager::SetUpFunction;
Delegate_Map ObjectManager::StartUpdate;
Delegate_Map ObjectManager::TransformUpdate;
Delegate_Map ObjectManager::PhysicsUpdate;
Delegate_Map ObjectManager::Update;
Delegate_Map ObjectManager::EndUpdate;

//������Ʈ ����Ʈ
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

	//������Ʈ�� �־��ٶ� ����� �ִ������� Ȯ��
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
	//����̾��ٸ� �׳� �־���
	ObjectList.push_back(obj);

	//�ش� ������Ʈ ������ Hash Color ����
	objData->ObjectIndex = ObjectListSize;
}

void ObjectManager::PushDeleteObject(GameObject* obj)
{
	std::vector<GameObject*>::iterator it = ObjectList.begin();
	for (it; it != ObjectList.end(); it++)
	{
		//������ ������Ʈ�� ã�Ҵٸ� �׿�����Ʈ�� �ϴ� ����Ʈ���� ����
		if ((*it) == obj)
		{
			GameObject* temp = (*it);
			(*it) = nullptr;

			//������ ����Ʈ�� �־��ش�
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
		//���� ���� ����
		int DeleteObjectCount = (int)ObjectList.size();
		if (DeleteObjectCount <= index)
		{
			break;
		}
		//����ִ� ��ü �ѱ��
		if (ObjectList[index] == nullptr) { index++; continue; }

		GameObject* DeleteObject = ObjectList[index];

		//�����ϸ� �ȵǴ� ������Ʈ �ű��
		if (ObjectList[index]->GetDontDestroy() == true)
		{
			DontDestroyObjectList.push_back(DeleteObject);
			ObjectList[index] = nullptr;
			index++;
			continue;
		}

		// Renderer �� Mesh Data ����..
		Graphic->DeleteInstance(DeleteObject->OneMeshData);

		int ChildMeshCount = DeleteObject->GetChildMeshCount();
		int ChildBoneCount = DeleteObject->GetChildBoneCount();

		//���� �ٷ� �ؿ� ������Ʈ�� �� ���� �� ����Ʈ�� �ִ´� 
		for (int i = 0; i < ChildMeshCount; i++)
		{
			ObjectList.push_back(DeleteObject->GetChildMesh(i));
		}

		for (int i = 0; i < ChildBoneCount; i++)
		{
			ObjectList.push_back(DeleteObject->GetChildBone(i));
		}

		//���� ������Ʈ�� ���۳�Ʈ�� ����
		int Componentcount = DeleteObject->GetComponentCount();
		for (int j = 0; j < Componentcount; j++)
		{
			Component* cpt = DeleteObject->GetDeleteComponent(j);
			DeleteComponent(cpt);
		}

		//��� ���� ������ ���ڽŵ� ����
		delete DeleteObject;
		DeleteObject = nullptr;

		ObjectList[index] = nullptr;
		index++;
	}
	ObjectList.clear();

	//�����ϸ� �ȵǴ� ������Ʈ���� �ٽ� �������ش�
	//DontDestroyObjectSetting();
}

void ObjectManager::Initialize()
{

}

void ObjectManager::PushAwake(Component* mComponent, int Order)
{
	ComponentFunctionData data;
	//Ȱ��ȭ ����
	data.Enabled = &mComponent->Enabled;
	//�Լ� ������
	data.FunctionPointer = std::bind(&Component::Awake, mComponent);
	//���۳�Ʈ ������
	data.ComponentPointer = mComponent;
	//���۳�Ʈ ����
	data.OrderCount = Order;

	AwakeFunction.Push(data);
}
void ObjectManager::PushStartUpdate(Component* mComponent, int Order)
{
	ComponentFunctionData data;
	//Ȱ��ȭ ����
	data.Enabled = &mComponent->Enabled;
	//�Լ� ������
	data.FunctionPointer = std::bind(&Component::StartUpdate, mComponent);
	//���۳�Ʈ ������
	data.ComponentPointer = mComponent;
	//���۳�Ʈ ����
	data.OrderCount = Order;
	StartUpdate.Push(data);
}
void ObjectManager::PushTransformUpdate(Component* mComponent, int Order)
{
	ComponentFunctionData data;
	//Ȱ��ȭ ����
	data.Enabled = &mComponent->Enabled;
	//�Լ� ������
	data.FunctionPointer = std::bind(&Component::TransformUpdate, mComponent);
	//���۳�Ʈ ������
	data.ComponentPointer = mComponent;
	//���۳�Ʈ ����
	data.OrderCount = Order;

	TransformUpdate.Push(data);
}
void ObjectManager::PushPhysicsUpdate(Component* mComponent, int Order)
{
	ComponentFunctionData data;
	//Ȱ��ȭ ����
	data.Enabled = &mComponent->Enabled;
	//�Լ� ������
	data.FunctionPointer = std::bind(&Component::PhysicsUpdate, mComponent);
	//���۳�Ʈ ������
	data.ComponentPointer = mComponent;
	//���۳�Ʈ ����
	data.OrderCount = Order;

	PhysicsUpdate.Push(data);
}
void ObjectManager::PushUpdate(Component* mComponent, int Order)
{
	ComponentFunctionData data;
	//Ȱ��ȭ ����
	data.Enabled = &mComponent->Enabled;
	//�Լ� ������
	data.FunctionPointer = std::bind(&Component::Update, mComponent);
	//���۳�Ʈ ������
	data.ComponentPointer = mComponent;
	//���۳�Ʈ ����
	data.OrderCount = Order;

	Update.Push(data);
}

int ObjectManager::FindTag(std::string TagName)
{
	//���� �̸��� ���� �±��� ��ȣ��ã�� �ش��ϴ� ������Ʈ�� �����´�
	std::map<int, std::string>::iterator Start_it = TagList.begin();
	std::map<int, std::string>::iterator End_it = TagList.begin();
	int FindIndex = -1;

	//���� �̸����� �±��� �ε��� ��ȣ�� ã�´�
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
	//Ȱ��ȭ ����
	data.Enabled = &mComponent->Enabled;
	//�Լ� ������
	data.FunctionPointer = std::bind(&Component::EndUpdate, mComponent);
	//���۳�Ʈ ������
	data.ComponentPointer = mComponent;
	//���۳�Ʈ ����
	data.OrderCount = Order;

	EndUpdate.Push(data);
}

void ObjectManager::PushStart(Component* mComponent, int Order)
{
	ComponentFunctionData data;
	//Ȱ��ȭ ����
	data.Enabled = &mComponent->Enabled;
	//�Լ� ������
	data.FunctionPointer = std::bind(&Component::Start, mComponent);
	//���۳�Ʈ ������
	data.ComponentPointer = mComponent;
	//���۳�Ʈ ����
	data.OrderCount = Order;

	StartFunction.Push(data);
}

void ObjectManager::PushStartPlay(Component* mComponent, int Order)
{
	ComponentFunctionData data;
	//Ȱ��ȭ ����
	data.Enabled = &mComponent->Enabled;
	//�Լ� ������
	data.FunctionPointer = std::bind(&Component::SetUp, mComponent);
	//���۳�Ʈ ������
	data.ComponentPointer = mComponent;
	//���۳�Ʈ ����
	data.OrderCount = Order;

	SetUpFunction.Push(data);
}


void ObjectManager::InitializeFuntion()
{

}

void ObjectManager::PlayUpdate()
{
	///������ �Լ������͵��� �����Ų��
	//���۳�Ʈ���� �������� �Լ� ������
	AwakeFunction.PlayOnce();
	//���۳�Ʈ �ʱ�ȭ�� ���� �ѹ� �����ϴ� �Լ�������
	SetUpFunction.PlayOnce();
	//���۳�Ʈ���� �ʱ�ȭ �ϴ� �Լ�������
	StartFunction.PlayOnce();

	//���� ��������Ǵ� StartUpdate �Լ� ����Ʈ(�� ���۳�Ʈ���� �ʱ�ȭ�۾��� ���ٶ�)
	StartUpdate.Play();
	//���� �浹���� Update �Լ� ����Ʈ (�������� ���۳�Ʈ���� ������Ʈ)
	PhysicsUpdate.Play();
	//�̵���� ����Ǵ� Update �Լ� ����Ʈ
	TransformUpdate.Play();
	//�߰� �ܰ迡 ����Ǵ� Update �Լ� ����Ʈ (Ŭ���̾�Ʈ�ʿ��� ���� ���۳�Ʈ���� ������Ʈ�ɰ�)
	Update.Play();
	//���� �������� ����Ǵ� Update �Լ� ����Ʈ
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
		//���� ���� ����
		int DeleteObjectCount = (int)DeleteList.size();
		if (DeleteObjectCount == 0)
		{
			break;
		}

		GameObject* temp = DeleteList.front();
		int ChildMeshCount = temp->GetChildMeshCount();
		int ChildBoneCount = temp->GetChildBoneCount();

		// Renderer �� Mesh Data ����..
		Graphic->DeleteInstance(temp->OneMeshData);

		//���� �ٷ� �ؿ� ������Ʈ�� �� ���� �� ����Ʈ�� �ִ´� 
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

		//���� ������Ʈ�� ���۳�Ʈ�� ����
		int Componentcount = temp->GetComponentCount();
		for (int j = 0; j < Componentcount; j++)
		{
			Component* cpt = temp->GetDeleteComponent(j);
			DeleteComponent(cpt);
		}

		//��� ���� ������ ���ڽŵ� ����
		delete temp;
		temp = nullptr;

		//����Ʈ������ ���ش�
		DeleteList.pop();
	}
}

GameObject* ObjectManager::FindGameObjectString(std::string& ObjectName)
{
	//��� ������Ʈ���� ���� �̸��� ������Ʈ�� ã�´�
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
	//���� �̸��� ���� �±��� ��ȣ��ã�� �ش��ϴ� ������Ʈ�� �����´�
	std::map<int, std::string>::iterator Start_it	= TagList.begin();
	std::map<int, std::string>::iterator End_it		= TagList.begin();
	int FindIndex = -1;

	//���� �̸����� �±��� �ε��� ��ȣ�� ã�´�
	for (Start_it; Start_it != End_it; Start_it++)
	{
		if (TagName == Start_it->second) 
		{
			FindIndex = Start_it->first;
		}
	}

	//�ε��� ��ȣ�� ��� ������Ʈ�� ��ȯ�� ���� ������Ʈ�� �����´�
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
	//Ȱ��ȭ ����
	data.Enabled = &mComponent->Enabled;
	//�Լ� ������
	data.FunctionPointer = std::bind(&Component::Awake, mComponent);
	//���۳�Ʈ ������
	data.ComponentPointer = mComponent;
	//���۳�Ʈ ����
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

		//���� �ڽ��� ���۳�Ʈ�� �־��ش�
		int ComponentMaxCount = obj->GetComponentCount();
		for (int j = 0; j < ComponentMaxCount; j++)
		{
			//���۳�Ʈ���� �Լ������ͷ� �־��ش�
			Component* cpt = obj->GetComponent(j);
			DontDestroyComponentSetting(cpt);
		}

		//�ڽ� �Ž� ������Ʈ�����
		for (int i = 0; i < ChildMeshCount; i++) 
		{
			GameObject* ChildMeshObject = obj->GetChildMesh(i);

			//���۳�Ʈ ����
			int ComponentMaxCount = ChildMeshObject->GetComponentCount();
			for (int j = 0; j < ComponentMaxCount; j++)
			{
				//���۳�Ʈ���� �Լ������ͷ� �־��ش�
				Component* cpt = ChildMeshObject->GetComponent(j);
				DontDestroyComponentSetting(cpt);
			}
		}
		
		//�ڽ� �� ������Ʈ�� ����
		for (int i = 0; i < ChildBoneCount; i++)
		{
			GameObject* ChildBoneObject = obj->GetChildBone(i);

			//���۳�Ʈ ����
			int ComponentMaxCount = ChildBoneObject->GetComponentCount();
			for (int j = 0; j < ComponentMaxCount; j++)
			{
				//���۳�Ʈ���� �Լ������ͷ� �־��ش�
				Component* cpt = ChildBoneObject->GetComponent(j);
				DontDestroyComponentSetting(cpt);
			}
		}
	}

	//��� �Ű����� ����Ʈ�� �ʱ�ȭ
	DontDestroyObjectList.clear();
}

void ObjectManager::DontDestroyComponentSetting(Component* cpt)
{
	//Awack,setup,Start �� ���� ������ �����Ǿ����� ���� ������ �־������ʴ´�

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
	//������Ʈ �Լ������Ϳ� �־��� �͵� ����
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


	///��¥ ���۳�Ʈ ����
	delete cpt;
	cpt = nullptr;
}
