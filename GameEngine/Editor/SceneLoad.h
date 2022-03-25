#pragma once
#include <string>
#include <map>
class GameObject;
class SceneLoad
{
public:
	SceneLoad();
	~SceneLoad();
	void Initialize(std::map<std::string, GameObject*>* ObjectList);
	void Load(std::string FilePath);
private:
	void Load_Component_Transform(int index, GameObject* Object);
	void Load_Component_MeshFilter(int index, GameObject* Object);
	void Load_Component_Skinning(int index, GameObject* Object);
	void Load_Component_Light(int index, GameObject* Object);
	void Load_Component_Particle(int index, GameObject* Object);
	void Load_Component_Collider(int index, GameObject* Object);
	void Load_Component_Rigidbody(int index, GameObject* Object);
	void Load_Component_Camera(int index, GameObject* Object);
	void Load_Component_Animation(int index, GameObject* Object);

	void CreateStaticObject(int index, GameObject* Object);
	void CreateSkinObject(int index, GameObject* Object);

	std::map<std::string, GameObject*>* LoadObject;
};

