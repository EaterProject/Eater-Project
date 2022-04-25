#pragma once
#include "HashBase.h"

#define RANDER_TARGET(ClassName) CREATE_HASH_CLASS(ClassName, RESOURCE_TYPE::RT)

///////////////////////////////////////////////////////////////////////////////////////////
// Global RenderTarget
///////////////////////////////////////////////////////////////////////////////////////////

RANDER_TARGET(RT_BackBuffer)

RANDER_TARGET(RT_Deffered_Albedo)
RANDER_TARGET(RT_Deffered_Emissive)
RANDER_TARGET(RT_Deffered_Normal)
RANDER_TARGET(RT_Deffered_Position)
RANDER_TARGET(RT_Deffered_NormalDepth)

RANDER_TARGET(RT_OutPut1)
RANDER_TARGET(RT_OutPut2)
RANDER_TARGET(RT_OutPut3)

RANDER_TARGET(RT_SSAO_Main)
RANDER_TARGET(RT_SSAO_Blur)

RANDER_TARGET(RT_Bloom_Brightx4_1)
RANDER_TARGET(RT_Bloom_Brightx4_2)

RANDER_TARGET(RT_Bloom_Downx24_1)
RANDER_TARGET(RT_Bloom_Downx24_2)
RANDER_TARGET(RT_Bloom_Downx144_1)
RANDER_TARGET(RT_Bloom_Downx144_2)

RANDER_TARGET(RT_ID)
RANDER_TARGET(RT_ID_Copy)

RANDER_TARGET(RT_Origin)
RANDER_TARGET(RT_OutLine)

