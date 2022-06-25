#pragma once

#include "EaterEngineDLL.h"
#include "SimpleMath.h"
#include <DirectXMath.h>
#include <vector>
#include "Component.h"
class Rigidbody;
class Collider;
class Transform :public Component
{
public:
	EATER_ENGINEDLL Transform();
	virtual EATER_ENGINEDLL ~Transform();

	//가장먼저 실행되는 업데이트
	void Start();
	void TransformUpdate();

	//물리충돌된 좌표로 움직일것인지
	bool isRigid;

public:
	//로컬 윗방향 벡터를 가져온다
	EATER_ENGINEDLL DirectX::SimpleMath::Vector3 GetLocalPosition_UP();
	//로컬 오른쪽방향 벡터를 가져온다
	EATER_ENGINEDLL DirectX::SimpleMath::Vector3 GetLocalPosition_Right();
	//로컬 앞방향 벡터를 가져온다
	EATER_ENGINEDLL DirectX::SimpleMath::Vector3 GetLocalPosition_Look();

	EATER_ENGINEDLL void LookAt(GameObject* Target);
	EATER_ENGINEDLL void LookAt_X(GameObject* Target);
	EATER_ENGINEDLL void LookAt_Y(Vector3 Pos);
	EATER_ENGINEDLL void LookAt_Z(GameObject* Target);

	//현재 로컬위치값에 값을 더해줌
	EATER_ENGINEDLL void SetLocalPosition(float X, float Y, float Z);
	EATER_ENGINEDLL void AddLocalPosition(float X, float Y, float Z);

	//현재 위치값을 변경해줌
	EATER_ENGINEDLL void SetPosition_X(float pos);
	EATER_ENGINEDLL void SetPosition_Y(float pos);
	EATER_ENGINEDLL void SetPosition_Z(float pos);
	EATER_ENGINEDLL void SetPosition(float X, float Y, float Z);
	EATER_ENGINEDLL void SetPosition(DirectX::SimpleMath::Vector3& mPos);
	EATER_ENGINEDLL void SetPosition(DirectX::SimpleMath::Vector3&& mPos);
	//현재 위치값에 값을 더해줌
	EATER_ENGINEDLL void AddPosition_X(float pos);
	EATER_ENGINEDLL void AddPosition_Y(float pos);
	EATER_ENGINEDLL void AddPosition_Z(float pos);
	EATER_ENGINEDLL void AddPosition(float X, float Y, float Z);
	EATER_ENGINEDLL void AddPosition(DirectX::SimpleMath::Vector3& mPos);
	EATER_ENGINEDLL void AddPosition(DirectX::SimpleMath::Vector3&& mPos);

	//현재 회전값을 변경해줌
	EATER_ENGINEDLL void SetRotate_X(float rot);
	EATER_ENGINEDLL void SetRotate_Y(float rot);
	EATER_ENGINEDLL void SetRotate_Z(float rot);
	EATER_ENGINEDLL void SetRotate(float X, float Y, float Z);
	EATER_ENGINEDLL void SetRotate(DirectX::SimpleMath::Vector3& mRot);
	EATER_ENGINEDLL void SetRotate(DirectX::SimpleMath::Vector3&& mRot);
	EATER_ENGINEDLL void SetRotate(DirectX::SimpleMath::Quaternion& mRot);
	EATER_ENGINEDLL void SetRotate(DirectX::SimpleMath::Quaternion&& mRot);
	//현재 회전값에 값을 더해줌
	EATER_ENGINEDLL void AddRotate_X(float rot);
	EATER_ENGINEDLL void AddRotate_Y(float rot);
	EATER_ENGINEDLL void AddRotate_Z(float rot);
	EATER_ENGINEDLL void AddRotate(float X, float Y, float Z);
	EATER_ENGINEDLL void AddRotate(DirectX::SimpleMath::Vector3& mRot);
	EATER_ENGINEDLL void AddRotate(DirectX::SimpleMath::Vector3&& mRot);
	EATER_ENGINEDLL void AddRotate(DirectX::SimpleMath::Quaternion& mRot);
	EATER_ENGINEDLL void AddRotate(DirectX::SimpleMath::Quaternion&& mRot);

	//현재 크기값을 변경해줌
	EATER_ENGINEDLL void SetScale_X(float scale);
	EATER_ENGINEDLL void SetScale_Y(float scale);
	EATER_ENGINEDLL void SetScale_Z(float scale);
	EATER_ENGINEDLL void SetScale(float scale);
	EATER_ENGINEDLL void SetScale(float X, float Y, float Z);
	EATER_ENGINEDLL void SetScale(DirectX::SimpleMath::Vector3& mScl);
	EATER_ENGINEDLL void SetScale(DirectX::SimpleMath::Vector3&& mScl);
	//현재 크기값에 값을 더해줌
	EATER_ENGINEDLL void AddScale_X(float scale);
	EATER_ENGINEDLL void AddScale_Y(float scale);
	EATER_ENGINEDLL void AddScale_Z(float scale);
	EATER_ENGINEDLL void AddScale(float scale);
	EATER_ENGINEDLL void AddScale(float X, float Y, float Z);
	EATER_ENGINEDLL void AddScale(DirectX::SimpleMath::Vector3& mScl);
	EATER_ENGINEDLL void AddScale(DirectX::SimpleMath::Vector3&& mScl);

	//현재 행렬값을 변경해줌
	EATER_ENGINEDLL void SetLoadLocal(DirectX::SimpleMath::Matrix& Local);
	EATER_ENGINEDLL void SetLoadLocal(DirectX::SimpleMath::Matrix&& Local);
	EATER_ENGINEDLL void SetLoadWorld(DirectX::SimpleMath::Matrix& World);
	EATER_ENGINEDLL void SetLoadWorld(DirectX::SimpleMath::Matrix&& World);


	//위치,회전,크기 행렬들을 따로 가져온다
	EATER_ENGINEDLL DirectX::SimpleMath::Matrix GetPositionXM();
	EATER_ENGINEDLL DirectX::SimpleMath::Matrix GetRotationXM();
	EATER_ENGINEDLL DirectX::SimpleMath::Matrix GetScaleXM();

	//월드매트릭스를 가져옴
	EATER_ENGINEDLL DirectX::SimpleMath::Matrix* GetWorld();
	EATER_ENGINEDLL DirectX::SimpleMath::Matrix* GetLocal();

	//위치, 회전, 크기 값을 가져옴
	EATER_ENGINEDLL DirectX::SimpleMath::Vector3 GetPosition();
	EATER_ENGINEDLL DirectX::SimpleMath::Vector3 GetRotation();
	EATER_ENGINEDLL DirectX::SimpleMath::Quaternion GetRotation_Q();
	EATER_ENGINEDLL DirectX::SimpleMath::Vector3 GetScale();

	//자식객체 넣기
	EATER_ENGINEDLL void SetChild(Transform* mChild);
	//부모 객체 넣기
	EATER_ENGINEDLL void SetParent(Transform* mParent);

	EATER_ENGINEDLL Transform* GetParent();

	//자식객체 로컬 업데이트
	void Child_Local_Update();

	//Y축 기준으로 보는방향으로 천천히 회전한다
	EATER_ENGINEDLL void Slow_Y_Rotation(Vector3 Dir, float RotationSpeed, bool Z_Front = false);
	EATER_ENGINEDLL float GetDistance(Vector3 Pos);

private:
	//현재 위치 회전 크기값을 가져와 행렬을 구한다
	DirectX::SimpleMath::Matrix CreateXMPos4x4();
	DirectX::SimpleMath::Matrix CreateXMRot4x4();
	DirectX::SimpleMath::Matrix CreateXMRot4x4_Q();
	DirectX::SimpleMath::Matrix CreateXMScl4x4();

	//위치
	DirectX::SimpleMath::Vector3 Position;
	//회전
	DirectX::SimpleMath::Vector3 Rotation;
	//크기
	DirectX::SimpleMath::Vector3 Scale;

	DirectX::SimpleMath::Quaternion Q_Rotation;

	DirectX::SimpleMath::Matrix Load_World; //로드된 월드
	DirectX::SimpleMath::Matrix Load_Local; //로드된 로컬

	///로컬 좌표들 현재위치에서 로컬방향으로 1만큼 움직인 좌표들
	///직접 클라쪽에서 바꿀필요가없기때문에 가져오는것만 가능
	//자기 기준으로 위방향으로 1만큼 움직인 좌표
	DirectX::SimpleMath::Vector3 Local_UP;
	//자기 기준으로 오른쪽 방향으로 1만큼 움직인 좌표
	DirectX::SimpleMath::Vector3 Local_Right;
	//자기 기준으로 앞방향으로 1만큼 움직인 좌표
	DirectX::SimpleMath::Vector3 Local_Look;

	//각각의 행렬들을 따로보관
	DirectX::SimpleMath::Matrix PositionXM;
	DirectX::SimpleMath::Matrix RotationXM;
	DirectX::SimpleMath::Matrix ScaleXM;

	///행렬 업데이트 함수들
	//로컬좌표를 프레임마다 구할것인가 여부
	bool LocalUpdate;

	//업데이트에서 매번 월드행렬, 로컬 갱신
	void UpdateTransform();

	//게임 오브젝트의 위치 회전 크기값을 모두곱한 월드 행렬
	DirectX::SimpleMath::Matrix World_M;
	DirectX::SimpleMath::Matrix Local_M;
private:
	const float ConvertPI = 180 / 3.141592f;

	bool IsUpdate = false;

	///계층 구조에서 부모 객체 자식객체
	Transform* Parent;
	std::vector<Transform*> ChildList;


	//physX
	Rigidbody* mRigidbody;
	Collider* mCollider;
};