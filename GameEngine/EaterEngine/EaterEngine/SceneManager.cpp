#include "SceneManager.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Profiler/Profiler.h"

SceneManager::SceneManager()
{
	NowScene	= nullptr;
	mObjManager = nullptr;
}

SceneManager::~SceneManager()
{

}

void SceneManager::PushScene(Scene* mScene, std::string SceneName)
{
	SceneList.insert({ SceneName,mScene});
}

void SceneManager::Initialize(ObjectManager* ObjManager)
{
	mObjManager = ObjManager;
}

void SceneManager::ChoiceScene(std::string mSceneName)
{
	Scene* Choice = FindScene(mSceneName);

	//만약 현재 정해진 씬이없다면
	//즉 처음 씬을 선택하는 상황이라면
	if (NowScene == nullptr)
	{
		//삭제 없이 그냥 씬을 선택
		NowScene = Choice;
	}
	else
	{
		//씬의 End 함수를 실행시키고 다른씬으로 변경
		NowScene->End();

		NowScene = nullptr;

		//모든 오브젝트를 삭제한다
		mObjManager->AllDeleteObject();

		//함수 포인터 리스트들을 리셋
		mObjManager->ClearFunctionList();

		//삭제하면 안되는 오브젝트들 다시 셋팅
		mObjManager->DontDestroyObjectSetting();

		NowScene = Choice;
	}
}

void SceneManager::Delete()
{
	NowScene = nullptr;

	//모든 스크린 삭제
	SceneList.clear();
}

Scene* SceneManager::FindScene(std::string& Name)
{
	//씬을 찾지 못했을 경우
	if (SceneList.find(Name) == SceneList.end())
	{
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Scene ] '%s Scene'을 찾지 못했습니다", Name.c_str());
		return nullptr;
	}
	else
	{
		return SceneList[Name];
	}
}

void SceneManager::SceneStart()
{
	NowScene->Awake();

	//쓰레드가 실행되지않을때
	while (true)
	{
		//쓰레드 활성화 여부
		if (SubThread.joinable() == false)
		{
			//활성화가 되어있지않다면 새로운 함수를넣어주고 쓰레드를 돌린다
			PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Thread ][ Load ] 데이터 로드 시작");
			PROFILE_TIMER_START(PROFILE_OUTPUT::CONSOLE, 1, "Thread Load");
			SubThread = std::thread(std::bind(&Scene::Run, NowScene));
			PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Thread ][ Load ] 데이터 로드 완료");
			PROFILE_TIMER_END("Thread Load");

			break;
		}
		else
		{
			//끝날때 까지 대기
			SubThread.join();
		}

		Sleep(0);
	}
}

void SceneManager::SceneEnd()
{
	if (NowScene != nullptr)
	{
		NowScene->End();
	}
}

void SceneManager::Update()
{
	NowScene->Update();
}


