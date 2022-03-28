#pragma once
#include <string>
#include <map>
class GameObject;
class Eater_LoadScene
{
public:
	Eater_LoadScene();
	~Eater_LoadScene();
	void Load(std::string FilePath);
	void Initialize(std::map<std::string, GameObject*>* ObjectList);
	void LoadData(std::string& Paht);
private:
	void Load_Component_Transform(int index, GameObject* Object);
	void Load_Component_MeshFilter(int index, GameObject* Object);
	void Load_Component_Light(int index, GameObject* Object);
	void Load_Component_Particle(int index, GameObject* Object);
	void Load_Component_Collider(int index, GameObject* Object);
	void Load_Component_Rigidbody(int index, GameObject* Object);
	void Load_Component_Camera(int index, GameObject* Object);
	void Load_Component_Animation(int index, GameObject* Object);

	std::map<std::string, GameObject*>* LoadObject;
};

