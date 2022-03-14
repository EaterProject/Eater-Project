#include "Delegate.h"

Delegate_Map::Delegate_Map()
{
}

Delegate_Map::~Delegate_Map()
{
}

void Delegate_Map::Push(ComponentFunctionData temp)
{
	//순서에맞게 함수포인터 리스트에 넣어준다
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
			//원하는 함수를 찾았다면
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
		//어떤 함수포인터 리스트를 돌릴것인지 체크
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
			//리스트를 전부 다돌았다면
			if (Index == (int)(temp->size()))
			{
				break;
			}

			//함수포인터가없을경우
			if ((*temp)[Index].FunctionPointer == nullptr)
			{
				Index++;
				continue;
			}
			//활성화조건이 아닌경우
			if (*((*temp)[Index].Enabled) == false)
			{
				Index++;
				continue;
			}

			//함수 실행
			(*temp)[Index].FunctionPointer();
			Index++;
		}

		//한번만 실행되는 함수포인터라면 모두 실행후 삭제
		(*temp).clear();
	}
}

void Delegate_Map::Play()
{
	for (int i = 0; i < MaxCount; i++)
	{
		std::vector<ComponentFunctionData>* temp = nullptr;
		//어떤 함수포인터 리스트를 돌릴것인지 체크
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
			//리스트를 전부 다돌았다면
			if (Index == (int)(temp->size()))
			{
				break;
			}

			//함수포인터가없을경우
			if ((*temp)[Index].FunctionPointer == nullptr)
			{
				Index++;
				continue;
			}
			//활성화조건이 아닌경우
			if (*((*temp)[Index].Enabled) == false)
			{
				Index++;
				continue;
			}

			//함수 실행
			(*temp)[Index].FunctionPointer();
			Index++;
		}
	}
}
