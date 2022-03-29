#pragma once

#include <map>
#include <vector>
#include <queue>
#include <windows.h>
#include "EngineData.h"
#include <functional>
#include "Delegate.h"
#include "EaterEngineDLL.h"
#include "BaseManager.h"
typedef size_t Hash_Code;

class Camera;
class FBXModel;
class GameObject;
class Component;
class MeshFilter;

class ObjectManager : public BaseManager
{
public:
	~ObjectManager();
	ObjectManager();

	//삭제할 오브젝트를 넣어줌(이함수를 실행시킬단계에서 오브젝트를 삭제하지않음 삭제는 가장 마지막에)
	void PushDeleteObject(GameObject* obj);

	//현재 오브젝트 리스트에 담겨있는 오브젝트와 컨퍼넌트 리스트를 전부 삭제
	void AllDeleteObject();

	//엔진 초기화
	void Initialize();

	//초기화 단계
	static void PushStart(Component* obj,int Order);
	static void PushStartPlay(Component* obj, int Order);
	static void PushAwake(Component* obj, int Order);

	// 컨퍼넌트의 업데이트 함수를 넣어준다
	static void PushStartUpdate(Component* obj, int Order);
	static void PushTransformUpdate(Component* obj, int Order);
	static void PushPhysicsUpdate(Component* obj, int Order);
	static void PushEndUpdate(Component* obj, int Order);
	static void PushUpdate(Component* obj, int Order);

	static void AddTag(int Key,std::string TagName);
	static int FindTag(std::string TagName);
	//static void DeleteTag(int TagNumber);

	// 최초 한번 호출되는 함수 리스트를 실행시킴
	void InitializeFuntion();
	// 업데이트 함수 리스트를 실행시킴
	void PlayUpdate();
	// 업데이트와 랜더링 함수리스트를 모두 삭제함
	void ClearFunctionList();
	//오브젝트를 삭제한다
	void DeleteObject();
	//삭제하면 안되는 오브젝트들을 다시 세팅해준다
	void DontDestroyObjectSetting();

	GameObject* FindGameObjectString(std::string& TagName);
	GameObject* FindGameObjectTag(std::string& TagName);
	void FindGameObjectTags(std::string& TagName, std::vector<GameObject*>* ObjectList);

private:
	//생성한 오브젝트를 넣어줌
	static void PushCreateObject(GameObject* obj);

private:
	//컨퍼넌트 데이터를 텀겨준다
	static ComponentFunctionData PushComponentData(Component*);
	void DontDestroyComponentSetting(Component* Com);
	///오브젝트 태그 리스트
	static std::map<int,std::string> TagList;
	///오브젝트 리스트
	static std::vector<GameObject*> ObjectList;
	///삭제되면 안되는 오브젝트 리스트
	std::vector<GameObject*> DontDestroyObjectList;
	///랜더링까지 모두끝나고 마지막에 삭제될 오브젝트리스트
	std::queue<GameObject*> DeleteList;

	/// 시작단계 한번만 실행됨
	static Delegate_Map AwakeFunction;		//시작단계 컨퍼넌트 가져오는 함수
	static Delegate_Map StartFunction;		//시작단계의 초기화 함수
	static Delegate_Map SetUpFunction;		//시작단계의 실행되는 함수


	/// 업데이트 단계 프레임마다 실행
	static Delegate_Map StartUpdate;		//가장먼저 시작되는 업데이트
	static Delegate_Map TransformUpdate;	//이동 행렬 업데이트
	static Delegate_Map PhysicsUpdate;		//물리 행동 업데이트
	static Delegate_Map Update;				//디폴트  중간단계의 시작되는 업데이트
	static Delegate_Map EndUpdate;			//가장 마지막에 실행되는 업데이트

	//컨퍼넌트 를넣으면 해당 함수포인터에 넣었던 포인터를 삭제시켜줌
	void DeleteComponent(Component* cpt);

private:
	friend class GameObject;
};