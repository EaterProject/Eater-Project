#pragma once

#include "DHSoundDefine.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#include <map>
#include <vector>

// 3D�� �������� Position / Forward / Up / Velocity �� �����͸� ������ ����ü.
struct Listener_PosFowardUpVelocity
{
	Listener_PosFowardUpVelocity() {};
	Listener_PosFowardUpVelocity(SoundMath::Vector3 _Pos, SoundMath::Vector3 _Foward_Vec, SoundMath::Vector3 _Up_Vec, SoundMath::Vector3 _Velocity) 
	{
		Pos = _Pos;
		Foward_Vec = _Foward_Vec;
		Up_Vec = _Up_Vec;
		Velocity = _Velocity;

		Fmod_Pos.x = *_Pos._x;				Fmod_Pos.y = *_Pos._y;				Fmod_Pos.z = *_Pos._z;
		Fmod_Foward.x = *_Foward_Vec._x;	Fmod_Foward.y = *_Foward_Vec._y;	Fmod_Foward.z = *_Foward_Vec._z;
		Fmod_Up.x = *_Up_Vec._x;			Fmod_Up.y = *_Up_Vec._y;			Fmod_Up.z = *_Up_Vec._z;
		Fmod_Vel.x = *_Velocity._x;			Fmod_Vel.y = *_Velocity._y;			Fmod_Vel.z = *_Velocity._z;
	};

	~Listener_PosFowardUpVelocity() {};

	void Update() 
	{
		Fmod_Pos.x = *Pos._x;				Fmod_Pos.y = *Pos._y;				Fmod_Pos.z = *Pos._z;
		Fmod_Foward.x = *Foward_Vec._x;		Fmod_Foward.y = *Foward_Vec._y;		Fmod_Foward.z = *Foward_Vec._z;
		Fmod_Up.x = *Up_Vec._x;				Fmod_Up.y = *Up_Vec._y;				Fmod_Up.z = *Up_Vec._z;
		Fmod_Vel.x = *Velocity._x;			Fmod_Vel.y = *Velocity._y;			Fmod_Vel.z = *Velocity._z;
	};

	// Update ������ �ǽð� �������� ��� ������ �ʿ���� �������� ���� ������� ������Ʈ�ϱ� ���� �������.
	SoundMath::Vector3 Pos;
	SoundMath::Vector3 Foward_Vec;
	SoundMath::Vector3 Up_Vec;
	SoundMath::Vector3 Velocity;
	// Fmod �� ����Ҷ� ����� ����ü.
	FMOD_VECTOR Fmod_Pos;
	FMOD_VECTOR Fmod_Foward;
	FMOD_VECTOR Fmod_Up;
	FMOD_VECTOR Fmod_Vel;
};

// 3D�� �����ϴ� �Ҹ� ������Ʈ���� Position / Velocity �� �����Ϳ� ä���� ������ ����ü.
struct Object_PosVelocity
{
	Object_PosVelocity() {};
	Object_PosVelocity(FMOD::Channel* _Channel, SoundMath::Vector3 _Pos, SoundMath::Vector3 _Velocity)
	{
		m_Channel = _Channel;
		Pos = _Pos;
		Velocity = _Velocity;

		Fmod_Pos.x = *_Pos._x;			Fmod_Pos.y = *_Pos._y;			Fmod_Pos.z = *_Pos._z;
		Fmod_Vel.x = *_Velocity._x;		Fmod_Vel.y = *_Velocity._y;		Fmod_Vel.z = *_Velocity._z;
	}
	~Object_PosVelocity()
	{
		m_Channel->stop();
	}

	void Update()
	{
		Fmod_Pos.x = *Pos._x;				Fmod_Pos.y = *Pos._y;				Fmod_Pos.z = *Pos._z;
		Fmod_Vel.x = *Velocity._x;			Fmod_Vel.y = *Velocity._y;			Fmod_Vel.z = *Velocity._z;
	};

	void SetAtt()
	{
		m_Channel->set3DAttributes(&Fmod_Pos, &Fmod_Vel);
	}

	FMOD::Channel* m_Channel = nullptr;
	// Update ������ �ǽð� �������� ��� ������ �ʿ���� �������� ���� ������� ������Ʈ�ϱ� ���� �������.
	SoundMath::Vector3 Pos;
	SoundMath::Vector3 Velocity;
	// Fmod �� ����Ҷ� ����� ����ü.
	FMOD_VECTOR Fmod_Pos;
	FMOD_VECTOR Fmod_Vel;
};

class DHSound
{
private:
	// Fmod �ý���
	FMOD::System* Sound_System = nullptr;
	// �� ���忡 ���� �����ͱ׷�.
	FMOD::ChannelGroup* Master_Group = nullptr;
	// Sound �׷��� ���� BGM/SFX �ΰ�.
	FMOD::ChannelGroup* BGM_Group = nullptr;
	FMOD::ChannelGroup* SFX_Group = nullptr;
	// �ܺο��� �ε��� ���� ���ҽ����� ������ ����Ʈ.
	std::map<std::string, FMOD::Sound*> Sound_Resource;
	// ä���� ������ ä�� ����Ʈ
	std::vector<FMOD::Channel*> Channel_Vec;
	// BGM path
	std::string BGM_Path = "NonSet";
	// SFX path
	std::string SFX_Path = "NonSet";
	// ���� ����
	Sound_VFP Master_VFP;
	Sound_VFP SFX_VFP;
	Sound_VFP BGM_VFP;
	// ������ Up Down�� ȣ������ �� ����ġ
	float Volume_Factor = 0.1f;
	float Frequency_Factor = 100.f;
	float Pitch_Factor = 0.1f;

private:
	// Fmod Default Setting
	void Initialize();

	// Sound_Category�� ���� ChannelGroup ��ȯ.
	FMOD::ChannelGroup* GetChannelGroup(Sound_Category _Sound_Category);
	Sound_VFP* GetVFP(Sound_Category _Sound_Category);
	std::string* GetFolderPath(Sound_Category _Sound_Category);
	FMOD::Channel* GetChannel();

public:
	DHSound();
	~DHSound();
	// Fmod Update
	void Update();
	// ��μ���
	void SetSoundFolderPath(Sound_Category _Sound_Category, std::string _Folder_Path);
	// �ش� ������ �ҷ��ͼ� ����.
	void LoadSound(Sound_Category _Sound_Category, std::string _Sound_Key, std::string _File_Path, bool _Loop);
	// �Ҹ� ũ�� ����
	void SetVolumeFactor(float _Set_Factor);
	void VolumeSet(Sound_Category _Sound_Category, float _Set_Volume);
	void VolumeUp(Sound_Category _Sound_Category);
	void VolumeDown(Sound_Category _Sound_Category);
	// �Ҹ� Frequency ���� (������� ����)
	void SetFrequencyFactor(float _Set_Factor);
	void FrequencySet(Sound_Category _Sound_Category, float _Set_Frequency);
	void FrequencyUp(Sound_Category _Sound_Category);
	void FrequencyDown(Sound_Category _Sound_Category);
	// �Ҹ� Pitch ���� (����ӵ� ����� �Ұ�)
	void SetPitchFactor(float _Set_Factor);
	void PitchSet(Sound_Category _Sound_Category, float _Set_Pitch);
	void PitchUp(Sound_Category _Sound_Category);
	void PitchDown(Sound_Category _Sound_Category);
	// ���� ��� ����
	void SoundPlay(Sound_Category _Sound_Category, std::string _Sound_Key);
	void StopSound(Sound_Category _Sound_Category);
	void PauseSound(Sound_Category _Sound_Category, bool _Play);
	
/// 3D ����
private:
	std::map<std::string, Object_PosVelocity*> Sound_Object_List;
	Listener_PosFowardUpVelocity* _Listener = nullptr;

public:
	// ���⼭ Position �� ī�޶�� �Ѵٸ�, ī�޶� �߽��� �Ҹ��� ����, �÷��̾�ĳ���ͷ� �Ѵٸ� ĳ���� �߽��� �Ҹ��� ����.
	// Up�� Foward���� ī�޶� �������� �־���Ѵ�.
	void Set3DListener(SoundMath::Vector3 _Pos, SoundMath::Vector3 _Foward_Vec, SoundMath::Vector3 _Up_Vec, SoundMath::Vector3 _Velocity);
	void Set3DSoundObject(Sound_Category _Sound_Category, std::string _Sound_Key, SoundMath::Vector3 _Position, SoundMath::Vector3 _Velocity, std::string _Object_Name);
	void Delete3DSoundObject(std::string _Object_Name);
	// 3D����� �ɼ��� �޶� ���� �ε带 �������!
	void Load3DSound(Sound_Category _Sound_Category, std::string _Sound_Key, std::string _File_Path, bool _Loop);
	// ���÷� ������ ����.
	void Set3DDoppler(float _Scale, float _Distance_Factor, float _Roll_Off_Scale);
};

