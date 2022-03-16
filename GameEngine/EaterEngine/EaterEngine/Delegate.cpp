#include "Delegate.h"

Delegate_Map::Delegate_Map()
{
}

Delegate_Map::~Delegate_Map()
{
}

void Delegate_Map::Push(ComponentFunctionData temp)
{
	//�������°� �Լ������� ����Ʈ�� �־��ش�
	switch (temp.OrderCount)
	{
	case 0:
		FunctionList00.push_back(temp);
		break;
	case 1:
		FunctionList01.push_back(temp);
		break;
	case 2:
		FunctionList02.push_back(temp);
		break;
	case 3:
		FunctionList03.push_back(temp);
		break;
	case 4:
		FunctionList04.push_back(temp);
		break;
	}
}

void Delegate_Map::Pop(Component* Key)
{
	std::vector<ComponentFunctionData>* temp = nullptr;
	for (int i = 0; i < MaxCount; i++)
	{
		switch (i)
		{
		case 0:
			temp = &FunctionList00;
			break;
		case 1:
			temp = &FunctionList01;
			break;
		case 2:
			temp = &FunctionList02;
			break;
		case 3:
			temp = &FunctionList03;
			break;
		case 4:
			temp = &FunctionList04;
			break;
		}


		int size = (int)temp->size();
		for (int j = 0; j < size; j++)
		{
			//���ϴ� �Լ��� ã�Ҵٸ�
			if ((*temp)[j].ComponentPointer == Key)
			{
				(*temp).erase((*temp).begin() + j);
				return;
			}
		}
	}

}

void Delegate_Map::Clear()
{
	FunctionList00.clear();
	FunctionList01.clear();
	FunctionList02.clear();
	FunctionList03.clear();
	FunctionList04.clear();
}

void Delegate_Map::PlayOnce()
{
	for (int i = 0; i < MaxCount; i++)
	{
		std::vector<ComponentFunctionData>* temp = nullptr;
		//� �Լ������� ����Ʈ�� ���������� üũ
		switch (i)
		{
		case 0:
			temp = &FunctionList00;
			break;
		case 1:
			temp = &FunctionList01;
			break;
		case 2:
			temp = &FunctionList02;
			break;
		case 3:
			temp = &FunctionList03;
			break;
		case 4:
			temp = &FunctionList04;
			break;
		}

		int Index = 0;
		while (true)
		{
			//����Ʈ�� ���� �ٵ��Ҵٸ�
			if (Index == (int)(temp->size()))
			{
				break;
			}

			//�Լ������Ͱ��������
			if ((*temp)[Index].FunctionPointer == nullptr)
			{
				Index++;
				continue;
			}
			//Ȱ��ȭ������ �ƴѰ��
			if (*((*temp)[Index].Enabled) == false)
			{
				Index++;
				continue;
			}

			//�Լ� ����
			(*temp)[Index].FunctionPointer();
			Index++;
		}

		//�ѹ��� ����Ǵ� �Լ������Ͷ�� ��� ������ ����
		(*temp).clear();
	}
}

void Delegate_Map::Play()
{
	for (int i = 0; i < MaxCount; i++)
	{
		std::vector<ComponentFunctionData>* temp = nullptr;
		//� �Լ������� ����Ʈ�� ���������� üũ
		switch (i)
		{
		case 0:
			temp = &FunctionList00;
			break;
		case 1:
			temp = &FunctionList01;
			break;
		case 2:
			temp = &FunctionList02;
			break;
		case 3:
			temp = &FunctionList03;
			break;
		case 4:
			temp = &FunctionList04;
			break;
		}

		int Index = 0;
		while (true)
		{
			//����Ʈ�� ���� �ٵ��Ҵٸ�
			if (Index == (int)(temp->size()))
			{
				break;
			}

			//�Լ������Ͱ��������
			if ((*temp)[Index].FunctionPointer == nullptr)
			{
				Index++;
				continue;
			}
			//Ȱ��ȭ������ �ƴѰ��
			if (*((*temp)[Index].Enabled) == false)
			{
				Index++;
				continue;
			}

			//�Լ� ����
			(*temp)[Index].FunctionPointer();
			Index++;
		}
	}
}
