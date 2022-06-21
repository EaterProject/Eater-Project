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
	};


	void Setting(GameObject* Object);

	void SetLimlightSetting(float R, float G, float B, float mFactor, float  mWidth);
	void SetLimlightSetting(COLOR_TYPE Type, float mFactor, float mWidth);

	void SetEmissiveSetting(float R, float G, float B, float mFactor);
	void SetEmissiveSetting(COLOR_TYPE Type, float mFactor);


	void SetLimlightColor(float R, float G, float B);
	void SetLimlightColorMax(float R, float G, float B);

	void SetLimlightFactor(float mFactor);
	void SetLimlightFactorMax(float mFactor);

	void SetLimlightWidth(float mWidth);
	void SetLimlightWidthMax(float mWidth);

	void LimLightUpdate();
	void EmissiveUpdate();
	void Update();
	void Default();
private:
	std::vector<MeshFilter*> MeshFilterList;
	std::vector<GameObject*> GameObjectList;
	std::vector<MaterialPropertyBlock*> MPBList;

	float LimColor[5]		= { 1.0f };
	float LimColorNow[5]	= { 1.0f };

	float EmissiveColor[4]		= { 1.0f };
	float EmissiveColorNow[4]	= { 1.0f };

	float DTime_R = 0.0f;
	float DTime_G = 0.0f;
	float DTime_B = 0.0f;

	bool IsUpDate = false;
	bool IsSetting = false;
	bool IsLimLightUpdate = false;
};

