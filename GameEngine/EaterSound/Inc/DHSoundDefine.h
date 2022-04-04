#pragma once

#define CHANNEL_COUNT 256
#define DEFAULT_FREQUENCY 44100

#ifdef EATERSOUND_EXPORTS
#define EATERSOUND_DLL __declspec(dllexport)
#else
#define EATERSOUND_DLL __declspec(dllimport)
#endif

// String ���.
#include <string>

// ������ ���� ����.
enum class Sound_Category
{
	BGM,	// BGM ���� ����
	SFX,	// SFX ���� ����
	Master	// ��ü ����
};

// ������ ���� / �������� / ��ġ
struct Sound_VFP
{
	// ���� ����
	float Volume	=	1.f;
	// ��� �� (�ӵ��� ������ �ش�. 2��� �ӵ� 2��� ����ȿ���̱���.)
	float FreQuency =	44100.f;
	// ��� �ӵ�
	float Pitch		=	1.f;
};

// 3D ���带 ���� Vector3( �ش��ϴ� ������Ʈ�� x,y,z ���� �����͸� ���� )
namespace SoundMath
{
	struct Vector3
	{
		Vector3() {};
		Vector3(float *_X, float *_Y, float *_Z) 
		{
			_x = _X;
			_y = _Y;
			_z = _Z;
		};
		float* _x = nullptr;
		float* _y = nullptr;
		float* _z = nullptr;
	};
}
