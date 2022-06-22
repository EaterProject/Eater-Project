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

	int LimColorValue[5];		//증가 감소 측정
	int LImColorOneFrame[5];	//한프레임 증가량
	bool IsLimLightColor_Setting	= false;
	bool IsLimLightColorMax_Setting = false;
	bool IsTypeSetting				= false;


	const int TYPE_UP	= 0;	//값이 증가
	const int TYPE_DOWN = 1;	//값이 감소
	const int TYPE_NONE = 2;	//값이 변화량 없음

	bool IsUpDate = false;
	bool IsSetting = false;
	bool IsLimLightUpdate = false;
};

