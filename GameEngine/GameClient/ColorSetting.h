#pragma once
#include <vector>

class MeshFilter;
class GameObject;
class MaterialPropertyBlock;
class ColorSetting
{
public:
	ColorSetting();
	~ColorSetting();

	enum class COLOR_TYPE
	{
		RED,
		GREEN,
		BLUE,
		WHITE
	};


	void Setting(GameObject* Object);

	void SetLimlightSetting(float R, float G, float B, float mFactor, float  mWidth);
	void SetLimlightSetting(COLOR_TYPE Type, float mFactor, float mWidth);
	void SetLimlightSettingMax(float R, float G, float B, float mFactor, float  mWidth);
	void SetLimlightSettingMax(COLOR_TYPE Type, float mFactor, float mWidth);





	void SetEmissiveSetting(float R, float G, float B, float mFactor);
	void SetEmissiveSetting(COLOR_TYPE Type, float mFactor);

	void Update(float Speed);
	void Default();
private:

	void LimLightUpdate();
	void EmissiveUpdate();
	std::vector<MeshFilter*> MeshFilterList;
	std::vector<GameObject*> GameObjectList;
	std::vector<MaterialPropertyBlock*> MPBList;

	float LimColor[5]		= { 1.0f };
	float LimColorMax[5]	= { 1.0f };

	float EmissiveColor[4]		= { 1.0f };
	float EmissiveColorMax[4]	= { 1.0f };

	int LimColorValue[5];		//���� ���� ����
	int LImColorOneFrame[5];	//�������� ������
	bool IsLimLightColor_Setting	= false;
	bool IsLimLightColorMax_Setting = false;
	bool IsTypeSetting				= false;


	const int TYPE_UP	= 0;	//���� ����
	const int TYPE_DOWN = 1;	//���� ����
	const int TYPE_NONE = 2;	//���� ��ȭ�� ����

	bool IsUpDate = false;
	bool IsSetting = false;
	bool IsLimLightUpdate = false;
};

