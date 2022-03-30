#include "EaterSound.h"
#include "DHSound.h"
#include <DirectXMath.h>

EaterSound* EaterSound::m_instance = nullptr;

EaterSound::EaterSound()
{
	m_Sound = new DHSound;
}

EaterSound::~EaterSound()
{
	
}


EaterSound* EaterSound::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new EaterSound();
	}

	return m_instance;
}

void EaterSound::Release()
{
	delete m_Sound;
	delete this;
}

void EaterSound::Update()
{
	m_Sound->Update();
}

void EaterSound::SetSoundFolderPath(Sound_Category _Sound_Category, std::string _Folder_Path)
{
	m_Sound->SetSoundFolderPath(_Sound_Category, _Folder_Path);
}

void EaterSound::LoadSound(Sound_Category _Sound_Category, std::string _Sound_Key, std::string _File_Path, bool _Loop)
{
	m_Sound->LoadSound(_Sound_Category, _Sound_Key, _File_Path, _Loop);
}

void EaterSound::SetVolumeFactor(float _Set_Factor)
{
	m_Sound->SetVolumeFactor(_Set_Factor);
}

void EaterSound::VolumeSet(Sound_Category _Sound_Category, float _Set_Volume)
{
	m_Sound->VolumeSet(_Sound_Category, _Set_Volume);
}

void EaterSound::VolumeUp(Sound_Category _Sound_Category)
{
	m_Sound->VolumeUp(_Sound_Category);
}

void EaterSound::VolumeDown(Sound_Category _Sound_Category)
{
	m_Sound->VolumeDown(_Sound_Category);
}

void EaterSound::SetFrequencyFactor(float _Set_Factor)
{
	m_Sound->SetFrequencyFactor(_Set_Factor);
}

void EaterSound::FrequencySet(Sound_Category _Sound_Category, float _Set_Frequency)
{
	m_Sound->FrequencySet(_Sound_Category, _Set_Frequency);
}

void EaterSound::FrequencyUp(Sound_Category _Sound_Category)
{
	m_Sound->FrequencyUp(_Sound_Category);
}

void EaterSound::FrequencyDown(Sound_Category _Sound_Category)
{
	m_Sound->FrequencyDown(_Sound_Category);
}

void EaterSound::SetPitchFactor(float _Set_Factor)
{
	m_Sound->SetPitchFactor(_Set_Factor);
}

void EaterSound::PitchSet(Sound_Category _Sound_Category, float _Set_Pitch)
{
	m_Sound->PitchSet(_Sound_Category, _Set_Pitch);
}

void EaterSound::PitchUp(Sound_Category _Sound_Category)
{
	m_Sound->PitchUp(_Sound_Category);
}

void EaterSound::PitchDown(Sound_Category _Sound_Category)
{
	m_Sound->PitchDown(_Sound_Category);
}

void EaterSound::SoundPlay(Sound_Category _Sound_Category, std::string _Sound_Key)
{
	m_Sound->SoundPlay(_Sound_Category, _Sound_Key);
}

void EaterSound::StopSound(Sound_Category _Sound_Category)
{
	m_Sound->StopSound(_Sound_Category);
}

void EaterSound::PauseSound(Sound_Category _Sound_Category, bool _Play)
{
	m_Sound->PauseSound(_Sound_Category, _Play);
}

void EaterSound::Set3DListener(SoundMath::Vector3 _Pos, SoundMath::Vector3 _Foward_Vec, SoundMath::Vector3 _Up_Vec, SoundMath::Vector3 _Velocity)
{
	m_Sound->Set3DListener(_Pos, _Foward_Vec, _Up_Vec, _Velocity);
}

void EaterSound::Set3DSoundObject(Sound_Category _Sound_Category, std::string _Sound_Key, SoundMath::Vector3 _Position, SoundMath::Vector3 _Velocity, std::string _Object_Name)
{
	m_Sound->Set3DSoundObject(_Sound_Category, _Sound_Key, _Position, _Velocity, _Object_Name);
}

void EaterSound::Delete3DSoundObject(std::string _Object_Name)
{
	m_Sound->Delete3DSoundObject(_Object_Name);
}

void EaterSound::Load3DSound(Sound_Category _Sound_Category, std::string _Sound_Key, std::string _File_Path, bool _Loop)
{
	m_Sound->Load3DSound(_Sound_Category, _Sound_Key, _File_Path, _Loop);
}

void EaterSound::Set3DDoppler(float _Scale, float _Distance_Factor, float _Roll_Off_Scale)
{
	m_Sound->Set3DDoppler(_Scale, _Distance_Factor, _Roll_Off_Scale);
}
