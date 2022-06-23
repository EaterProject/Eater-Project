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
/// 기본 게임 오브젝트
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
	MeshData* OneMeshData;												//그래픽 엔진으로 넘겨줄 데이터
	Transform* transform;												//기본적으로 생성해주는 Transform 컨퍼넌트
	std::string Name;													//이름
public:
	EATER_ENGINEDLL void SetActive(bool active);						//모든 컨퍼넌트 기능중지 여부
	EATER_ENGINEDLL void SetStatic(bool active);						//오브젝트 정적 여부
	EATER_ENGINEDLL void SetShadow(bool active);						//오브젝트 그림자 여부
	EATER_ENGINEDLL void SetCull(bool active);							//오브젝트 컬링 여부
	EATER_ENGINEDLL void SetDontDestroy(bool DontDestroy);				//씬이 넘어갈때 삭제여부
	EATER_ENGINEDLL bool SetTag(std::string TagName);					//태그를 지정한다
	EATER_ENGINEDLL bool SetTag(int TagNumber);							//태그를 지정한다

	EATER_ENGINEDLL GameObject* GetChildBone(std::string Name);			//자식 본 객체를 가져옴_이름
	EATER_ENGINEDLL GameObject* GetChildBone(int num);					//자식 본 객체를 가져옴_인덱스
	EATER_ENGINEDLL GameObject* GetChildMesh(std::string Name);			//자식 매쉬 객체를 가져옴_이름
	EATER_ENGINEDLL GameObject* GetChildMesh(int num);					//자식 매쉬 객체를 가져옴_인덱스
	EATER_ENGINEDLL GameObject* GetChildObject(std::string Name);		//자식 매쉬 , 본을 모두 찾아서 오브젝트를 가져옴
	EATER_ENGINEDLL bool		GetDontDestroy();						//삭제 되는 오브젝트인지 여부 반환
	EATER_ENGINEDLL int			GetTag();								//태그를 가져온다
	EATER_ENGINEDLL UINT		GetIndex();								//인덱스를 가져온다
	EATER_ENGINEDLL int			GetChildMeshCount();					//자식 매쉬 객체의 개수를 가져옴
	EATER_ENGINEDLL int			GetChildBoneCount();					//자식 본 객체의 개수를 가져옴
	EATER_ENGINEDLL	Transform*	GetTransform();							//기본 컨퍼넌트인 Transform을 가져옴
	EATER_ENGINEDLL Material*	GetMaterial();							//
	EATER_ENGINEDLL void ChoiceParent(GameObject* obj);					//나자신을 선택한 오브젝트의 자식으로 넣는다
	EATER_ENGINEDLL void ChoiceChild(GameObject* obj);					//선택한 오브젝트를 나의 자식으로 넣는다
public:
	Component* GetDeleteComponent(int i);	//삭제할 컨퍼넌트를 가져옴
	int GetComponentCount();				//오브젝트의 컨퍼넌트 갯수를 가져옴
	void PushChildList(GameObject* obj);	//오브젝트를 자식 리스트에 넣어준다
	void ChildActive(bool Active);

	EATER_ENGINEDLL void PushChildMeshObject(GameObject* obj); //나의 자식으로 기본 게임오브젝트를 넣는다
	EATER_ENGINEDLL void PushChildBoneObject(GameObject* obj); //나의 자식으로 기본 게임오브젝트를 넣는다

public:
	///컨퍼넌트를 추가 시킨다
	template<typename T>
	T* AddComponent(typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type t = std::is_base_of<Component, T>::value);

	///컨퍼넌트를 가져온다
	template<typename T>
	T* GetComponent(typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type t = std::is_base_of<Component, T>::value);
	Component* GetComponent(int index);

	///자식객체에서 찾고자하는 컨퍼넌트가있다면 그오브젝트에 컨퍼넌트를 가져옴
	template<typename T>
	T* GetChildComponent(typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type t = std::is_base_of<Component, T>::value);

	///오브젝트에 컨퍼넌트가 있는지 체크
	template<typename T>
	bool FindComponent(typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type t = std::is_base_of<Component, T>::value);
private:
	bool IsActive;				//기능 중지여부
	bool DontDestroy;			//삭제 여부
	unsigned int FunctionMask;	//어떤 함수포인터에 넣었는지 여부
	int	 Tag;				//오브젝트의 테그
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

	//충돌 함수 호출 
	friend Collider;
};

template<typename T>
inline T* GameObject::AddComponent(typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type t)
{
	//컨퍼넌트가 이미 있다면 추가하지않는다
	bool isFind =  FindComponent<T>();
	if (isFind == true){return nullptr;}
	T* mComponent = new T();
	//게임오브젝트 설정
	mComponent->gameobject = this;
	//생성한 컨퍼넌트를 리스트에 넣는다
	ComponentList.push_back(mComponent);

	//나중에 이타입으로 찾아서 가져올수있도록 타입 설정
	mComponent->ComponentType = typeid(T).hash_code();

	///오버라이딩 확인 각각에맞는 함수포인터 리스트에 넣는다 
	//Awake --- 현재 컨퍼넌트에서 다른컨퍼넌트를 가져올때
	if (typeid(&Component::Awake).hash_code() != typeid(&T::Awake).hash_code())
	{
		PushComponentFunction(mComponent, AWAKE);
	}

	//SetUp  --- 가져온 컨퍼넌트들 기본값 초기화
	if (typeid(&Component::SetUp).hash_code() != typeid(&T::SetUp).hash_code())
	{
		PushComponentFunction(mComponent, SETUP);
	}

	//Start  --- 초기화된 값으로 컨퍼넌트값 적용
	if (typeid(&Component::Start).hash_code() != typeid(&T::Start).hash_code())
	{
		PushComponentFunction(mComponent, START);
	}

	//StartUpdate --- 가장 먼저 실행되는 업데이트 
	if (typeid(&Component::StartUpdate).hash_code() != typeid(&T::StartUpdate).hash_code())
	{
		PushComponentFunction(mComponent, START_UPDATE);
	}

	//이동 행렬 --- Transform 컨퍼넌트가 동작하는 업데이트
	if (typeid(&Component::TransformUpdate).hash_code() != typeid(&T::TransformUpdate).hash_code())
	{
		PushComponentFunction(mComponent, Transform_UPDATE);
	}

	//물리 --- Phys 관련 업데이트
	if (typeid(&Component::PhysicsUpdate).hash_code() != typeid(&T::PhysicsUpdate).hash_code())
	{
		PushComponentFunction(mComponent, Physics_UPDATE);
	}

	//DefaultUpdate --- 기본 업데이트
	if (typeid(&Component::Update).hash_code() != typeid(&T::Update).hash_code())
	{
		PushComponentFunction(mComponent, UPDATE);
	}

	//EndUpdate --- 마지막 업데이트
	if (typeid(&Component::EndUpdate).hash_code() != typeid(&T::EndUpdate).hash_code())
	{
		PushComponentFunction(mComponent, END_UPDATE);
	}

	if (typeid(&Component::Debug).hash_code() != typeid(&T::Debug).hash_code())
	{
		PushComponentFunction(mComponent, DEBUG_UPDATE);
	}

	///PhysX 충돌 체크 전용
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
	//같은 컨퍼넌트가 2개가있으면 찾지못한다
	int count = (int)ComponentList.size();
	for (int i = 0; i < count; i++)
	{
		//현재 컨퍼넌트와 찾고자하는 컨퍼넌트의 타입 비교
		if (ComponentList[i]->ComponentType == typeid(T).hash_code())
		{
			//찾은 컨퍼넌트를 타입변환
			T* temp = dynamic_cast<T*>(ComponentList[i]);
			return temp;
		}
	}

	//만약 찾지못했다면 에러코드를 내보내주면될듯..
	return nullptr;
}

template<typename T>
inline T* GameObject::GetChildComponent(typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type t)
{
	//자식오브젝트를 돌다가 가장 먼저찾게되는 컨퍼넌트를 가져온다
	std::vector<GameObject*>::iterator it = ChildMeshList.begin();
	for (it; it != ChildMeshList.end(); it++)
	{
		int ComSize = (*it).ComponentList.size();
		for (int i = 0; i < ComSize; i++)
		{
			//현재 컨퍼넌트와 찾고자하는 컨퍼넌트의 타입 비교
			if (ComponentList[i]->ComponentType == typeid(T).hash_code())
			{
				//찾은 컨퍼넌트를 타입변환
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
		//현재 컨퍼넌트와 찾고자하는 컨퍼넌트의 타입 비교
		if (ComponentList[i]->ComponentType == typeid(T).hash_code())
		{
			return true;
		}
	}

	return false;
}
