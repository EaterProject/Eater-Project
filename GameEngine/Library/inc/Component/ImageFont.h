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

	EATER_ENGINEDLL void SetTexture(std::string TextureName);				//�ְ� ���� ���� �̹���
	EATER_ENGINEDLL void SetColor(float R, float G, float B);				//�÷� ��
	EATER_ENGINEDLL void SetColor(float R, float G, float B, float A);		//�÷� ��
	EATER_ENGINEDLL void SetFontNumber(int Number);							//�ְ� ���� ����
	EATER_ENGINEDLL void SetFontCount(int Number, bool FillZero = true);	//ǥ�� �ϰ� ���� �ڸ���
	EATER_ENGINEDLL void SetFontPivot(FONT_PIVOT pivot);					//�߽��� ��ġ
	EATER_ENGINEDLL void SetPosition(float X,float Y);						//��ġ
	EATER_ENGINEDLL void SetOffset(float FontOffset);						//���� ������ ����
	EATER_ENGINEDLL void SetScale(float X,float Y);							//���� ������ ����
	EATER_ENGINEDLL void SetPivot(PIVOT_TYPE type);							//���� ������ ����
	EATER_ENGINEDLL void SetPositionObject(GameObject* Object,Vector3 Pos);	//���� ������ ����
	EATER_ENGINEDLL void SetLayer(int Layer);								//���� ������ ����
	EATER_ENGINEDLL void SetActive(bool IsActive);							//���� ������ ����

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

