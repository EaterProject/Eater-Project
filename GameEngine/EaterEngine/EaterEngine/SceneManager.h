#pragma once
#include <functional>
#include <map>
#include <string>
#include <thread>
class Scene;
class ObjectManager;

/// <summary>
/// 스크린을 관리 해주는 스크린 매니저
/// </summary>


class SceneManager
{
public:
	~SceneManager();
	SceneManager();

	//스크린 매니저 초기화
	void Initialize(ObjectManager* ObjManager);

	//생성된 씬 넣기
	void PushScene(Scene* mScene,std::string SceneName);

	//씬 선택
	void ChoiceScene(std::string SceneName);

	//씬의 Awack Start함수를 실행
	void SceneStart();

	//씬이 다른씬으로 넘어가기전 마무리 실행
	void SceneEnd();

	//스크린 매니저 업데이트
	void Update();		

	//매니저 삭제
	void Delete();

private:
	Scene* FindScene(std::string& Name);

	//현재 스크린
	Scene* NowScene;
	std::string SceneName;

	//스크린 리스트
	std::map<std::string, Scene*> SceneList;
private:
	ObjectManager* mObjManager;
	std::thread SubThread;
};