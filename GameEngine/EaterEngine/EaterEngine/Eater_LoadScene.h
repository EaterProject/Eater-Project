#pragma once
#include <string>
#include <map>
#include "Eater_Interface.h"

class GameObject;
class Eater_LoadScene :public Eater_Interface
{
public:
	Eater_LoadScene();
	~Eater_LoadScene();

	// Eater_Interface을(를) 통해 상속됨
	virtual void LoadData(std::string& Path) override;
private:
	void Load_Component_Transform(int index, GameObject* Object);
	void Load_Component_MeshFilter(int index, GameObject* Object);
	void Load_Component_Light(int index, GameObject* Object);
	void Load_Component_Particle(int index, GameObject* Object);
	void Load_Component_Collider(int index, GameObject* Object);
	void Load_Component_Rigidbody(int index, GameObject* Object);
	void Load_Component_Camera(int index, GameObject* Object);
	void Load_Component_Animation(int index, GameObject* Object);

};

