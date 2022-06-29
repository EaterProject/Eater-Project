#pragma once
#include <vector>
#include "EngineData.h"

class MeshFilter;
class GameObject;
class MaterialPropertyBlock;
class MeshFilterSetting
{
public:
	MeshFilterSetting();
	~MeshFilterSetting();

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
	void SetEmissiveSettingMax(float R, float G, float B, float mFactor);
	void SetEmissiveSettingMax(COLOR_TYPE Type, float mFactor);

	void SetDissolveSetting(DISSOLVE_OPTION mOption,std::string& TextureName);
	//void SetDissolveSetting(DISSOLVE_OPTION mOption,std::string& TextureName);


	//void SetDissolve(DISSOLVE_OPTION mOption, std::string& TextureName,Vector3& mColor);
	//void SetDissolve(DISSOLVE_OPTION mOption, std::string&& TextureName, Vector3&& mColor);

	void LimLightUpdate(float Speed);
	void EmissiveUpdate(float Speed);



	void Default();
	void Release();
private:
	void LimLightUpdate();
	void EmissiveUpdate();
	std::vector<MeshFilter*> MeshFilterList;
	std::vector<GameObject*> GameObjectList;
	std::vector<MaterialPropertyBlock*> MPBList;

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



	DISSOLVE_OPTION mDissolveOption;
	std::string mDissolveName;
	float DissolveColor[3];

};

