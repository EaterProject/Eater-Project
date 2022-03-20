#pragma once

#include "Eater_Interface.h"
class GameObject;
class Eater_LoadScene : Eater_Interface
{
public:
	Eater_LoadScene();
	virtual ~Eater_LoadScene();

	// Eater_Interface을(를) 통해 상속됨
	virtual void LoadData(std::string& Path) override;
private:
	//씬파일을 로드함
	void CreateStaticObject(int index, GameObject* Object);					//스테틱 오브젝트를 생성한다
	void CreateSkinObject(int index, GameObject* Object);					//스킨 오브젝트를 생성한다
	void CreateParticleObject(int index, GameObject* Object);				//파티클 오브젝트를 생성한다

	void Load_Component_MeshFilter(int index, GameObject* Object);			//MeshFilter Load
	void Load_Component_Transform(int index, GameObject* Object);			//
	void Load_Component_Skinning(int index, GameObject* Object);			//Transform Load
	void LoadGameObject(int index);
};

