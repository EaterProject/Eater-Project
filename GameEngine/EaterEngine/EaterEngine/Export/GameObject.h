#pragma once

#include <string>
#include <vector>
#include <functional>

#include "EaterEngineDLL.h"
#include "Component.h"


#define AWAKE				0x00000001
#define START				0x00000010
#define SETUP				0x00000100
#define START_UPDATE		0x00001000
#define Transform_UPDATE	0x00010000
#define Physics_UPDATE		0x00100000
#define UPDATE				0x01000000
#define END_UPDATE			0x10000000
#define DEBUG_UPDATE		0x10000001

#define PHYS_TRIIGER_ENTER	0x00000001
#define PHYS_TRIIGER_STAY	0x00000010
#define PHYS_TRIIGER_EXIT	0x00000100


/// <summary>
/// �⺻ ���� ������Ʈ
/// </summary>
class MeshData;
class Material;
class Transform;
class Collider;
class GameObject
{
public:
	EATER_ENGINEDLL GameObject();
	~GameObject();

public:
	MeshData* OneMeshData;												//�׷��� �������� �Ѱ��� ������
	Transform* transform;												//�⺻������ �������ִ� Transform ���۳�Ʈ
	std::string Name;													//�̸�
public:
	EATER_ENGINEDLL void SetActive(bool active);						//��� ���۳�Ʈ ������� ����
	EATER_ENGINEDLL void SetStatic(bool active);						//������Ʈ ���� ����
	EATER_ENGINEDLL void SetShadow(bool active);						//������Ʈ �׸��� ����
	EATER_ENGINEDLL void SetCull(bool active);							//������Ʈ �ø� ����
	EATER_ENGINEDLL void SetDontDestroy(bool DontDestroy);				//���� �Ѿ�� ��������
	EATER_ENGINEDLL bool SetTag(std::string TagName);					//�±׸� �����Ѵ�
	EATER_ENGINEDLL bool SetTag(int TagNumber);							//�±׸� �����Ѵ�

	EATER_ENGINEDLL GameObject* GetChildBone(std::string Name);			//�ڽ� �� ��ü�� ������_�̸�
	EATER_ENGINEDLL GameObject* GetChildBone(int num);					//�ڽ� �� ��ü�� ������_�ε���
	EATER_ENGINEDLL GameObject* GetChildMesh(std::string Name);			//�ڽ� �Ž� ��ü�� ������_�̸�
	EATER_ENGINEDLL GameObject* GetChildMesh(int num);					//�ڽ� �Ž� ��ü�� ������_�ε���
	EATER_ENGINEDLL GameObject* GetChildObject(std::string Name);		//�ڽ� �Ž� , ���� ��� ã�Ƽ� ������Ʈ�� ������
	EATER_ENGINEDLL bool		GetDontDestroy();						//���� �Ǵ� ������Ʈ���� ���� ��ȯ
	EATER_ENGINEDLL int			GetTag();								//�±׸� �����´�
	EATER_ENGINEDLL UINT		GetIndex();								//�ε����� �����´�
	EATER_ENGINEDLL int			GetChildMeshCount();					//�ڽ� �Ž� ��ü�� ������ ������
	EATER_ENGINEDLL int			GetChildBoneCount();					//�ڽ� �� ��ü�� ������ ������
	EATER_ENGINEDLL	Transform*	GetTransform();							//�⺻ ���۳�Ʈ�� Transform�� ������
	EATER_ENGINEDLL Material*	GetMaterial();							//
	EATER_ENGINEDLL void ChoiceParent(GameObject* obj);					//���ڽ��� ������ ������Ʈ�� �ڽ����� �ִ´�
	EATER_ENGINEDLL void ChoiceChild(GameObject* obj);					//������ ������Ʈ�� ���� �ڽ����� �ִ´�
public:
	Component* GetDeleteComponent(int i);	//������ ���۳�Ʈ�� ������
	int GetComponentCount();				//������Ʈ�� ���۳�Ʈ ������ ������
	void PushChildList(GameObject* obj);	//������Ʈ�� �ڽ� ����Ʈ�� �־��ش�
	void ChildActive(bool Active);

	EATER_ENGINEDLL void PushChildMeshObject(GameObject* obj); //���� �ڽ����� �⺻ ���ӿ�����Ʈ�� �ִ´�
	EATER_ENGINEDLL void PushChildBoneObject(GameObject* obj); //���� �ڽ����� �⺻ ���ӿ�����Ʈ�� �ִ´�

public:
	///���۳�Ʈ�� �߰� ��Ų��
	template<typename T>
	T* AddComponent(typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type t = std::is_base_of<Component, T>::value);

	///���۳�Ʈ�� �����´�
	template<typename T>
	T* GetComponent(typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type t = std::is_base_of<Component, T>::value);
	Component* GetComponent(int index);

	///�ڽİ�ü���� ã�����ϴ� ���۳�Ʈ���ִٸ� �׿�����Ʈ�� ���۳�Ʈ�� ������
	template<typename T>
	T* GetChildComponent(typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type t = std::is_base_of<Component, T>::value);

	///������Ʈ�� ���۳�Ʈ�� �ִ��� üũ
	template<typename T>
	bool FindComponent(typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type t = std::is_base_of<Component, T>::value);
private:
	bool IsActive;				//��� ��������
	bool DontDestroy;			//���� ����
	unsigned int FunctionMask;	//� �Լ������Ϳ� �־����� ����
	int	 Tag;				//������Ʈ�� �ױ�
private:
	std::vector<Component*> ComponentList;
	std::vector<GameObject*> ChildMeshList;
	std::vector<GameObject*> ChildBoneList;

	EATER_ENGINEDLL void PushStartComponentFunction(Component* con, unsigned int type);
	EATER_ENGINEDLL void PushComponentFunction(Component* con, unsigned int type);
	EATER_ENGINEDLL void PushPhysFunction(Component* con, unsigned int type);
	
	void PlayPhysFunction(GameObject* Obj, unsigned int type);

	std::vector<std::function<void(GameObject*)>> OnTrigger_Enter_Function;
	std::vector<std::function<void(GameObject*)>> OnTrigger_Stay_Function;
	std::vector<std::function<void(GameObject*)>> OnTrigger_Exit_Function;

	//�浹 �Լ� ȣ�� 
	friend Collider;
};

template<typename T>
inline T* GameObject::AddComponent(typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type t)
{
	//���۳�Ʈ�� �̹� �ִٸ� �߰������ʴ´�
	bool isFind =  FindComponent<T>();
	if (isFind == true){return nullptr;}
	T* mComponent = new T();
	//���ӿ�����Ʈ ����
	mComponent->gameobject = this;
	//������ ���۳�Ʈ�� ����Ʈ�� �ִ´�
	ComponentList.push_back(mComponent);

	//���߿� ��Ÿ������ ã�Ƽ� �����ü��ֵ��� Ÿ�� ����
	mComponent->ComponentType = typeid(T).hash_code();

	///�������̵� Ȯ�� �������´� �Լ������� ����Ʈ�� �ִ´� 
	//Awake --- ���� ���۳�Ʈ���� �ٸ����۳�Ʈ�� �����ö�
	if (typeid(&Component::Awake).hash_code() != typeid(&T::Awake).hash_code())
	{
		PushComponentFunction(mComponent, AWAKE);
	}

	//SetUp  --- ������ ���۳�Ʈ�� �⺻�� �ʱ�ȭ
	if (typeid(&Component::SetUp).hash_code() != typeid(&T::SetUp).hash_code())
	{
		PushComponentFunction(mComponent, SETUP);
	}

	//Start  --- �ʱ�ȭ�� ������ ���۳�Ʈ�� ����
	if (typeid(&Component::Start).hash_code() != typeid(&T::Start).hash_code())
	{
		PushComponentFunction(mComponent, START);
	}

	//StartUpdate --- ���� ���� ����Ǵ� ������Ʈ 
	if (typeid(&Component::StartUpdate).hash_code() != typeid(&T::StartUpdate).hash_code())
	{
		PushComponentFunction(mComponent, START_UPDATE);
	}

	//�̵� ��� --- Transform ���۳�Ʈ�� �����ϴ� ������Ʈ
	if (typeid(&Component::TransformUpdate).hash_code() != typeid(&T::TransformUpdate).hash_code())
	{
		PushComponentFunction(mComponent, Transform_UPDATE);
	}

	//���� --- Phys ���� ������Ʈ
	if (typeid(&Component::PhysicsUpdate).hash_code() != typeid(&T::PhysicsUpdate).hash_code())
	{
		PushComponentFunction(mComponent, Physics_UPDATE);
	}

	//DefaultUpdate --- �⺻ ������Ʈ
	if (typeid(&Component::Update).hash_code() != typeid(&T::Update).hash_code())
	{
		PushComponentFunction(mComponent, UPDATE);
	}

	//EndUpdate --- ������ ������Ʈ
	if (typeid(&Component::EndUpdate).hash_code() != typeid(&T::EndUpdate).hash_code())
	{
		PushComponentFunction(mComponent, END_UPDATE);
	}

	if (typeid(&Component::Debug).hash_code() != typeid(&T::Debug).hash_code())
	{
		PushComponentFunction(mComponent, DEBUG_UPDATE);
	}

	///PhysX �浹 üũ ����
	if (typeid(&Component::OnTriggerEnter).hash_code() != typeid(&T::OnTriggerEnter).hash_code())
	{
		PushPhysFunction(mComponent, PHYS_TRIIGER_ENTER);
	}

	if (typeid(&Component::OnTriggerStay).hash_code() != typeid(&T::OnTriggerStay).hash_code())
	{
		PushPhysFunction(mComponent, PHYS_TRIIGER_STAY);
	}

	if (typeid(&Component::OnTriggerExit).hash_code() != typeid(&T::OnTriggerExit).hash_code())
	{
		PushPhysFunction(mComponent, PHYS_TRIIGER_EXIT);
	}


	return mComponent;
}

template<typename T>
inline T* GameObject::GetComponent(typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type t)
{
	//���� ���۳�Ʈ�� 2���������� ã�����Ѵ�
	int count = (int)ComponentList.size();
	for (int i = 0; i < count; i++)
	{
		//���� ���۳�Ʈ�� ã�����ϴ� ���۳�Ʈ�� Ÿ�� ��
		if (ComponentList[i]->ComponentType == typeid(T).hash_code())
		{
			//ã�� ���۳�Ʈ�� Ÿ�Ժ�ȯ
			T* temp = dynamic_cast<T*>(ComponentList[i]);
			return temp;
		}
	}

	//���� ã�����ߴٸ� �����ڵ带 �������ָ�ɵ�..
	return nullptr;
}

template<typename T>
inline T* GameObject::GetChildComponent(typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type t)
{
	//�ڽĿ�����Ʈ�� ���ٰ� ���� ����ã�ԵǴ� ���۳�Ʈ�� �����´�
	std::vector<GameObject*>::iterator it = ChildMeshList.begin();
	for (it; it != ChildMeshList.end(); it++)
	{
		int ComSize = (*it).ComponentList.size();
		for (int i = 0; i < ComSize; i++)
		{
			//���� ���۳�Ʈ�� ã�����ϴ� ���۳�Ʈ�� Ÿ�� ��
			if (ComponentList[i]->ComponentType == typeid(T).hash_code())
			{
				//ã�� ���۳�Ʈ�� Ÿ�Ժ�ȯ
				T* temp = dynamic_cast<T*>(ComponentList[i]);
				return temp;
			}
		}
	}
	return nullptr;
}

template<typename T>
inline bool GameObject::FindComponent(typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type t)
{
	int ComponentCount = (int)ComponentList.size();
	for (int i = 0; i < ComponentCount; i++)
	{
		//���� ���۳�Ʈ�� ã�����ϴ� ���۳�Ʈ�� Ÿ�� ��
		if (ComponentList[i]->ComponentType == typeid(T).hash_code())
		{
			return true;
		}
	}

	return false;
}
