#include "MeshFilterSetting.h"
#include "EaterEngineAPI.h"
#include "EngineData.h"

// 컴포넌트
#include "MeshFilter.h"
#include "GameObject.h"

MeshFilterSetting::MeshFilterSetting()
{

}

MeshFilterSetting::~MeshFilterSetting()
{
	Release();
}

void MeshFilterSetting::Setting(GameObject* Object)
{
	int Count =  Object->GetChildMeshCount();
	if(Count == 0)
	{
		MeshFilter* mMeshFilter = Object->GetComponent<MeshFilter>();
		MeshFilterList.push_back(mMeshFilter);
		GameObjectList.push_back(Object);

		mMeshFilter->SetMaterialPropertyBlock(true);
		MaterialPropertyBlock* Block = mMeshFilter->GetMaterialPropertyBlock();
		MPBList.push_back(Block);
	}
	else
	{
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
	}
}

void MeshFilterSetting::SetLimlightSetting(float R, float G, float B, float mFactor, float mWidth)
{
	LimColor[0] = R;
	LimColor[1] = G;
	LimColor[2] = B;
	LimColor[3] = mFactor;
	LimColor[4] = mWidth;
	LimLightUpdate();
	IsSetting_LimLight = true;
}

void MeshFilterSetting::SetLimlightSetting(COLOR_TYPE Type, float mFactor, float mWidth)
{
	switch (Type)
	{
	case COLOR_TYPE::RED:
		LimColor[0] = 1;
		LimColor[1] = 0;
		LimColor[2] = 0;
		break;
	case COLOR_TYPE::BLUE:
		LimColor[0] = 0;
		LimColor[1] = 0;
		LimColor[2] = 1;
		break;
	case COLOR_TYPE::GREEN:
		LimColor[0] = 0;
		LimColor[1] = 1;
		LimColor[2] = 0;
		break;
	case COLOR_TYPE::WHITE:
		LimColor[0] = 1;
		LimColor[1] = 1;
		LimColor[2] = 1;
		break;
	}
	LimColor[3] = mFactor;
	LimColor[4] = mWidth;
	LimLightUpdate();
	IsSetting_LimLight = true;
}

void MeshFilterSetting::SetLimlightSettingMax(float R, float G, float B, float mFactor, float mWidth)
{
	LimColorMax[0] = R;
	LimColorMax[1] = G;
	LimColorMax[2] = B;
	LimColorMax[3] = mFactor;
	LimColorMax[4] = mWidth;
	IsSetting_LimLight = true;
}

void MeshFilterSetting::SetLimlightSettingMax(COLOR_TYPE Type, float mFactor, float mWidth)
{
	switch (Type)
	{
	case COLOR_TYPE::RED:
		LimColorMax[0] = 1;
		LimColorMax[1] = 0;
		LimColorMax[2] = 0;
		break;
	case COLOR_TYPE::BLUE:
		LimColorMax[0] = 0;
		LimColorMax[1] = 0;
		LimColorMax[2] = 1;
		break;
	case COLOR_TYPE::GREEN:
		LimColorMax[0] = 0;
		LimColorMax[1] = 1;
		LimColorMax[2] = 0;
		break;
	case COLOR_TYPE::WHITE:
		LimColorMax[0] = 1;
		LimColorMax[1] = 1;
		LimColorMax[2] = 1;
		break;
	}
	LimColorMax[3] = mFactor;
	LimColorMax[4] = mWidth;
	IsSetting_LimLight = true;
}

void MeshFilterSetting::SetEmissiveSetting(float R, float G, float B, float mFactor)
{
	EmissiveColor[0] = R;
	EmissiveColor[1] = G;
	EmissiveColor[2] = B;
	EmissiveColor[3] = mFactor;
	EmissiveUpdate();
	IsSetting_Emissive = true;
}

void MeshFilterSetting::SetEmissiveSetting(COLOR_TYPE Type, float mFactor)
{
	switch (Type)
	{
	case COLOR_TYPE::RED:
		EmissiveColor[0] = 1.0f;
		EmissiveColor[1] = 0.0f;
		EmissiveColor[2] = 0.0f;
		break;
	case COLOR_TYPE::GREEN:
		EmissiveColor[0] = 0.0f;
		EmissiveColor[1] = 1.0f;
		EmissiveColor[2] = 0.0f;
		break;
	case COLOR_TYPE::BLUE:
		EmissiveColor[0] = 0.0f;
		EmissiveColor[1] = 0.0f;
		EmissiveColor[2] = 1.0f;
		break;
	}
	EmissiveColor[3] = mFactor;
	EmissiveUpdate();
	IsSetting_Emissive = true;
}

void MeshFilterSetting::SetEmissiveSettingMax(float R, float G, float B, float mFactor)
{
	EmissiveColor[0] = R;
	EmissiveColor[1] = G;
	EmissiveColor[2] = B;
	EmissiveColor[3] = mFactor;
	IsSetting_Emissive = true;
}

void MeshFilterSetting::SetEmissiveSettingMax(COLOR_TYPE Type, float mFactor)
{
	switch (Type)
	{
		case COLOR_TYPE::RED:
			EmissiveColor[0] = 1.0f;
			EmissiveColor[1] = 0.0f;
			EmissiveColor[2] = 0.0f;
			break;
		case COLOR_TYPE::GREEN:
			EmissiveColor[0] = 0.0f;
			EmissiveColor[1] = 1.0f;
			EmissiveColor[2] = 0.0f;
			break;
		case COLOR_TYPE::BLUE:
			EmissiveColor[0] = 0.0f;
			EmissiveColor[1] = 0.0f;
			EmissiveColor[2] = 1.0f;
			break;
		case COLOR_TYPE::WHITE:
			EmissiveColor[0] = 1.0f;
			EmissiveColor[1] = 1.0f;
			EmissiveColor[2] = 1.0f;
			break;
	}
	EmissiveColor[2] = mFactor;
	IsSetting_Emissive = true;
}

void MeshFilterSetting::LimLightUpdate()
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

void MeshFilterSetting::EmissiveUpdate()
{
	int Count = (int)MPBList.size();
	for (int i = 0; i < Count; i++)
	{
		MPBList[i]->EmissiveColor	= { EmissiveColor[0],EmissiveColor[1],EmissiveColor[2] };
		MPBList[i]->EmissiveFactor	= EmissiveColor[3];
	}
}

void MeshFilterSetting::LimLightUpdate(float Speed)
{
	//처음 한번만 값 증가,감소 여부 판단
	if (IsSetting_LimLight == true)
	{
		for (int i = 0; i < 5; i++)
		{
			if (LimColor[i] > LimColorMax[i])
			{
				LimColorValue[i] = TYPE_DOWN;
				LImColorOneFrame[i] = LimColor[i] - LimColorMax[i];
			}
			else if (LimColor[i] < LimColorMax[i])
			{
				LimColorValue[i] = TYPE_UP;
				LImColorOneFrame[i] = LimColorMax[i] - LimColor[i];
			}
			else
			{
				LimColorValue[i] = NONE;
				LImColorOneFrame[i] = 0;
			}
		}
		IsSetting_LimLight = false;
	}
	else
	{
		float DTime = GetDeltaTime();
		for (int i = 0; i < 5; i++)
		{
			switch (LimColorValue[i])
			{
			case 0: //증가a
				LimColor[i] += DTime * (LImColorOneFrame[i] * Speed);
				if (LimColor[i] >= LimColorMax[i]) { LimColorValue[i] = TYPE_NONE; LimColor[i] = LimColorMax[i]; }
				//DebugPrint("%.2f", LimColor[i]);
				break;
			case 1: //감소
				LimColor[i] -= DTime * (LImColorOneFrame[i] * Speed);
				if (LimColor[i] <= LimColorMax[i]) { LimColorValue[i] = TYPE_NONE; LimColor[i] = LimColorMax[i]; }
				break;
			case 2: //변화량 없음
				break;
			}
		}
		LimLightUpdate();
	}
}

void MeshFilterSetting::EmissiveUpdate(float Speed)
{
	//처음 한번만 값 증가,감소 여부 판단
	if (IsSetting_Emissive == true)
	{
		for (int i = 0; i < 5; i++)
		{
			if (EmissiveColor[i] > EmissiveColorMax[i])
			{
				EmissiveValue[i] = TYPE_DOWN;
				EmissiveOneFrame[i] = EmissiveColor[i] - EmissiveColorMax[i];
			}
			else if (EmissiveColor[i] < EmissiveColorMax[i])
			{
				EmissiveValue[i] = TYPE_UP;
				EmissiveOneFrame[i] = EmissiveColorMax[i] - EmissiveColor[i];
			}
			else
			{
				EmissiveValue[i] = NONE;
				EmissiveOneFrame[i] = 0;
			}
		}
		IsSetting_Emissive = false;
	}
	else
	{
		float DTime = GetDeltaTime();
		for (int i = 0; i < 5; i++)
		{
			switch (EmissiveValue[i])
			{
			case 0: //증가a
				EmissiveColor[i] += DTime * (EmissiveOneFrame[i] * Speed);
				if (EmissiveColor[i] >= EmissiveColorMax[i]) { EmissiveValue[i] = TYPE_NONE; EmissiveColor[i] = EmissiveColorMax[i]; }
				//DebugPrint("%.2f", LimColor[i]);
				break;
			case 1: //감소
				EmissiveColor[i] -= DTime * (EmissiveOneFrame[i] * Speed);
				if (EmissiveColor[i] <= EmissiveColorMax[i]) { EmissiveValue[i] = TYPE_NONE; EmissiveColor[i] = EmissiveColorMax[i]; }
				break;
			case 2: //변화량 없음
				break;
			}
		}
		EmissiveUpdate();
	}
}

void MeshFilterSetting::Default()
{
	//기본 셋팅
	LimColor[0] = 1.0f;
	LimColor[1] = 1.0f;
	LimColor[2] = 1.0f;
	LimColor[3] = 1.0f;
	LimColor[4] = 1.0f;

	LimColorMax[0] = 1.0f;
	LimColorMax[1] = 1.0f;
	LimColorMax[2] = 1.0f;
	LimColorMax[3] = 1.0f;
	LimColorMax[4] = 1.0f;

	int Count = (int)MPBList.size();
	for (int i = 0; i < Count; i++)
	{
		MPBList[i]->LimLightColor	= { LimColor[0],LimColor[1],LimColor[2] } ;
		MPBList[i]->LimLightFactor	= LimColor[3];
		MPBList[i]->LimLightWidth	= LimColor[4];
	}
}

void MeshFilterSetting::Release()
{
	int Size = (int)MeshFilterList.size();
	for (int i = 0; i < Size; i++)
	{
		MeshFilterList[i]->SetMaterialPropertyBlock(false);
	}
}





