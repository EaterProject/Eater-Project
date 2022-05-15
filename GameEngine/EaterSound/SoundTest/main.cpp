#include "EaterSound.h"
#include <DirectXMath.h>
#include "KeyIO.h"

using namespace DirectX;

int main()
{
	EaterSound* m_Sound = EaterSound::GetInstance();

	/// 일반 사운드 예시

	// 파일경로 세팅
	m_Sound->SetSoundFolderPath(Sound_Category::BGM, "../TestResource/BGM/");
	m_Sound->SetSoundFolderPath(Sound_Category::SFX, "../TestResource/SFX/");

	// 사운드 로드
	m_Sound->LoadSound(Sound_Category::BGM, "Ending", "BGM_Ending.mp3", true);
	m_Sound->LoadSound(Sound_Category::SFX, "Siren", "SFX_Siren.wav", false);

	// BGM 재생
	//m_Sound->SoundPlay(Sound_Category::BGM, "Ending");
	//m_Sound->SoundPlay(Sound_Category::SFX, "Siren");

	/// 3D 사운드 예시

	// 도플러 효과 설정
	m_Sound->Set3DDoppler(1.f, 2.f, 1.f);

	// 3D 사운드 로드
	m_Sound->Load3DSound(Sound_Category::BGM, "Vitality", "Vitality.mp3", true);

	// 임시 카메라 객체
	XMFLOAT3 _Pos(0, 0, 0);
	XMFLOAT3 _Up(0, 1, 0);
	XMFLOAT3 _Foward(1, 0, 0);
	XMFLOAT3 _Velocity(0, 0, 0);

	// 임시 오브젝트
	XMFLOAT3 _Pos_Obj(0, 0, 0);
	XMFLOAT3 _Velocity_Obj(0, 0, 0);

	m_Sound->Set3DListener(
		SoundMath::Vector3(&_Pos.x, &_Pos.y, &_Pos.z),
		SoundMath::Vector3(&_Foward.x, &_Foward.y, &_Foward.z),
		SoundMath::Vector3(&_Up.x, &_Up.y, &_Up.z),
		SoundMath::Vector3(&_Velocity.x, &_Velocity.y, &_Velocity.z)
	);

	m_Sound->Set3DSoundObject(Sound_Category::BGM, "Vitality",
		SoundMath::Vector3(&_Pos_Obj.x, &_Pos_Obj.y, &_Pos_Obj.z),
		SoundMath::Vector3(&_Velocity_Obj.x, &_Velocity_Obj.y, &_Velocity_Obj.z),
		"TestObject"
	);

	while (true)
	{
		m_Sound->Update();

		// 포지션 감소
		if (KeyIO::GetInstance()->Is_KeyDown_Keep('E'))
		{
			_Pos_Obj.x -= 0.0003;
		}
		// 포지션 증가
		if (KeyIO::GetInstance()->Is_KeyDown_Keep('R'))
		{
			_Pos_Obj.x += 0.0003;
		}
		// 포지션 감소
		if (KeyIO::GetInstance()->Is_KeyDown_Keep('D'))
		{
			_Velocity_Obj.x -= 0.001;
		}
		// 포지션 증가
		if (KeyIO::GetInstance()->Is_KeyDown_Keep('F'))
		{
			_Velocity_Obj.x += 0.001;
		}

		printf("오브젝트 위치 : %.4f \t\t 오브젝트 속도 : %.4f\n", _Pos_Obj.x, _Velocity_Obj.x);

		// 소리재생 멈춤
		if (KeyIO::GetInstance()->Is_KeyDown(VK_F1))
		{
			m_Sound->PauseSound(Sound_Category::BGM, true);
		}
		// 다시 재생
		if (KeyIO::GetInstance()->Is_KeyDown(VK_F2))
		{
			m_Sound->PauseSound(Sound_Category::BGM, false);
		}
		// 볼륨 줄이기
		if (KeyIO::GetInstance()->Is_KeyDown(VK_F3))
		{
			m_Sound->VolumeDown(Sound_Category::BGM);
		}
		// 볼륨 높이기
		if (KeyIO::GetInstance()->Is_KeyDown(VK_F4))
		{
			m_Sound->VolumeUp(Sound_Category::BGM);
		}
		// 프리퀀시 높이기
		if (KeyIO::GetInstance()->Is_KeyDown('W'))
		{
			m_Sound->FrequencyUp(Sound_Category::BGM);
		}
		// 프리퀀시 낮추기
		if (KeyIO::GetInstance()->Is_KeyDown('Q'))
		{
			m_Sound->FrequencyDown(Sound_Category::BGM);
		}
		// 피치 높이기
		if (KeyIO::GetInstance()->Is_KeyDown('S'))
		{
			m_Sound->PitchUp(Sound_Category::BGM);
		}
		// 피치 낮추기
		if (KeyIO::GetInstance()->Is_KeyDown('A'))
		{
			m_Sound->PitchDown(Sound_Category::BGM);
		}
	}

}