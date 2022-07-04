#pragma once
#include <vector>
#include "EngineData.h"

class MeshFilter;
class GameObject;
class MaterialPropertyBlock;
class DissolveManager;

class MeshFilterSetting
{
public:
	MeshFilterSetting();
	~MeshFilterSetting();

public:
	enum class COLOR_TYPE
	{
		RED,
		GREEN,
		BLUE,
		WHITE
	};

public:
	static void Initialize();
	static void Release();

public:
	void Setting(GameObject* Object);

	void SetLimlightSetting(float R, float G, float B, float mFactor, float  mWidth);
	void SetLimlightSetting(COLOR_TYPE Type, float mFactor, float mWidth);
	void SetLimlightSettingMax(float R, float G, float B, float mFactor, float  mWidth);
	void SetLimlightSettingMax(COLOR_TYPE Type, float mFactor, float mWidth);

	void SetEmissiveSetting(float R, float G, float B, float mFactor);
	void SetEmissiveSetting(COLOR_TYPE Type, float mFactor);
	void SetEmissiveSettingMax(float R, float G, float B, float mFactor);
	void SetEmissiveSettingMax(COLOR_TYPE Type, float mFactor);

	void SetDissolveTexture(std::string& mTexture);
	void SetDissolveTexture(std::string&& mTexture);
	void SetDissolveOption(DISSOLVE_OPTION mOption);

	void LimLightUpdate(float Speed);
	void EmissiveUpdate(float Speed);

	void Default();
	void ReSet();

	//������ �ɼ�
	void SetDissolveColor(Vector3 mColor);
	void SetDissolveColor(float R,float G,float B);
	void SetDissolveColorFactor(float mFactor);
	void SetDissolvePlayTime(float mTime);
	void SetDissolveWidth(float mWidth);
	void SetDissolveOuterFactor(float mFactor);
	void SetDissolveInnerFactor(float mFactor);
	void PlayDissolve();
	const bool EndDissolve();
	
private:
	GameObject* mTopObject = nullptr;

	void LimLightUpdate();
	void EmissiveUpdate();

	std::vector<MeshFilter*> MeshFilterList;
	std::vector<GameObject*> GameObjectList;
	std::vector<MaterialPropertyBlock*> MPBList;

	static DissolveManager* mDissolveManager;

	float LimColor[5]		= { 1.0f };
	float LimColorMax[5]	= { 1.0f };
	int LimColorValue[5];		//���� ���� ����
	int LImColorOneFrame[5];	//�������� ������
	
	float EmissiveColor[4]		= { 1.0f };
	float EmissiveColorMax[4]	= { 1.0f };
	int EmissiveValue[4];		//���� ���� ����
	int EmissiveOneFrame[4];	//�������� ������

	
	bool IsSetting_LimLight = false;
	bool IsSetting_Emissive = false;

	const int TYPE_UP	= 0;	//���� ����
	const int TYPE_DOWN = 1;	//���� ����
	const int TYPE_NONE = 2;	//���� ��ȭ�� ����
	

	//������ ����
	DISSOLVE_OPTION mDissolveOption;
	std::string mDissolveName;
	Vector3 mDissolveColor;
	float DissolveColorFactor;

	//������ �ɼ� ��
	float DissolvePlayTime		= 1.0f;
	float DissolveOuterFactor	= 1.0f;
	float DissolveInnerFactor	= 1.0f;
	float DissolveWidth			= 1.0f;
	bool* IsDissolvePlay		= nullptr;
};

