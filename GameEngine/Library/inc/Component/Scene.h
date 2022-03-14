#pragma once
#include <string>

/// <summary>
/// 생성은 엔진쪽에서 생성할때 이름과 함수포인터들만 넘겨주고
///  생성과삭제는 스크린매니저에서 해준다
/// </summary>

#include "EaterEngineDLL.h"
class Scene
{
public:
	EATER_ENGINEDLL Scene();
	EATER_ENGINEDLL virtual ~Scene();

	//씬이 생성될떄 호출
	virtual void Awake()=0;
	virtual void Update()=0;
	virtual void End()=0;

	EATER_ENGINEDLL virtual void ThreadFunction();
	EATER_ENGINEDLL virtual void Run();

	std::string Name;
	bool ThreadRun;
private:
};


