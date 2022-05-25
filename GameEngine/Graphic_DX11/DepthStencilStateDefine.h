#pragma once
#include "HashBase.h"

#define DEPTH_STENCIL_STATE(ClassName) CREATE_HASH_CLASS(ClassName, RESOURCE_TYPE::DSS)

///////////////////////////////////////////////////////////////////////////////////////////
// Global Depth Stencil State
///////////////////////////////////////////////////////////////////////////////////////////

DEPTH_STENCIL_STATE(DSS_Defalt)
DEPTH_STENCIL_STATE(DSS_OutLine)
DEPTH_STENCIL_STATE(DSS_NoStencil)
DEPTH_STENCIL_STATE(DSS_NoDepth)
DEPTH_STENCIL_STATE(DSS_Mask)
DEPTH_STENCIL_STATE(DSS_CubeMap)
