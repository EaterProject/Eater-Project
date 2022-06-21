#include "ColorSetting.h"
#include "EngineData.h"

// 컴포넌트
#include "MeshFilter.h"
#include "GameObject.h"

ColorSetting::ColorSetting()
{




}

ColorSetting::~ColorSetting()
{




}

void ColorSetting::Setting(GameObject* Object)
{
	int Count =  Object->GetChildMeshCount();
	for (int i = 0; i < Count; i++)
	{
		//게임 오브젝트와 매쉬필터 가져오기
		GameObject* Obj			= Object->GetChildMesh(i);
		MeshFilter*	mMeshFilter = Obj->GetComponent<MeshFilter>();

		//리스트에 추가
		MeshFilterList.push_back(mMeshFilter);
		GameObjectList.push_back(Obj);

		//메터리얼 블록 추가
		mMeshFilter->SetMaterialPropertyBlock(true);
		MaterialPropertyBlock* Block = mMeshFilter->GetMaterialPropertyBlock();
		MPBList.push_back(Block);
	}
	IsSetting = true;
}

void ColorSetting::SetLimlightSetting(float R, float G, float B, float mFactor, float mWidth)
{
	LimColor[0] = R;
	LimColor[1] = G;
	LimColor[2] = B;
	LimColor[3] = mFactor;
	LimColor[4] = mWidth;
	LimLightUpdate();
}

void ColorSetting::SetEmissiveSetting(float R, float G, float B, float mFactor, float mWidth)
{
	EmissiveColor[0] = R;
	EmissiveColor[1] = G;
	EmissiveColor[2] = B;
	EmissiveColor[3] = mFactor;
	EmissiveColor[4] = mWidth;
	LimLightUpdate();
}

void ColorSetting::SetLimlightColor(float R, float G, float B)
{
	LimColor[0] = R;
	LimColor[1] = G;
	LimColor[2] = B;
	LimLightUpdate();
}

void ColorSetting::SetLimlightColorMax(float R, float G, float B)
{
	LimColorNow[0] = R;
	LimColorNow[1] = G;
	LimColorNow[2] = B;
}

void ColorSetting::SetLimlightFactor(float mFactor)
{
	LimColor[3] = mFactor;
}

void ColorSetting::SetLimlightFactorMax(float mFactor)
{
	LimColorNow[3] = mFactor;
}

void ColorSetting::SetLimlightWidth(float mWidth)
{
	LimColor[4] = mWidth;
}

void ColorSetting::SetLimlightWidthMax(float mWidth)
{
	LimColorNow[4] = mWidth;
}

void ColorSetting::LimLightUpdate()
{
	//셋팅이 완료 되었을 경우에만 업데이트
	int Count = (int)MPBList.size();
	for (int i = 0; i < Count; i++)
	{
		MPBList[i]->LimLightColor	= { LimColor[0],LimColor[1],LimColor[2] };
		MPBList[i]->LimLightFactor	= LimColor[3];
		MPBList[i]->LimLightWidth	= LimColor[4];
	}
}

void ColorSetting::Default()
{
	//기본 셋팅
	LimColor[0] = 1.0f;
	LimColor[1] = 1.0f;
	LimColor[2] = 1.0f;
	LimColor[3] = 1.0f;
	LimColor[4] = 1.0f;

	LimColorNow[0] = 1.0f;
	LimColorNow[1] = 1.0f;
	LimColorNow[2] = 1.0f;
	LimColorNow[3] = 1.0f;
	LimColorNow[4] = 1.0f;

	int Count = (int)MPBList.size();
	for (int i = 0; i < Count; i++)
	{
		MPBList[i]->LimLightColor	= { LimColor[0],LimColor[1],LimColor[2] } ;
		MPBList[i]->LimLightFactor	= LimColor[3];
		MPBList[i]->LimLightWidth	= LimColor[4];
	}
}




