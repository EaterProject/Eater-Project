#include "UIBoss.h"


#include "EaterEngineAPI.h"
#include "GameObject.h"

#include "Slider.h"
#include "Image.h"
#include "RectTransform.h"
#include "ImageFont.h"
UIBoss::UIBoss()

{
}

UIBoss::~UIBoss()
{
}

void UIBoss::Awake()
{
	GameObject* Object = Instance_Image();
	mBossName_Img = Object->GetComponent<Image>();
	mBossName_Rect = Object->GetComponent<RectTransform>();
	mBossName_Img->SetTexture("BossTitle");
	mBossName_Img->SetPivot(PIVOT_MIDDLE_TOP);
	mBossName_Img->SetPosition(20, 100);

	Object			= Instance_Slider();
	mBossHP_Slider	= Object->GetComponent<Slider>();
	mBossHP_Rect	= Object->GetComponent<RectTransform>();
	mBossHP_Slider->SetBackGroundTexture("BossHP_Back");
	mBossHP_Slider->SetFillTexture("BossHP_Front");
	mBossHP_Slider->SetPosition(0, 150);
	mBossHP_Slider->SetPivot(PIVOT_MIDDLE_TOP);
	mBossHP_Slider->SetValueRange(0, 100);
	mBossHP_Slider->SetFillRange(FILL_LEFT, 125);
	
	Object = Instance_Image();
	mBossColor_Img = Object->GetComponent<Image>();
	mBossColor_Rect = Object->GetComponent<RectTransform>();
	mBossColor_Img->SetTexture("Boss_White_Color");
	mBossColor_Img->SetPivot(PIVOT_MIDDLE_TOP);
	mBossColor_Img->SetPosition(-400, 100);
	mBossColor_Img->SetLayer(1);

	Object = Instance_ImageFont();
	mBossCount_Font = Object->GetComponent<ImageFont>();
	mBossCount_Font->SetTexture("number_");
	mBossCount_Font->SetPivot(PIVOT_MIDDLE_TOP);
	mBossCount_Font->SetFontCount(2);
	mBossCount_Font->SetOffset(15);
	mBossCount_Font->SetScale(1, 1);
	mBossCount_Font->SetPosition(-425, 150);
	mBossCount_Font->SetColor(65, 197, 198);
	mBossCount_Font->SetFontCount(2);
	mBossCount_Font->SetScale(0.5f, 0.5f);
}

void UIBoss::Start()
{
	mBossCount_Font->SetFontNumber(30);
}

void UIBoss::SetUp()
{

}
