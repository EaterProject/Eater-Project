#pragma once
// Parsing Option
typedef enum PARSING_OPTION : UINT
{
	SCALING			= 0x00000001,	// FBX Parser Parsing Scale 100 -> 1
	ANIMATION_ONLY	= 0x00000010,	// FBX Parser Parsing Only Animaition
	ORIGIN_ONLY		= 0x00000100	// FBX Parser Parsing Only Origin Vertex (Collision 전용)
}PARSING_OPTION;

// Render Option
typedef enum RENDER_OPTION : UINT
{
	RENDER_SHADOW	= 0x00000001,
	RENDER_SSAO		= 0x00000010,
	RENDER_IBL		= 0x00000100,
	RENDER_FOG		= 0x00001000,
	RENDER_UI		= 0x00010000,
}RENDER_OPTION;

// PostProcess Option
typedef enum POSTPROCESS_OPTION : UINT
{
	POSTPROCESS_BLOOM			= 0x00000001,
	POSTPROCESS_HDR				= 0x00000010,
	POSTPROCESS_COLORGRADING	= 0x00000100,
	POSTPROCESS_FXAA			= 0x00001000,
}POSTPROCESS_OPTION;	 

// Debug Option
typedef enum DEBUG_OPTION : UINT
{
	DEBUG_RENDERTARGET = 0x00000001,
	DEBUG_MODE		   = 0x00000010,
}DEBUG_OPTION;

// UI Type
typedef enum UI_TYPE
{
	UI_DEFAULT,
	UI_FILL,
	UI_FILL_REVERSE
}UI_TYPE;

// Fill Type
typedef enum FILL_TYPE
{
	FILL_UP,
	FILL_DOWN,
	FILL_LEFT,
	FILL_RIGHT,
}FILL_TYPE;

// Rect Pivot Type
typedef enum PIVOT_TYPE
{
	PIVOT_NONE,
	PIVOT_LEFT_TOP,
	PIVOT_LEFT_BOTTOM,
	PIVOT_RIGHT_TOP,
	PIVOT_RIGHT_BOTTOM,
	PIVOT_MIDDLE_TOP,
	PIVOT_MIDDLE_BOTTOM,
	PIVOT_MIDDLE_LEFT,
	PIVOT_MIDDLE_RIGHT,
	PIVOT_MIDDLE_CENTER,
	PIVOT_OBJECT,
	PIVOT_IMAGE,
}PIVOT_TYPE;

// Pivot Rotate Type
typedef enum ROTATE_ANGLE
{
	ROTATE_0,
	ROTATE_90,
	ROTATE_180,
	ROTATE_270
}ROTATE_ANGLE;

// Shader Type
typedef enum DISSOLVE_OPTION
{
	DISSOLVE_FADEOUT,		// 서서히 사라짐
	DISSOLVE_FADEIN			// 서서히 나타남
}DISSOLVE_OPTION;

// Particle Animation Option
typedef enum PARTICLE_ANIMATION_OPTION
{
	TEXTURE_ANI		= 0x00000001,
	COLOR_ANI		= 0x00000010,
	POSITION_ANI	= 0x00000100,
	ROTATION_ANI	= 0x00001000,
	SCALE_ANI		= 0x00010000
}PARTICLE_ANIMATION_OPTION;

// Particle Render Option
typedef enum PARTICLE_RENDER_OPTION
{
	BILLBOARD,
	VERTICAL_BILLBOARD,
	HORIZONTAL_BILLBOARD,
	MESH
}PARTICLE_RENDER_OPTION;

// Particle LifeTime Option
typedef enum PARTICLE_LIFETIME_OPTION
{
	NONE,
	UP,
	DOWN,
	UPDOWN
}PARTICLE_LIFETIME_OPTION;

// Particle State
typedef enum PARTICLE_STATE
{
	START_STATE,		// 파티클 시작
	PLAY_STATE,			// 파티클 진행중
	PLAY_STAY_STATE,	// 파티클 생성 끝, 생성된 파티클 끝나길 기다리는 시간
	END_STATE			// 최종적으로 파티클이 전부 끝난 경우
}PARTICLE_STATE;

// Object Type
typedef enum OBJECT_TYPE
{
	DEFALT,			//값을 넣지않았을때 기본형
	GAMEOBJECT,		//다른 오브젝트들을 묶어놓는 용도
	BASE,			//상속구조로 되어있는 오브젝트
	SKINNING,		//스키닝이 추가된 오브젝트
	BONE,			//본만 있는 오브젝트
	TERRAIN,		//터레인 오브젝트
	CAMERA,			//카메라 기능만 하는 오브젝트
	LIGHT,			//라이트 객체
	SKYBOX,			//스카이 박스
	TEXTURE,		//한개의 텍스쳐로된 오브젝트(페이스두개가 합쳐진 사각형 텍스쳐)
	PARTICLE,		//파티클
	PARTICLE_SYSTEM,//파티클 시스템
	UI,
}OBJECT_TYPE;

// Light Type
typedef enum LIGHT_TYPE
{
	NONE_LIGHT,
	DIRECTION_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT
}LIGHT_TYPE;

// Debug Mesh Type
typedef enum DEBUG_MESH_TYPE
{
	DEBUG_MESH_RAY,
	DEBUG_MESH_CIRCLE,
	DEBUG_MESH_BOX,
	DEBUG_MESH_SPHERE
}DEBUG_MESH_TYPE;

enum class FUNCTION_STATE
{
	FUNCTION_AWAKE = 0,
	FUNCTION_SETUP,
	FUNCTION_START,
	FUNCTION_SETAT_UPDATE,
	FUNCTION_TRANSFORM_UPDATE,
	FUNCTION_PHYSICS_UPDATE,
	FUNCTION_UPDATE,
	FUNCTION_END_UPDATE,
};