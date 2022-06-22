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


	EATER_ENGINEDLL void SetTexture(std::string TextureName);			//�ְ� ���� ���� �̹���
	EATER_ENGINEDLL void SetColor(float R, float G, float B);			//�÷� ��
	EATER_ENGINEDLL void SetColor(float R, float G, float B, float A);	//�÷� ��
	EATER_ENGINEDLL void SetFontNumber(int Number);						//�ְ� ���� ����
	EATER_ENGINEDLL void SetFontCount(int Number);						//ǥ�� �ϰ� ���� �ڸ���
	EATER_ENGINEDLL void SetPosition(float X,float Y);					//��ġ
	EATER_ENGINEDLL void SetOffset(float FontOffset);					//���� ������ ����
	EATER_ENGINEDLL void SetScale(float X,float Y);						//���� ������ ����
	EATER_ENGINEDLL void SetPivot(PIVOT_TYPE mType);					//���� ������ ����
	EATER_ENGINEDLL void SetPositionObject(GameObject* Object,Vector3 Pos);	//���� ������ ����
	EATER_ENGINEDLL void SetLayer(int Layer);							//���� ������ ����
private:
	std::vector<Image*>			ImageList;
	std::vector<RectTransform*> TransformList;

	std::string	FontImageName;

	int FontCount	= 3;
	int FontNumber	= 0;

	float OffsetX	= 0.0f;

	bool IsAwake = false;

	Vector4 mColor		= { 255,255,255,255};
	Vector2 mPosition	= {0,0};
	Vector2 mScale		= {1,1};
};

