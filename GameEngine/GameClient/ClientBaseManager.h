#pragma once
//Ŭ���̾�Ʈ���� ����ϴ� �Ŵ������� ����� 
//�Լ����� ������� Ŭ����
class ObjectFactory;

class ClientBaseManager
{
public:
	ClientBaseManager() = default;
	virtual ~ClientBaseManager() = default;

	virtual void Initialize() = 0;
	void SetFactory(ObjectFactory* FactoryGM);
protected:
	ObjectFactory* mFactory = nullptr; //���� ������ ���丮
};