#pragma once
//클라이언트에서 사용하는 매니저들의 공통된 
//함수들을 묶어놓는 클래스
class ObjectFactory;

class ClientBaseManager
{
public:
	ClientBaseManager() = default;
	virtual ~ClientBaseManager() = default;

	virtual void Initialize() = 0;
	void SetFactory(ObjectFactory* FactoryGM);
protected:
	ObjectFactory* mFactory = nullptr; //몬스터 생성용 펙토리
};