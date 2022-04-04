#pragma once
// Parsing Option
typedef enum PARSING_OPTION : UINT
{
	SCALING			= 0x00000001,
	ANIMATION_ONLY	= 0x00000010,
	ROTATE_AXIS		= 0x00000100
}PARSING_OPTION;

// Lighting Option
typedef enum RENDER_OPTION : UINT
{
	RENDER_DEBUG			= 0x00000001,
	RENDER_SHADOW			= 0x00000010,
	RENDER_SSAO				= 0x00000100,
	RENDER_IBL				= 0x00001000,
}RENDER_OPTION;

// PostProcess Option
typedef enum POSTPROCESS_OPTION : UINT
{
	RENDER_FOG		= 0x00000001,
	RENDER_BLOOM	= 0x00000010,
	RENDER_HDR		= 0x00000100,
	RENDER_FXAA		= 0x00001000,
}POSTPROCESS_OPTION;

// Debug Option
typedef enum DEBUG_OPTION : UINT
{
	DEBUG_ENGINE,
	DEBUG_EDITOR,
}DEBUG_OPTION;

// Particle Animation Option
typedef enum PARTICLE_ANIMATION_OPTION : UINT
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

// Object Type
typedef enum OBJECT_TYPE
{
	DEFALT,			//���� �����ʾ����� �⺻��
	GAMEOBJECT,		//�ٸ� ������Ʈ���� ������� �뵵
	BASE,			//��ӱ����� �Ǿ��ִ� ������Ʈ
	SKINNING,		//��Ű���� �߰��� ������Ʈ
	BONE,			//���� �ִ� ������Ʈ
	TERRAIN,		//�ͷ��� ������Ʈ
	CAMERA,			//ī�޶� ��ɸ� �ϴ� ������Ʈ
	LIGHT,			//����Ʈ ��ü
	SKYBOX,			//��ī�� �ڽ�
	TEXTURE,		//�Ѱ��� �ؽ��ķε� ������Ʈ(���̽��ΰ��� ������ �簢�� �ؽ���)
	PARTICLE,		//��ƼŬ
	PARTICLE_SYSTEM,//��ƼŬ �ý���
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
	DEBUG_MESH_BOX,
	DEBUG_MESH_SPHERE,
	DEBUG_MESH_RAY
}DEBUG_MESH_TYPE;