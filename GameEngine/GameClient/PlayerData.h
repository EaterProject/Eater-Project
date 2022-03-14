#pragma once

//�÷��̾� ������ ����ü
class PlayerData
{
public:
	PlayerData() = default;
	~PlayerData() = default;
public:
	//��ġ
	Vector3 Position;
	//�ִϸ��̼� Ÿ��
	int AnimationType;
	//��ü ��ȣ
	int ObjectNumber;

	double Passed_Time;

	int SequenceNumber;
};


//������ ����
class FrameData
{
public:
	FrameData() = default;
	~FrameData()= default;

	void Reset()
	{
		m_Sequence	= -1;
		Position	= { 0,0,0 };
		Direction	= { 0,0,0 };
		Dtime		= 0.0f;
	}

	int		m_Sequence;		//������ �ѹ�
	Vector3 Position;		//��ġ ����
	Vector3 Direction;		//���� ����
	double	Dtime;			//���� ������ ����

	bool Run = false;		//�� �����͸� ��������� �ƴ��� ����
};


