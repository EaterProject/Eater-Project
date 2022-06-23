#include "ImageFont.h"
#include "EaterEngineAPI.h"

#include "GameObject.h"
#include "Image.h"
#include "RectTransform.h"
ImageFont::ImageFont()
{


}

ImageFont::~ImageFont()
{

}

void ImageFont::Awake()
{
	IsAwake = true;
	for (int i = 0; i < FontCount; i++)
	{
		GameObject* Object = Instance_Image();
		Image*			mImage	= Object->GetComponent<Image>();
		RectTransform*	mRect	= Object->GetComponent<RectTransform>();

		ImageList.push_back(mImage);
		TransformList.push_back(mRect);
	}


	for (int i = 0; i < (int)ImageList.size(); i++)
	{
		std::string Name;
		for (int j = 0; j < 10; j++)
		{
			Name = FontImageName + std::to_string(j);
			ImageList[i]->PushTextureList(Name);
		}

		ImageList[i]->SetTexture(Name = FontImageName + std::to_string(0));
		TransformList[i]->SetPosition((mPosition.x)+ (i * OffsetX), mPosition.y);
		TransformList[i]->SetScale(mScale);
	}
}

void ImageFont::SetTexture(std::string texture_name)
{
	FontImageName = texture_name;
}

void ImageFont::SetColor(float r, float g, float b)
{
	mMF_Setting = { r,g,b,255 };
}

void ImageFont::SetColor(float r, float g, float b, float a)
{
	mMF_Setting = { r,g,b,a };
}

void ImageFont::SetFontNumber(int Number)
{
	FontNumber = Number;
	
	//들어온 숫자를 string 형으로 변환
	std::string Str_Number = std::to_string(Number);
	int Size = Str_Number.size();
	int Index = 0;
	//들어온 숫자보다 자리수가 더 많다면 나머지는 0으로 채운다
	if (FontCount > Size)
	{
		int Count = FontCount - Size;
		for (int i = 0; i < FontCount; i++)
		{
			if (i < Count)
			{
				std::string Name = FontImageName + std::to_string(0);
				ImageList[i]->SetTexture(Name);
			}
			else
			{
				int ComboNumber = Str_Number[Index] - 48;
				std::string Name = FontImageName + std::to_string(ComboNumber);
				ImageList[i]->SetTexture(Name);
				Index++;
			}
		}
	}
	else
	{
		for (int i = 0; i < Size; i++)
		{
			int ComboNumber = Str_Number[i] - 48;
			std::string Name = FontImageName + std::to_string(ComboNumber);
			ImageList[i]->SetTexture(Name);
			Index++;
		}
	}
}

void ImageFont::SetFontCount(int Number)
{
	FontCount = Number;
}

void ImageFont::SetPosition(float X, float Y)
{
	mPosition.x = X;
	mPosition.y = Y;
}

void ImageFont::SetOffset(float FontOffset)
{
	OffsetX = FontOffset;
}

void ImageFont::SetScale(float X, float Y)
{
	mScale = { X,Y };
}

void ImageFont::SetPivot(PIVOT_TYPE mType)
{
	for(int i = 0; i < (int)ImageList.size();i++)
	{
		TransformList[i]->SetPivot(mType);
	}
}

void ImageFont::SetPositionObject(GameObject* Object, Vector3 Pos)
{
	for (int i = 0; i < (int)ImageList.size(); i++)
	{
		TransformList[i]->SetPositionObject(Object,Pos);
	}
}

void ImageFont::SetLayer(int Layer)
{
	for (int i = 0; i < (int)ImageList.size(); i++)
	{
		ImageList[i]->SetLayer(Layer);
	}
}

void ImageFont::SetActive(bool IsActive)
{
	for (int i = 0; i < (int)ImageList.size(); i++)
	{
		ImageList[i]->gameobject->SetActive(IsActive);
	}
}

