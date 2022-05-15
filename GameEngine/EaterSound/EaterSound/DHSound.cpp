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
	// 시스템 생성 및 기본 세팅 ( CHANNEL_COUNT 만큼 채널 생성 )
	FMOD::System_Create(&Sound_System);
	Sound_System->init(CHANNEL_COUNT, FMOD_INIT_NORMAL, nullptr);
	// 채널 그룹 미리 생성
	Sound_System->createChannelGroup("SFX", &SFX_Group);
	Sound_System->createChannelGroup("BGM", &BGM_Group);
	Sound_System->getMasterChannelGroup(&Master_Group);
	// 마스터그룹에 SFX,BGM 그룹 배당.
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
		printf("[DHSound Wrong Category Error] Sound_Category의 값이 잘못 되었습니다.");
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

	printf("[DHSound Not Enough Channel] 채널이 모두 사용중입니다.");
	return nullptr;
}

void DHSound::Update()
{
	// 듣는사람(청취자)가 있어야 업데이트가 의미가 있음.
	if (_Listener != nullptr)
	{
		_Listener->Update();
		Sound_System->set3DListenerAttributes(0, &_Listener->Fmod_Pos, &_Listener->Fmod_Vel, &_Listener->Fmod_Foward, &_Listener->Fmod_Up);
		// 등록된 주솟값에 있는 float값으로 실시간 업데이트.
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
	// 해당하는 path 를 들고온다.
	std::string* _Path = nullptr;
	_Path = GetFolderPath(_Sound_Category);

	if (_Path == nullptr)
	{
		return;
	}

	// 입력받은 경로 설정.
	*_Path = _Folder_Path;
}

void DHSound::LoadSound(Sound_Category _Sound_Category, std::string _Sound_Key, std::string _File_Path, bool _Loop)
{
	// 해당하는 path 를 들고온다.
	std::string* _Path = nullptr;
	_Path = GetFolderPath(_Sound_Category);

	if (_Path == nullptr || *_Path == "NonSet")
	{
		printf("[DHSound LoadSound Error] Sound를 읽어오지 못했습니다. 폴더 경로를 먼저 설정하거나 올바른 Category를 지정하세요.");
		return;
	}

	// 해당하는 파일에 총 path를 완성시킨다.
	std::string _Totla_Path = *_Path + _File_Path;

	FMOD::Sound* _New_Sound = nullptr;

	// 해당하는 사운드 생성
	if (_Loop)	{ Sound_System->createSound(_Totla_Path.c_str(), FMOD_LOOP_NORMAL, nullptr, &_New_Sound); }
	else		{ Sound_System->createSound(_Totla_Path.c_str(), FMOD_DEFAULT, nullptr, &_New_Sound); }

	// 사운드 리스트에 추가.
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

	// 해당 채널그룹과 볼륨값을 가져와서 세팅해줌.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Vol = nullptr;
	_Vol = GetVFP(_Sound_Category);

	_Vol->Volume = _Set_Volume;
	_CG->setVolume(_Set_Volume);
}

void DHSound::VolumeUp(Sound_Category _Sound_Category)
{
	// 해당 채널그룹과 볼륨값을 가져옴.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Vol = nullptr;
	_Vol = GetVFP(_Sound_Category);

	// 현재 지정된 볼륨의 가중치에 따라 볼륨을 증가시켜 적용한다.
	_Vol->Volume += Volume_Factor;

	if (_Vol->Volume > 1) { _Vol->Volume = 1.f; }

	_CG->setVolume(_Vol->Volume);
}

void DHSound::VolumeDown(Sound_Category _Sound_Category)
{
	// 해당 채널그룹과 볼륨값을 가져옴.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Vol = nullptr;
	_Vol = GetVFP(_Sound_Category);

	// 현재 지정된 볼륨의 가중치에 따라 볼륨을 감소시켜 적용한다.
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
	// 해당 채널그룹과 프리퀀시값을 가져와서 세팅해줌.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Fre = nullptr;
	_Fre = GetVFP(_Sound_Category);

	_Fre->FreQuency = _Set_Frequency;
	_CG->setVolume(_Set_Frequency);
}

void DHSound::FrequencyUp(Sound_Category _Sound_Category)
{
	// 해당 채널그룹과 프리퀀시값을 가져옴.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Fre = nullptr;
	_Fre = GetVFP(_Sound_Category);

	// 현재 지정된 프리퀀시의 가중치에 따라 증가시켜 적용한다.
	_Fre->FreQuency += Frequency_Factor;

	// 지정된 채널의 모든 프리퀀시를 높임.
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
	// 해당 채널그룹과 프리퀀시값을 가져옴.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Fre = nullptr;
	_Fre = GetVFP(_Sound_Category);

	// 현재 지정된 프리퀀시의 가중치에 따라 증가시켜 적용한다.
	_Fre->FreQuency -= Frequency_Factor;

	// 지정된 채널의 모든 프리퀀시를 낮춤.
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
	// 해당 채널그룹과 피치값을 가져와서 세팅해줌.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Pitch = nullptr;
	_Pitch = GetVFP(_Sound_Category);

	_Pitch->Pitch = _Set_Pitch;
	_CG->setVolume(_Set_Pitch);
}

void DHSound::PitchUp(Sound_Category _Sound_Category)
{
	// 해당 채널그룹과 피치값을 가져옴.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Pitch = nullptr;
	_Pitch = GetVFP(_Sound_Category);

	// 현재 지정된 피치의 가중치에 따라 증가시켜 적용한다.
	_Pitch->Pitch += Pitch_Factor;

	// 지정된 채널의 모든 피치를 높임.
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
	// 해당 채널그룹과 피치값을 가져옴.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Pitch = nullptr;
	_Pitch = GetVFP(_Sound_Category);

	// 현재 지정된 피치의 가중치에 따라 증가시켜 적용한다.
	_Pitch->Pitch -= Pitch_Factor;

	if (_Pitch->Pitch < 0) { _Pitch->Pitch = 0.f; }

	// 지정된 채널의 모든 피치를 낮춤.
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
		printf("[DHSound PlaySound Error] Matser Channel에선 사운드를 재생할 수 없습니다.");
		return;
	}

	if (Sound_Resource.find(_Sound_Key) == Sound_Resource.end())
	{
		printf("[DHSound PlaySound Error] 해당 사운드리소스가 존재하지 않습니다.");
		return;
	}

	// 재생할 사운드를 가져옴.
	FMOD::Sound* _Play_Sound = nullptr;
	_Play_Sound = Sound_Resource[_Sound_Key];
	// 해당 채널그룹을 가져옴.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	// 재생할 채널을 가져옴.
	FMOD::Channel* _Play_Channel = nullptr;
	_Play_Channel = GetChannel();
	// 해당 사운드 재생
	Sound_System->playSound(_Play_Sound, _CG, false, &_Play_Channel);
	// 채널그룹에 채널 등록
	_Play_Channel->setChannelGroup(_CG);
}

void DHSound::StopSound(Sound_Category _Sound_Category)
{
	// 해당 채널그룹을 가져옴.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	// 해당 채널그룹 재생중지.
	_CG->stop();
}

void DHSound::PauseSound(Sound_Category _Sound_Category, bool _Play)
{
	// 해당 채널그룹을 가져옴.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	// 재생 / 일시중지 요청에 따라 실행.
	_CG->setPaused(_Play);
}

void DHSound::Set3DListener(SoundMath::Vector3 _Pos, SoundMath::Vector3 _Foward_Vec, SoundMath::Vector3 _Up_Vec, SoundMath::Vector3 _Velocity)
{
	// 만약 이미 셋팅된 Listener 가 존재한다면 해당 리스너를 제거하고 새로 등록.
	if (_Listener != nullptr)
	{
		delete _Listener;
		_Listener = nullptr;
	}
	// 리스너에 대한 구조체를 만들어주고 초기값을 설정해준다.
	_Listener = new Listener_PosFowardUpVelocity(_Pos, _Foward_Vec, _Up_Vec, _Velocity);
	Sound_System->set3DListenerAttributes(0, &_Listener->Fmod_Pos, &_Listener->Fmod_Vel, &_Listener->Fmod_Foward, &_Listener->Fmod_Up);
}

void DHSound::Set3DSoundObject(Sound_Category _Sound_Category, std::string _Sound_Key, SoundMath::Vector3 _Position, SoundMath::Vector3 _Velocity, std::string _Object_Name)
{
	if (_Sound_Category == Sound_Category::Master)
	{
		printf("[DHSound PlaySound Error] Matser Channel에선 사운드를 재생할 수 없습니다.");
		return;
	}

	if (Sound_Resource.find(_Sound_Key) == Sound_Resource.end())
	{
		printf("[DHSound PlaySound Error] 해당 사운드리소스가 존재하지 않습니다.");
		return;
	}

	// 재생할 사운드를 가져옴.
	FMOD::Sound* _Play_Sound = Sound_Resource[_Sound_Key];
	// 해당 채널그룹을 가져옴.
	FMOD::ChannelGroup* _CG = nullptr;
	_CG = GetChannelGroup(_Sound_Category);
	// 재생할 채널을 가져옴.
	FMOD::Channel* _Play_Channel = GetChannel();
	// 해당 사운드 재생
	Sound_System->playSound(_Play_Sound, _CG, false, &_Play_Channel);
	// 채널그룹에 채널 등록
	_Play_Channel->setChannelGroup(_CG);
	// 해당 오브젝트에 대한 포지션, 속도값을 저장해둘 구조체 선언.
	Object_PosVelocity* _New_Object_Data = new Object_PosVelocity(_Play_Channel, _Position, _Velocity);
	// 해당 오브젝트를 3D 오브젝트로 Fmod에 등록
	_Play_Channel->set3DAttributes(&_New_Object_Data->Fmod_Pos, &_New_Object_Data->Fmod_Pos);
	// 해당 데이터를 오브젝트 이름과 같이 관리 리스트에 추가.
	Sound_Object_List.insert({ _Object_Name, _New_Object_Data });
}

void DHSound::Delete3DSoundObject(std::string _Object_Name)
{
	// 해당 오브젝트 이름에 해당하는 iterator를 받아온다.
	auto itr = Sound_Object_List.find(_Object_Name);
	if (itr == Sound_Object_List.end())
	{
		printf("[DHSound 3DObject Delete Error] 존재하지 않는 3D 사운드 오브젝트를 삭제하려 했습니다.");
		return;
	}

	// 해당 채널을 종료시켜주고
	Sound_Object_List[_Object_Name]->m_Channel->stop();
	// 해당 값을 삭제.
	Sound_Object_List.erase(itr);
}

void DHSound::Load3DSound(Sound_Category _Sound_Category, std::string _Sound_Key, std::string _File_Path, bool _Loop)
{
	// 해당하는 path 를 들고온다.
	std::string* _Path = nullptr;
	_Path = GetFolderPath(_Sound_Category);

	if (_Path == nullptr || *_Path == "NonSet")
	{
		printf("[DHSound Load3DSound Error] Sound를 읽어오지 못했습니다. 폴더 경로를 먼저 설정하거나 올바른 Category를 지정하세요.");
		return;
	}

	// 해당하는 파일에 총 path를 완성시킨다.
	std::string _Totla_Path = *_Path + _File_Path;

	FMOD::Sound* _New_Sound = nullptr;

	// 해당하는 사운드 생성
	if (_Loop) { Sound_System->createSound(_Totla_Path.c_str(), FMOD_3D | FMOD_LOOP_NORMAL, nullptr, &_New_Sound); }
	else { Sound_System->createSound(_Totla_Path.c_str(), FMOD_3D, nullptr, &_New_Sound); }

	// 사운드 리스트에 추가.
	Sound_Resource.insert({ _Sound_Key, _New_Sound });
}

void DHSound::Set3DDoppler(float _Scale, float _Distance_Factor, float _Roll_Off_Scale)
{
	Sound_System->set3DSettings(_Scale, _Distance_Factor, _Roll_Off_Scale);
}
