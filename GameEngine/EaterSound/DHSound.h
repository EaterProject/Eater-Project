#pragma once

#include "DHSoundDefine.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#include <map>
#include <vector>

// 3D상 리스너의 Position / Forward / Up / Velocity 의 포인터를 저장할 구조체.
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

	// Update 문에서 실시간 포지션을 계속 가져올 필요없이 포인터의 값을 기반으로 업데이트하기 위해 들고있음.
	SoundMath::Vector3 Pos;
	SoundMath::Vector3 Foward_Vec;
	SoundMath::Vector3 Up_Vec;
	SoundMath::Vector3 Velocity;
	// Fmod 에 등록할때 념겨줄 구조체.
	FMOD_VECTOR Fmod_Pos;
	FMOD_VECTOR Fmod_Foward;
	FMOD_VECTOR Fmod_Up;
	FMOD_VECTOR Fmod_Vel;
};

// 3D상 존재하는 소리 오브젝트들의 Position / Velocity 의 포인터와 채널을 저장할 구조체.
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
	// Update 문에서 실시간 포지션을 계속 가져올 필요없이 포인터의 값을 기반으로 업데이트하기 위해 들고있음.
	SoundMath::Vector3 Pos;
	SoundMath::Vector3 Velocity;
	// Fmod 에 등록할때 념겨줄 구조체.
	FMOD_VECTOR Fmod_Pos;
	FMOD_VECTOR Fmod_Vel;
};

class DHSound
{
private:
	// Fmod 시스템
	FMOD::System* Sound_System = nullptr;
	// 총 사운드에 대한 마스터그룹.
	FMOD::ChannelGroup* Master_Group = nullptr;
	// Sound 그룹의 종류 BGM/SFX 두개.
	FMOD::ChannelGroup* BGM_Group = nullptr;
	FMOD::ChannelGroup* SFX_Group = nullptr;
	// 외부에서 로드한 사운드 리소스들을 관리할 리스트.
	std::map<std::string, FMOD::Sound*> Sound_Resource;
	// 채널을 관리할 채널 리스트
	std::vector<FMOD::Channel*> Channel_Vec;
	// BGM path
	std::string BGM_Path = "NonSet";
	// SFX path
	std::string SFX_Path = "NonSet";
	// 사운드 설정
	Sound_VFP Master_VFP;
	Sound_VFP SFX_VFP;
	Sound_VFP BGM_VFP;
	// 볼륨의 Up Down을 호출했을 때 가중치
	float Volume_Factor = 0.1f;
	float Frequency_Factor = 100.f;
	float Pitch_Factor = 0.1f;

private:
	// Fmod Default Setting
	void Initialize();

	// Sound_Category에 따라 ChannelGroup 반환.
	FMOD::ChannelGroup* GetChannelGroup(Sound_Category _Sound_Category);
	Sound_VFP* GetVFP(Sound_Category _Sound_Category);
	std::string* GetFolderPath(Sound_Category _Sound_Category);
	FMOD::Channel* GetChannel();

public:
	DHSound();
	~DHSound();
	// Fmod Update
	void Update();
	// 경로설정
	void SetSoundFolderPath(Sound_Category _Sound_Category, std::string _Folder_Path);
	// 해당 파일을 불러와서 세팅.
	void LoadSound(Sound_Category _Sound_Category, std::string _Sound_Key, std::string _File_Path, bool _Loop);
	// 소리 크기 설정
	void SetVolumeFactor(float _Set_Factor);
	void VolumeSet(Sound_Category _Sound_Category, float _Set_Volume);
	void VolumeUp(Sound_Category _Sound_Category);
	void VolumeDown(Sound_Category _Sound_Category);
	// 소리 Frequency 설정 (역재생도 가능)
	void SetFrequencyFactor(float _Set_Factor);
	void FrequencySet(Sound_Category _Sound_Category, float _Set_Frequency);
	void FrequencyUp(Sound_Category _Sound_Category);
	void FrequencyDown(Sound_Category _Sound_Category);
	// 소리 Pitch 설정 (재생속도 역재생 불가)
	void SetPitchFactor(float _Set_Factor);
	void PitchSet(Sound_Category _Sound_Category, float _Set_Pitch);
	void PitchUp(Sound_Category _Sound_Category);
	void PitchDown(Sound_Category _Sound_Category);
	// 사운드 재생 관련
	void SoundPlay(Sound_Category _Sound_Category, std::string _Sound_Key);
	void StopSound(Sound_Category _Sound_Category);
	void PauseSound(Sound_Category _Sound_Category, bool _Play);
	
/// 3D 사운드
private:
	std::map<std::string, Object_PosVelocity*> Sound_Object_List;
	Listener_PosFowardUpVelocity* _Listener = nullptr;

public:
	// 여기서 Position 을 카메라로 한다면, 카메라 중심의 소리가 나고, 플레이어캐릭터로 한다면 캐릭터 중심의 소리가 난다.
	// Up과 Foward등은 카메라 기준으로 넣어야한다.
	void Set3DListener(SoundMath::Vector3 _Pos, SoundMath::Vector3 _Foward_Vec, SoundMath::Vector3 _Up_Vec, SoundMath::Vector3 _Velocity);
	void Set3DSoundObject(Sound_Category _Sound_Category, std::string _Sound_Key, SoundMath::Vector3 _Position, SoundMath::Vector3 _Velocity, std::string _Object_Name);
	void Delete3DSoundObject(std::string _Object_Name);
	// 3D사운드는 옵션이 달라서 따로 로드를 해줘야함!
	void Load3DSound(Sound_Category _Sound_Category, std::string _Sound_Key, std::string _File_Path, bool _Loop);
	// 도플러 스케일 설정.
	void Set3DDoppler(float _Scale, float _Distance_Factor, float _Roll_Off_Scale);
};

