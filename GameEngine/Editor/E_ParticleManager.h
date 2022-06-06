#pragma once
#include "E_interface.h"
class GameObject;
class E_ParticleManager : public E_Interface
{
public:
	E_ParticleManager();
	virtual ~E_ParticleManager();

	virtual void Initialize() override;
	virtual void Release() override;

	virtual void CreateEaterFile(GameObject* Obj) override;  //생성
	virtual void ChangeEaterFile(GameObject* Obj) override;	 //변경
	virtual void LoadData() override;
private:


};

