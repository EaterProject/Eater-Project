#include "DHSound.h"
#include <string>

DHSound::DHSound()
{
	Initialize();
}

DHSound::~DHSound()
{
	Sound_System->close();
}

void DHSound::Initialize()
{
	// �ý��� ���� �� �⺻ ���� ( CHANNEL_COUNT ��ŭ ä�� ���� )
	FMOD::System_Create(&Sound_System);
	Sound_System->init(CHANNEL_COUNT, FMOD_INIT_NORMAL, nullptr);
	// ä�� �׷� �̸� ����
	Sound_System->createChannelGroup("SFX", &SFX_Group);
	Sound_System->createChannelGroup("BGM", &BGM_Group);
	Sound_System->getMasterChannelGroup(&Master_Group);
	// �����ͱ׷쿡 SFX,BGM �׷� ���.
	Master_Group->addGroup(SFX_Group);
	Master_Group->addGroup(BGM_Group);

	Channel_Vec.resize(CHANNEL_COUNT);
}

FMOD::ChannelGroup* DHSound::GetChannelGroup(Sound_Category _Sound_Category)
{
	switch (_Sound_Category)
	{
	case Sound_Category::BGM:
		return BGM_Group;
	case Sound_Category::SFX:
		return SFX_Group;
	case Sound_Category::Master:
		return Master_Group;
	}
}

Sound_VFP* DHSound::GetVFP(Sound_Category _Sound_Category)
{
	switch (_Sound_Category)
	{
	case Sound_Category::BGM:
		return &BGM_VFP;
	case Sound_Category::SFX:
		return &SFX_VFP;
	case Sound_Category::Master:
		return &Master_VFP;
	}
}

std::string* DHSound::GetFolderPath(Sound_Category _Sound_Category)
{
	switch (_Sound_Category)
	{
	case Sound_Category::BGM:
		return &BGM_Path;
	case Sound_Category::SFX:
		return &SFX_Path;
	case Sound_Category::Master:
	{
		printf("[DHSound Wrong Category Error] Sound_Category�� ���� �߸� �Ǿ����ϴ�.");
		return nullptr;
	}
	}
}

FMOD::Channel* DHSound::GetChannel()
{
	bool _Is_Play = false;

	for (auto _Ready_Channel : Channel_Vec)
	{
		_Ready_Channel->isPlaying(&_Is_Play);
		if (_Is_Play == false)
		{
			return _Ready_Channel;
		}
	}

	printf("[DHSound Not Enough Channel] ä���� ��� ������Դϴ�.");
	return nullptr;
}

void DHSound::Update()
{
	// ��»��(û����)�� �־�� ������Ʈ�� �ǹ̰� ����.
	if (_Listener != nullptr)
	{
		_Listener->Update();
		Sound_System->set3DListenerAttributes(0, &_Listener->Fmod_Pos, &_Listener->Fmod_Vel, &_Listener->Fmod_Foward, &_Listener->Fmod_Up);
		// ��ϵ� �ּڰ��� �ִ� float������ �ǽð� ������Ʈ.
		for (auto _3D_Object : Sound_Object_List)
		{
			_3D_Object.second->Update();
			_3D_Object.second->SetAtt();
		}
	}

	Sound_System->update();
}

void DHSound::SetSoundFolderPath(Sound_Category _Sound_Category, std::string _Folder_Path)
{
	// �ش��ϴ� path �� ���´�.
	std::string* _Path = nullptr;
	_Path = GetFolderPath(_Sound_Category);

	if (_Path == nullptr)
	{
		return;
	}

	// �Է¹��� ��� ����.
	*_Path = _Folder_Path;
}

void DHSound::LoadSound(Sound_Category _Sound_Category, std::string _Sound_Key, std::string _File_Path, bool _Loop)
{
	// �ش��ϴ� path �� ���´�.
	std::string* _Path = nullptr;
	_Path = GetFolderPath(_Sound_Category);

	if (_Path == nullptr || *_Path == "NonSet")
	{
		printf("[DHSound LoadSound Error] Sound�� �о���� ���߽��ϴ�. ���� ��θ� ���� �����ϰų� �ùٸ� Category�� �����ϼ���.");
		return;
	}

	// �ش��ϴ� ���Ͽ� �� path�� �ϼ���Ų��.
	std::string _Totla_Path = *_Path + _File_Path;

	FMOD::Sound* _New_Sound = nullptr;

	// �ش��ϴ� ���� ����
	if (_Loop)	{ Sound_System->createSound(_Totla_Path.c_str(), FMOD_LOOP_NORMAL, nullptr, &_New_Sound); }
	else		{ Sound_System->createSound(_Totla_Path.c_str(), FMOD_DEFAULT, nullptr, &_New_Sound); }

	// ���� ����Ʈ�� �߰�.
	Sound_Resource.insert({ _Sound_Key, _New_Sound });
}

void DHSound::SetVolumeFactor(float _Set_Factor)
{
	Volume_Factor = _Set_Factor;
}

void DHSound::VolumeSet(Sound_Category _Sound_Category, float _Set_Volume)
{
	if (_Set_Volume < 0) { _Set_Volume = 0; }
	if (_Set_Volume > 1) { _Set_Volume = 1.f; }

	// �ش� ä�α׷�� �������� �����ͼ� ��������.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Vol = nullptr;
	_Vol = GetVFP(_Sound_Category);

	_Vol->Volume = _Set_Volume;
	_CG->setVolume(_Set_Volume);
}

void DHSound::VolumeUp(Sound_Category _Sound_Category)
{
	// �ش� ä�α׷�� �������� ������.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Vol = nullptr;
	_Vol = GetVFP(_Sound_Category);

	// ���� ������ ������ ����ġ�� ���� ������ �������� �����Ѵ�.
	_Vol->Volume += Volume_Factor;

	if (_Vol->Volume > 1) { _Vol->Volume = 1.f; }

	_CG->setVolume(_Vol->Volume);
}

void DHSound::VolumeDown(Sound_Category _Sound_Category)
{
	// �ش� ä�α׷�� �������� ������.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Vol = nullptr;
	_Vol = GetVFP(_Sound_Category);

	// ���� ������ ������ ����ġ�� ���� ������ ���ҽ��� �����Ѵ�.
	_Vol->Volume -= Volume_Factor;

	if (_Vol->Volume > 1) { _Vol->Volume = 1.f; }

	_CG->setVolume(_Vol->Volume);
}

void DHSound::SetFrequencyFactor(float _Set_Factor)
{
	Frequency_Factor = _Set_Factor;
}

void DHSound::FrequencySet(Sound_Category _Sound_Category, float _Set_Frequency)
{
	// �ش� ä�α׷�� �������ð��� �����ͼ� ��������.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Fre = nullptr;
	_Fre = GetVFP(_Sound_Category);

	_Fre->FreQuency = _Set_Frequency;
	_CG->setVolume(_Set_Frequency);
}

void DHSound::FrequencyUp(Sound_Category _Sound_Category)
{
	// �ش� ä�α׷�� �������ð��� ������.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Fre = nullptr;
	_Fre = GetVFP(_Sound_Category);

	// ���� ������ ���������� ����ġ�� ���� �������� �����Ѵ�.
	_Fre->FreQuency += Frequency_Factor;

	// ������ ä���� ��� �������ø� ����.
	int Channel_Nums = 0;
	_CG->getNumChannels(&Channel_Nums);

	for (int i = 0; i < Channel_Nums; i++)
	{
		FMOD::Channel* _Channel = nullptr;
		_CG->getChannel(i, &_Channel);
		_Channel->setFrequency(_Fre->FreQuency);
	}
}

void DHSound::FrequencyDown(Sound_Category _Sound_Category)
{
	// �ش� ä�α׷�� �������ð��� ������.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Fre = nullptr;
	_Fre = GetVFP(_Sound_Category);

	// ���� ������ ���������� ����ġ�� ���� �������� �����Ѵ�.
	_Fre->FreQuency -= Frequency_Factor;

	// ������ ä���� ��� �������ø� ����.
	int Channel_Nums = 0;
	_CG->getNumChannels(&Channel_Nums);

	for (int i = 0; i < Channel_Nums; i++)
	{
		FMOD::Channel* _Channel = nullptr;
		_CG->getChannel(i, &_Channel);
		_Channel->setFrequency(_Fre->FreQuency);
	}
}

void DHSound::SetPitchFactor(float _Set_Factor)
{
	Pitch_Factor = _Set_Factor;
}

void DHSound::PitchSet(Sound_Category _Sound_Category, float _Set_Pitch)
{
	if (_Set_Pitch < 0) { _Set_Pitch = 0; }
	// �ش� ä�α׷�� ��ġ���� �����ͼ� ��������.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Pitch = nullptr;
	_Pitch = GetVFP(_Sound_Category);

	_Pitch->Pitch = _Set_Pitch;
	_CG->setVolume(_Set_Pitch);
}

void DHSound::PitchUp(Sound_Category _Sound_Category)
{
	// �ش� ä�α׷�� ��ġ���� ������.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Pitch = nullptr;
	_Pitch = GetVFP(_Sound_Category);

	// ���� ������ ��ġ�� ����ġ�� ���� �������� �����Ѵ�.
	_Pitch->Pitch += Pitch_Factor;

	// ������ ä���� ��� ��ġ�� ����.
	int Channel_Nums = 0;
	_CG->getNumChannels(&Channel_Nums);


	for (int i = 0; i < Channel_Nums; i++)
	{
		FMOD::Channel* _Channel = nullptr;
		_CG->getChannel(i, &_Channel);
		_Channel->setPitch(_Pitch->Pitch);
	}
}

void DHSound::PitchDown(Sound_Category _Sound_Category)
{
	// �ش� ä�α׷�� ��ġ���� ������.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Pitch = nullptr;
	_Pitch = GetVFP(_Sound_Category);

	// ���� ������ ��ġ�� ����ġ�� ���� �������� �����Ѵ�.
	_Pitch->Pitch -= Pitch_Factor;

	if (_Pitch->Pitch < 0) { _Pitch->Pitch = 0.f; }

	// ������ ä���� ��� ��ġ�� ����.
	int Channel_Nums = 0;
	_CG->getNumChannels(&Channel_Nums);

	for (int i = 0; i < Channel_Nums; i++)
	{
		FMOD::Channel* _Channel = nullptr;
		_CG->getChannel(i, &_Channel);
		_Channel->setPitch(_Pitch->Pitch);
	}
}

void DHSound::SoundPlay(Sound_Category _Sound_Category, std::string _Sound_Key)
{
	if (_Sound_Category == Sound_Category::Master)
	{
		printf("[DHSound PlaySound Error] Matser Channel���� ���带 ����� �� �����ϴ�.");
		return;
	}

	if (Sound_Resource.find(_Sound_Key) == Sound_Resource.end())
	{
		printf("[DHSound PlaySound Error] �ش� ���帮�ҽ��� �������� �ʽ��ϴ�.");
		return;
	}

	// ����� ���带 ������.
	FMOD::Sound* _Play_Sound = nullptr;
	_Play_Sound = Sound_Resource[_Sound_Key];
	// �ش� ä�α׷��� ������.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	// ����� ä���� ������.
	FMOD::Channel* _Play_Channel = nullptr;
	_Play_Channel = GetChannel();
	// �ش� ���� ���
	Sound_System->playSound(_Play_Sound, _CG, false, &_Play_Channel);
	// ä�α׷쿡 ä�� ���
	_Play_Channel->setChannelGroup(_CG);
}

void DHSound::StopSound(Sound_Category _Sound_Category)
{
	// �ش� ä�α׷��� ������.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	// �ش� ä�α׷� �������.
	_CG->stop();
}

void DHSound::PauseSound(Sound_Category _Sound_Category, bool _Play)
{
	// �ش� ä�α׷��� ������.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	// ��� / �Ͻ����� ��û�� ���� ����.
	_CG->setPaused(_Play);
}

void DHSound::Set3DListener(SoundMath::Vector3 _Pos, SoundMath::Vector3 _Foward_Vec, SoundMath::Vector3 _Up_Vec, SoundMath::Vector3 _Velocity)
{
	// ���� �̹� ���õ� Listener �� �����Ѵٸ� �ش� �����ʸ� �����ϰ� ���� ���.
	if (_Listener != nullptr)
	{
		delete _Listener;
		_Listener = nullptr;
	}
	// �����ʿ� ���� ����ü�� ������ְ� �ʱⰪ�� �������ش�.
	_Listener = new Listener_PosFowardUpVelocity(_Pos, _Foward_Vec, _Up_Vec, _Velocity);
	Sound_System->set3DListenerAttributes(0, &_Listener->Fmod_Pos, &_Listener->Fmod_Vel, &_Listener->Fmod_Foward, &_Listener->Fmod_Up);
}

void DHSound::Set3DSoundObject(Sound_Category _Sound_Category, std::string _Sound_Key, SoundMath::Vector3 _Position, SoundMath::Vector3 _Velocity, std::string _Object_Name)
{
	if (_Sound_Category == Sound_Category::Master)
	{
		printf("[DHSound PlaySound Error] Matser Channel���� ���带 ����� �� �����ϴ�.");
		return;
	}

	if (Sound_Resource.find(_Sound_Key) == Sound_Resource.end())
	{
		printf("[DHSound PlaySound Error] �ش� ���帮�ҽ��� �������� �ʽ��ϴ�.");
		return;
	}

	// ����� ���带 ������.
	FMOD::Sound* _Play_Sound = Sound_Resource[_Sound_Key];
	// �ش� ä�α׷��� ������.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	// ����� ä���� ������.
	FMOD::Channel* _Play_Channel = GetChannel();
	// �ش� ���� ���
	Sound_System->playSound(_Play_Sound, _CG, false, &_Play_Channel);
	// ä�α׷쿡 ä�� ���
	_Play_Channel->setChannelGroup(_CG);
	// �ش� ������Ʈ�� ���� ������, �ӵ����� �����ص� ����ü ����.
	Object_PosVelocity* _New_Object_Data = new Object_PosVelocity(_Play_Channel, _Position, _Velocity);
	// �ش� ������Ʈ�� 3D ������Ʈ�� Fmod�� ���
	_Play_Channel->set3DAttributes(&_New_Object_Data->Fmod_Pos, &_New_Object_Data->Fmod_Pos);
	// �ش� �����͸� ������Ʈ �̸��� ���� ���� ����Ʈ�� �߰�.
	Sound_Object_List.insert({ _Object_Name, _New_Object_Data });
}

void DHSound::Delete3DSoundObject(std::string _Object_Name)
{
	// �ش� ������Ʈ �̸��� �ش��ϴ� iterator�� �޾ƿ´�.
	auto itr = Sound_Object_List.find(_Object_Name);
	if (itr == Sound_Object_List.end())
	{
		printf("[DHSound 3DObject Delete Error] �������� �ʴ� 3D ���� ������Ʈ�� �����Ϸ� �߽��ϴ�.");
		return;
	}

	// �ش� ä���� ��������ְ�
	Sound_Object_List[_Object_Name]->m_Channel->stop();
	// �ش� ���� ����.
	Sound_Object_List.erase(itr);
}

void DHSound::Load3DSound(Sound_Category _Sound_Category, std::string _Sound_Key, std::string _File_Path, bool _Loop)
{
	// �ش��ϴ� path �� ���´�.
	std::string* _Path = nullptr;
	_Path = GetFolderPath(_Sound_Category);

	if (_Path == nullptr || *_Path == "NonSet")
	{
		printf("[DHSound Load3DSound Error] Sound�� �о���� ���߽��ϴ�. ���� ��θ� ���� �����ϰų� �ùٸ� Category�� �����ϼ���.");
		return;
	}

	// �ش��ϴ� ���Ͽ� �� path�� �ϼ���Ų��.
	std::string _Totla_Path = *_Path + _File_Path;

	FMOD::Sound* _New_Sound = nullptr;

	// �ش��ϴ� ���� ����
	if (_Loop) { Sound_System->createSound(_Totla_Path.c_str(), FMOD_3D | FMOD_LOOP_NORMAL, nullptr, &_New_Sound); }
	else { Sound_System->createSound(_Totla_Path.c_str(), FMOD_3D, nullptr, &_New_Sound); }

	// ���� ����Ʈ�� �߰�.
	Sound_Resource.insert({ _Sound_Key, _New_Sound });
}

void DHSound::Set3DDoppler(float _Scale, float _Distance_Factor, float _Roll_Off_Scale)
{
	Sound_System->set3DSettings(_Scale, _Distance_Factor, _Roll_Off_Scale);
}
