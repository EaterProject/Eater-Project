#include "FontImage.h"
#include "EaterEngineAPI.h"
#include "GameObject.h"
#include "Image.h"
#include "RectTransform.h"

FontImage::FontImage()
{

}

FontImage::~FontImage()
{

}

void FontImage::SetFontNumber(int Number)
{
	FontUpdte		= true;
	FontAlpha = 255;
	Com_Rect[0]->SetScale(FontOriginalSize, FontOriginalSize);
	Com_Rect[1]->SetScale(FontOriginalSize, FontOriginalSize);
	Com_Rect[2]->SetScale(FontOriginalSize, FontOriginalSize);
	std::string Str_Number = std::to_string(Number);
	int ComboNumber = 0;
	int Size = Str_Number.size();

	for (int i = 0; i < Size; i++)
	{
		if (Size == 1)
		{
			ComboNumber = Str_Number[0] - 48;
			Com_Image[0]->SetTexture(FontName + std::to_string(0));
			Com_Image[1]->SetTexture(FontName + std::to_string(ComboNumber));
			Com_Image[2]->SetDraw(false);

			Com_Rect[0]->SetPosition(PosX + (OffsetX * 0), PosY);
			Com_Rect[1]->SetPosition(PosX + (OffsetX * 1), PosY);
			Com_Rect[2]->SetPosition(PosX + (OffsetX * 2), PosY);
			break;
		}
		else if (Size == 2)
		{
			ComboNumber = Str_Number[0] - 48;
			Com_Image[0]->SetTexture(FontName + std::to_string(ComboNumber));
			ComboNumber = Str_Number[1] - 48;
			Com_Image[1]->SetTexture(FontName + std::to_string(ComboNumber));
			Com_Image[2]->SetDraw(false);

			Com_Rect[0]->SetPosition(PosX + (OffsetX * 0), PosY);
			Com_Rect[1]->SetPosition(PosX + (OffsetX * 1), PosY);
			Com_Rect[2]->SetPosition(PosX + (OffsetX * 2), PosY);
			break;
		}
		else
		{
			ComboNumber = Str_Number[i] - 48;
			Com_Image[i]->SetTexture(FontName + std::to_string(ComboNumber));
			Com_Rect[i]->SetPosition(PosX + (OffsetX * i), PosY);
		}
	}
}

void FontImage::SetSize(float x, float Y)
{




}

void FontImage::UpdateFontAnimation()
{
	if (FontUpdte == true)
	{
		FontSizeUPTime += GetDeltaTime() * 3.0f;
		if (FontSizeUPTime >= FontSizeUPTimeMax)
		{
			Com_Rect[0]->SetScale(FontSizeUPTimeMax, FontSizeUPTimeMax);
			Com_Rect[1]->SetScale(FontSizeUPTimeMax, FontSizeUPTimeMax);
			Com_Rect[2]->SetScale(FontSizeUPTimeMax, FontSizeUPTimeMax);
			FontSizeUPTime = FontSizeUPTimeMin;
			FontUpdte = false;
		}
		else
		{
			Com_Rect[0]->SetScale(FontSizeUPTime, FontSizeUPTime);
			Com_Rect[1]->SetScale(FontSizeUPTime, FontSizeUPTime);
			Com_Rect[2]->SetScale(FontSizeUPTime, FontSizeUPTime);
		}
	}
}

void FontImage::Setting(int mPosX, int mPosY, std::string mFontName, float SizeX, float SizeY,float NumberOffset, PIVOT_TYPE pivot)
{
	PosX = mPosX;
	PosY = mPosY;
	OffsetX = NumberOffset;
	FontName = mFontName;

	//FontUpdte = true;
	for (int i = 0; i < 3; i++)
	{
		ImageObject[i] = Instance_UI();
		ImageObject[i]->AddComponent<Image>();

		Com_Image[i] = ImageObject[i]->GetComponent<Image>();
		Com_Rect[i] = ImageObject[i]->GetComponent<RectTransform>();

		Com_Rect[i]->SetPivot(pivot);

		FontOriginalSize = SizeX;
		Com_Rect[i]->SetScale(SizeX, SizeY);
		Com_Rect[i]->SetPosition(PosX + (OffsetX * i), PosY);
		std::string Name = mFontName + std::to_string(i);
		Com_Image[i]->SetTexture(Name);
		Com_Image[i]->SetColor(255, 255, 255);
	}
}

void FontImage::FontAnimation(float MinSize, float MaxSize)
{
	FontSizeUPTimeMin	= MinSize;
	FontSizeUPTimeMax	= MaxSize;
	FontSizeUPTime = FontSizeUPTimeMin;
}
