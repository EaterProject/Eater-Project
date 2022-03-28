#pragma once
#include <map>
#include <string>

class GameObject;
class Transform;
class MeshFilter;
class Light;
class Camera;
class Collider;
class Rigidbody;
class AnimationController;
class ParticleSystem;

class SceneSave
{
public:
	SceneSave();
	~SceneSave();
	void Initialize(std::map<std::string, GameObject*>* ObjectList);

	void Save(std::string SaveFilePath,std::string SaveFileName);
private:
	void SceneOption();
	void SaveTransform(Transform* Obj);
	void SaveMeshFilter(MeshFilter* Obj);
	void SaveAnimation(AnimationController* Obj);
	void SaveLight(Light* Obj);
	void SaveParticle(ParticleSystem* Obj);
	void SaveCollider(Collider* Obj);
	void SaveRigidbody(Rigidbody* Obj);
	void SaveCamera(Camera* Obj);


	std::map<std::string, GameObject*>* SaveList;
};

