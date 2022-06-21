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

	void Setting(GameObject* Object);

	void SetLimlightSetting(float R, float G, float B, float mFactor, float  mWidth);
	void SetEmissiveSetting(float R, float G, float B, float mFactor, float  mWidth);


	void SetLimlightColor(float R, float G, float B);
	void SetLimlightColorMax(float R, float G, float B);

	void SetLimlightFactor(float mFactor);
	void SetLimlightFactorMax(float mFactor);

	void SetLimlightWidth(float mWidth);
	void SetLimlightWidthMax(float mWidth);

	void LimLightUpdate();
	void EmissiveUpdate();
	void Default();
private:
	std::vector<MeshFilter*> MeshFilterList;
	std::vector<GameObject*> GameObjectList;
	std::vector<MaterialPropertyBlock*> MPBList;

	float LimColor[5]		= { 0.0f };
	float LimColorNow[5]	= { 0.0f };

	float EmissiveColor[5]		= { 0.0f };
	float EmissiveColorNow[5]	= { 0.0f };

	bool IsUpDate = false;
	bool IsSetting = false;
};

