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

void FontImage::Awake()
{
	for (int i = 0; i < 3; i++)
	{
		ImageObject[i] = Instance_UI();
		ImageObject[i]->AddComponent<Image>();

		Com_Image[i] = ImageObject[i]->GetComponent<Image>();
		Com_Rect[i]	 = ImageObject[i]->GetComponent<RectTransform>();

		Com_Rect[i]->SetImagePivot(RECT_PIVOT::PIVOT_MIDDLE_CENTER);
		Com_Rect[i]->SetScale(0.5f, 0.5f);

		if (i == 2)
		{
			Com_Rect[i]->SetPosition(60 * 4, 0.0f);
			std::string Name = "combo_title";
			Com_Image[i]->SetTexture(Name);
		}
		else
		{
			Com_Rect[i]->SetPosition(60 * i, 0.0f);
			std::string Name = "combo_" + std::to_string(i);
			Com_Image[i]->SetTexture(Name);
		}
	}
}

void FontImage::Update()
{



}

void FontImage::SetCombo(int Number)
{
	std::string Str_Number = std::to_string(Number);
	int ComboNumber = 0;
	if (Str_Number.size() == 1)
	{
		Com_Image[0]->SetTexture("combo_0");
		ComboNumber = Str_Number[0] - 48;
		Com_Image[1]->SetTexture("combo_" + std::to_string(ComboNumber));
	}
	else
	{
		ComboNumber = Str_Number[0] - 48;
		Com_Image[0]->SetTexture("combo_" + std::to_string(ComboNumber));
		ComboNumber = Str_Number[1] - 48;
		Com_Image[1]->SetTexture("combo_" + std::to_string(ComboNumber));
	}
}
