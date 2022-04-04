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
	// ��μ���
	void SetSoundFolderPath(Sound_Category _Sound_Category, std::string _Folder_Path);
	// �ش� ������ �ҷ��ͼ� ����.
	void LoadSound(Sound_Category _Sound_Category, std::string _Sound_Key, std::string _File_Path, bool _Loop);
	// �Ҹ� ũ�� ����
	void SetVolumeFactor(float _Set_Factor);
	void VolumeSet(Sound_Category _Sound_Category, float _Set_Volume);
	void VolumeUp(Sound_Category _Sound_Category);
	void VolumeDown(Sound_Category _Sound_Category);
	// �Ҹ� Frequency ����
	void SetFrequencyFactor(float _Set_Factor);
	void FrequencySet(Sound_Category _Sound_Category, float _Set_Frequency);
	void FrequencyUp(Sound_Category _Sound_Category);
	void FrequencyDown(Sound_Category _Sound_Category);
	// �Ҹ� Pitch ����
	void SetPitchFactor(float _Set_Factor);
	void PitchSet(Sound_Category _Sound_Category, float _Set_Pitch);
	void PitchUp(Sound_Category _Sound_Category);
	void PitchDown(Sound_Category _Sound_Category);
	// ���� ��� ����
	void SoundPlay(Sound_Category _Sound_Category, std::string _Sound_Key);
	void StopSound(Sound_Category _Sound_Category);
	void PauseSound(Sound_Category _Sound_Category, bool _Play);

/// 3D ����
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

private:
	DHSound* m_Sound;
};