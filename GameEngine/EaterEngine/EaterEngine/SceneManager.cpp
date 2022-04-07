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

	//���� ���� ������ ���̾��ٸ�
	//�� ó�� ���� �����ϴ� ��Ȳ�̶��
	if (NowScene == nullptr)
	{
		//���� ���� �׳� ���� ����
		NowScene = Choice;
	}
	else
	{
		//���� End �Լ��� �����Ű�� �ٸ������� ����
		NowScene->End();

		NowScene = nullptr;

		//��� ������Ʈ�� �����Ѵ�
		mObjManager->AllDeleteObject();

		//�Լ� ������ ����Ʈ���� ����
		mObjManager->ClearFunctionList();

		//�����ϸ� �ȵǴ� ������Ʈ�� �ٽ� ����
		mObjManager->DontDestroyObjectSetting();

		NowScene = Choice;
	}
}

void SceneManager::Delete()
{
	NowScene = nullptr;

	//��� ��ũ�� ����
	SceneList.clear();
}

Scene* SceneManager::FindScene(std::string& Name)
{
	//���� ã�� ������ ���
	if (SceneList.find(Name) == SceneList.end())
	{
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Scene ] '%s Scene'�� ã�� ���߽��ϴ�", Name.c_str());
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

	//�����尡 �������������
	while (true)
	{
		//������ Ȱ��ȭ ����
		if (SubThread.joinable() == false)
		{
			//Ȱ��ȭ�� �Ǿ������ʴٸ� ���ο� �Լ����־��ְ� �����带 ������
			PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Thread ][ Load ] ������ �ε� ����");
			PROFILE_TIMER_START(PROFILE_OUTPUT::CONSOLE, 1, "Thread Load");
			SubThread = std::thread(std::bind(&Scene::Run, NowScene));
			PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Thread ][ Load ] ������ �ε� �Ϸ�");
			PROFILE_TIMER_END("Thread Load");

			break;
		}
		else
		{
			//������ ���� ���
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


