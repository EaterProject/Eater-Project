#pragma once

#include "DHSoundDefine.h"

class DHSound;

class EATERSOUND_DLL EaterSound
{
private:
	static EaterSound* m_instance;

	EaterSound();
	~EaterSound();

public:
	// Get Class Pointer
	static EaterSound* GetInstance();
	// Class Release();
	void Release();
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
	// 소리 Frequency 설정
	void SetFrequencyFactor(float _Set_Factor);
	void FrequencySet(Sound_Category _Sound_Category, float _Set_Frequency);
	void FrequencyUp(Sound_Category _Sound_Category);
	void FrequencyDown(Sound_Category _Sound_Category);
	// 소리 Pitch 설정
	void SetPitchFactor(float _Set_Factor);
	void PitchSet(Sound_Category _Sound_Category, float _Set_Pitch);
	void PitchUp(Sound_Category _Sound_Category);
	void PitchDown(Sound_Category _Sound_Category);
	// 사운드 재생 관련
	void SoundPlay(Sound_Category _Sound_Category, std::string _Sound_Key);
	void StopSound(Sound_Category _Sound_Category);
	void PauseSound(Sound_Category _Sound_Category, bool _Play);

/// 3D 사운드
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

private:
	DHSound* m_Sound;
};