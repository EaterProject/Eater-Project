#pragma once
#include "HashBase.h"

#define RASTERIZER_STATE(ClassName) CREATE_HASH_CLASS(ClassName, RESOURCE_TYPE::RS)

///////////////////////////////////////////////////////////////////////////////////////////
// Global Rasterizer State
///////////////////////////////////////////////////////////////////////////////////////////

RASTERIZER_STATE(RS_Solid)
RASTERIZER_STATE(RS_WireFrame)
RASTERIZER_STATE(RS_NoCull)
RASTERIZER_STATE(RS_Depth)
RASTERIZER_STATE(RS_CubeMap)
