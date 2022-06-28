#pragma once
#include <vector>
#include <string>
#include "Component.h"
#include "EngineData.h"

class Image;
class RectTransform;
class ImageFont :public Component
{
public:
	ImageFont();
	virtual ~ImageFont();

	void Awake() override;
	void SetUp() override;

	EATER_ENGINEDLL void SetTexture(std::string TextureName);				//넣고 싶은 숫자 이미지
	EATER_ENGINEDLL void SetColor(float R, float G, float B);				//컬러 값
	EATER_ENGINEDLL void SetColor(float R, float G, float B, float A);		//컬러 값
	EATER_ENGINEDLL void SetFontNumber(int Number);							//넣고 싶은 숫자
	EATER_ENGINEDLL void SetFontCount(int Number, bool FillZero = true);	//표현 하고 싶은 자리수
	EATER_ENGINEDLL void SetFontPivot(FONT_PIVOT pivot);					//중심점 위치
	EATER_ENGINEDLL void SetPosition(float X,float Y);						//위치
	EATER_ENGINEDLL void SetOffset(float FontOffset);						//글자 사이의 간격
	EATER_ENGINEDLL void SetScale(float X,float Y);							//글자 사이의 간격
	EATER_ENGINEDLL void SetPivot(PIVOT_TYPE type);							//글자 사이의 간격
	EATER_ENGINEDLL void SetPositionObject(GameObject* Object,Vector3 Pos);	//글자 사이의 간격
	EATER_ENGINEDLL void SetLayer(int Layer);								//글자 사이의 간격
	EATER_ENGINEDLL void SetActive(bool IsActive);							//글자 사이의 간격

private:
	void SetRelocation(RectTransform* tr, int now_index);

private:
	std::vector<Image*>			ImageList;
	std::vector<RectTransform*> TransformList;

	std::string	FontImageName;

	bool IsFillZero = true;
	bool IsCenter;
	int Center_Index;
	FONT_PIVOT PivotType;

	int FontCount	= 3;
	int FontNumber	= 0;
	float OffsetX	= 0.0f;
	float Half_OffsetX	= 0.0f;

	bool IsAwake = false;

	Vector4 mMF_Setting		= { 255,255,255,255};
	Vector2 mPosition	= {0,0};
	Vector2 mScale		= {1,1};
	PIVOT_TYPE mType;
};

