#pragma once

#include "Eater_Interface.h"
class GameObject;
class Eater_LoadScene : Eater_Interface
{
public:
	Eater_LoadScene();
	virtual ~Eater_LoadScene();

	// Eater_Interface��(��) ���� ��ӵ�
	virtual void LoadData(std::string& Path) override;
private:
	//�������� �ε���
	void CreateStaticObject(int index, GameObject* Object);					//����ƽ ������Ʈ�� �����Ѵ�
	void CreateSkinObject(int index, GameObject* Object);					//��Ų ������Ʈ�� �����Ѵ�
	void CreateParticleObject(int index, GameObject* Object);				//��ƼŬ ������Ʈ�� �����Ѵ�

	void Load_Component_MeshFilter(int index, GameObject* Object);			//MeshFilter Load
	void Load_Component_Transform(int index, GameObject* Object);			//
	void Load_Component_Skinning(int index, GameObject* Object);			//Transform Load
	void LoadGameObject(int index);
};

