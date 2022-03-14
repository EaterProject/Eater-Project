#pragma once
#include "HashBase.h"

#define VIEW_PORT(ClassName) CREATE_HASH_CLASS(ClassName, RESOURCE_TYPE::VP)

///////////////////////////////////////////////////////////////////////////////////////////
// Global ViewPort
///////////////////////////////////////////////////////////////////////////////////////////

VIEW_PORT(VP_FullScreen)
VIEW_PORT(VP_Shadow)
VIEW_PORT(VP_HalfScreen)

VIEW_PORT(VP_DownSampling)

VIEW_PORT(VP_MRT1)
VIEW_PORT(VP_MRT2)
VIEW_PORT(VP_MRT3)
VIEW_PORT(VP_MRT4)
VIEW_PORT(VP_MRT5)
VIEW_PORT(VP_MRT6)
VIEW_PORT(VP_MRT7)
VIEW_PORT(VP_MRT8)