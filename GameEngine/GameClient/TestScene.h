#pragma once
#include "Scene.h"

class GameObject;
class Transform;
class MeshFilter;
class Rigidbody;
class AnimationController;
class ParticleSystem;
class Camera;
class Image;
class Slider;
class RectTransform;
class MiniMapSystem;
class Button;
class ParticleController;
class TextureBuffer;
class MaterialPropertyBlock;

class TestScene : public Scene
{
public:
	TestScene() = default;
	~TestScene() = default;

public:
	void Awake() override;
	void Update() override;
	void End() override;

	void CreateMap();
	void ChangeCubeMap();

	void InButton();
	void OutButton();
	void DownButton();
	void UpButton();
	void PressDownButton();

public:
	/// 각각의 생성 예시용
	void CreateMiniMap();
	void CreateButton();
	void CreateParicleController();
	void CreateDissolve();

	void SetColorGrading();
	void SetScreenBlur();


private:
	void SetMaterialBlock(GameObject* object, std::vector<MaterialPropertyBlock*>& blocklist, bool alpha);

private:
	GameObject* Object;
	Transform* Tr;
	MeshFilter* Filter;
	AnimationController* AC;

	MiniMapSystem* m_MiniMap;

	GameObject* m_Mana;
	GameObject* m_Boss;
	GameObject* m_MonsterA;
	GameObject* m_MonsterB;

	MeshFilter* m_ManaFilter;
	MeshFilter* m_BossFilter;
	MeshFilter* m_MonsterAFilter;
	MeshFilter* m_MonsterBFilter;

	std::vector<TextureBuffer*> m_NoiseTextureList;
	int TextureIndex = 0;
	int TextureCount = 16;

	int BlurCount = 1;

	float BlendFactor = 0.0f;

	std::vector<MaterialPropertyBlock*> m_ManaBlock;
	std::vector<MaterialPropertyBlock*> m_BossBlock;
	std::vector<MaterialPropertyBlock*> m_MonsterABlock;
	std::vector<MaterialPropertyBlock*> m_MonsterBBlock;

	Transform* m_Controller;
	ParticleController* m_ParticleController;
	std::vector<ParticleController*> m_ParticleControllerList;

	int ControllerIndex = 0;

	RectTransform* Rect_Tr;
	Button* button;

	std::vector<Transform*> TRList;
	std::vector<AnimationController*> ACList;
};

