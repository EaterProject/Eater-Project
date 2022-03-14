#pragma once
#include <functional>
#include <map>
#include <string>
#include <thread>
class Scene;
class ObjectManager;

/// <summary>
/// ��ũ���� ���� ���ִ� ��ũ�� �Ŵ���
/// </summary>


class SceneManager
{
public:
	~SceneManager();
	SceneManager();

	//��ũ�� �Ŵ��� �ʱ�ȭ
	void Initialize(ObjectManager* ObjManager);

	//������ �� �ֱ�
	void PushScene(Scene* mScene,std::string SceneName);

	//�� ����
	void ChoiceScene(std::string SceneName);

	//���� Awack Start�Լ��� ����
	void SceneStart();

	//���� �ٸ������� �Ѿ���� ������ ����
	void SceneEnd();

	//��ũ�� �Ŵ��� ������Ʈ
	void Update();		

	//�Ŵ��� ����
	void Delete();

private:
	Scene* FindScene(std::string& Name);

	//���� ��ũ��
	Scene* NowScene;
	std::string SceneName;

	//��ũ�� ����Ʈ
	std::map<std::string, Scene*> SceneList;
private:
	ObjectManager* mObjManager;
	std::thread SubThread;
};