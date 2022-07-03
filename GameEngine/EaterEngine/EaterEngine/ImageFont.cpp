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
	for (int i = 0; i < FontCount; i++)
	{
		GameObject* Object = Instance_Image();
		Image* mImage = Object->GetComponent<Image>();
		RectTransform* mRect = Object->GetComponent<RectTransform>();

		ImageList.push_back(mImage);
		TransformList.push_back(mRect);
	}
}

void ImageFont::SetUp()
{
	Center_Index = ceil(ImageList.size() * 0.5f);
	IsCenter = ImageList.size() % 2;

	for (int i = 0; i < (int)ImageList.size(); i++)
	{
		std::string Name;
		for (int j = 0; j < 10; j++)
		{
			Name = FontImageName + std::to_string(j);
			ImageList[i]->PushTextureList(Name);
		}
		ImageList[i]->SetTexture(FontImageName + std::to_string(0));
		ImageList[i]->SetColor(mMF_Setting);
		TransformList[i]->SetPivot(mType);
		TransformList[i]->SetPosition((mPosition.x) + (i * OffsetX), mPosition.y);
		TransformList[i]->SetScale(mScale);

		if (IsFillZero == false)
		{
			SetRelocation(TransformList[i], i + 1);
		}
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
	mMF_Setting = { r,g,b,255 };
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
				// 0으로 채우는 경우
				if (IsFillZero)
				{
					std::string Name = FontImageName + std::to_string(0);
					ImageList[i]->SetDraw(true);
					ImageList[i]->SetTexture(Name);
				}
				else
				{
					ImageList[i]->SetDraw(false);
				}
			}
			else
			{
				int ComboNumber = Str_Number[Index] - 48;
				std::string Name = FontImageName + std::to_string(ComboNumber);
				ImageList[i]->SetDraw(true);
				ImageList[i]->SetTexture(Name);
				TransformList[i]->SetPivot(mType);
				Index++;
			}

			if (PivotType == FONT_CENTER)
			{
				Center_Index = ceil(Size * 0.5f);
				IsCenter = Size % 2;

				SetRelocation(TransformList[i], Index);
			}
		}
	}
	else
	{
		for (int i = 0; i < Size; i++)
		{
			int ComboNumber = Str_Number[i] - 48;
			std::string Name = FontImageName + std::to_string(ComboNumber);
			ImageList[i]->SetDraw(true);
			ImageList[i]->SetTexture(Name);
			Index++;

			if (PivotType == FONT_CENTER)
			{
				Center_Index = ceil(Size * 0.5f);
				IsCenter = Size % 2;

				SetRelocation(TransformList[i], i + 1);
			}
		}
	}
}

void ImageFont::SetRelocation(RectTransform* tr, int now_index)
{
	switch (PivotType)
	{
	case FONT_CENTER:
	{
		if (IsCenter)
		{
			if (Center_Index < now_index)
			{
				tr->SetPosition(mPosition.x + (OffsetX * (now_index - Center_Index)), mPosition.y);
			}
			else if (Center_Index > now_index)
			{
				tr->SetPosition(mPosition.x - (OffsetX * (Center_Index - now_index)), mPosition.y);
			}
			else
			{
				tr->SetPosition(mPosition);
			}
		}
		else
		{
			if (Center_Index < now_index)
			{
				tr->SetPosition(mPosition.x + (OffsetX * (now_index - Center_Index)) - Half_OffsetX, mPosition.y);
			}
			else
			{
				tr->SetPosition(mPosition.x - (OffsetX * (Center_Index - (now_index - 1))) + Half_OffsetX, mPosition.y);
			}
		}
	}
	break;
	default:
		break;
	}
}

void ImageFont::SetFontCount(int Number, bool FillZero)
{
	FontCount = Number;
	IsFillZero = FillZero;
}

void ImageFont::SetFontPivot(FONT_PIVOT pivot)
{
	PivotType = pivot;
}

void ImageFont::SetPosition(float X, float Y)
{
	mPosition.x = X;
	mPosition.y = Y;
}

void ImageFont::SetOffset(float FontOffset)
{
	OffsetX = FontOffset;
	Half_OffsetX = FontOffset * 0.5f;
}

void ImageFont::SetScale(float X, float Y)
{
	mScale = { X,Y };
}

void ImageFont::SetPivot(PIVOT_TYPE type)
{
	mType = type;
}

void ImageFont::SetPositionObject(GameObject* Object, Vector3 Pos)
{
	for (int i = 0; i < (int)ImageList.size(); i++)
	{
		TransformList[i]->SetPositionObject(Object, Pos);
	}
}

void ImageFont::SetLayer(int Layer)
{
	for (int i = 0; i < (int)ImageList.size(); i++)
	{
		ImageList[i]->SetLayer(Layer);
	}
}

void ImageFont::SetDraw(bool IsActive)
{
	for (int i = 0; i < (int)ImageList.size(); i++)
	{
		ImageList[i]->SetDraw(IsActive);
	}
}

void ImageFont::SetActive(bool IsActive)
{
	for (int i = 0; i < (int)ImageList.size(); i++)
	{
		ImageList[i]->SetActive(IsActive);
	}
}

