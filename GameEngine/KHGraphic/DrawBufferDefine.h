#pragma once
#include "HashBase.h"

#define DRAW_BUFFER(ClassName) CREATE_HASH_CLASS(ClassName, RESOURCE_TYPE::DB)

///////////////////////////////////////////////////////////////////////////////////////////
// Global Draw Buffer
///////////////////////////////////////////////////////////////////////////////////////////

DRAW_BUFFER(DB_Quad)
DRAW_BUFFER(DB_SSAO)
DRAW_BUFFER(DB_Box)
DRAW_BUFFER(DB_Sphere)

DRAW_BUFFER(DB_Line_Ray)
DRAW_BUFFER(DB_Line_Quad)
DRAW_BUFFER(DB_Line_Axis)
DRAW_BUFFER(DB_Line_Circle)
DRAW_BUFFER(DB_Line_Box)
