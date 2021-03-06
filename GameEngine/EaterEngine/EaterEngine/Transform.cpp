#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "EngineData.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "TimeManager.h"
#include "MathHelper.h"

Transform::Transform()
{
	Position	= { 0,0,0 };
	Rotation	= { 0,0,0 };
	Scale		= { 1,1,1 };
	World_M		= DirectX::XMMatrixIdentity();

	Local_UP	= { 0,1,0 };
	Local_Right = { 1,0,0 };
	Local_Look	= { 0,0,-1 };


	Load_World = DirectX::XMMatrixIdentity();
	Load_Local = DirectX::XMMatrixIdentity();

	LocalUpdate = false;
	isRigid		= false;

	SetUp_Order = FUNCTION_ORDER_LAST;
	Start_Order = FUNCTION_ORDER_LAST;
}

Transform::~Transform()
{

}

void Transform::Start()
{
	//물리충돌된 좌표를 받는다
	mRigidbody	= gameobject->GetComponent<Rigidbody>();
	mCollider	= gameobject->GetComponent<Collider>();

	if (mRigidbody != nullptr)
	{
		isRigid = true;
	}
	if (mCollider != nullptr)
	{

	}

	// 최초 위치값 설정..
	UpdateTransform();
}

void Transform::TransformUpdate()
{
	//월드 좌표들을 기반으로 월드 행렬을 구한다
	if (IsUpdate)
	{
		UpdateTransform();

		IsUpdate = false;
	}
}

DirectX::SimpleMath::Vector3 Transform::GetLocalPosition_UP()
{
	return Local_UP;
}

DirectX::SimpleMath::Vector3 Transform::GetLocalPosition_Right()
{
	return Local_Right;
}

DirectX::SimpleMath::Vector3 Transform::GetLocalPosition_Look()
{
	return Local_Look;
}

void Transform::LookAt(GameObject* Target)
{
	if (Target == nullptr) { return; }

	//타겟의 위치
	Vector3 Pos = Target->GetTransform()->Position;
	Vector3 N = Pos - Position;
	N.Normalize();

	//좌표계에 맞게 변환
	float Angle_Y = -atan2(N.z, N.x) * 180 / 3.141592f;
	float Angle_X = atan2(N.y, N.z) * 180 / 3.141592f;
	

	//-(Yangle + 90)
	Rotation = { Angle_X,-(Angle_Y + 90),Rotation.z };
}

void Transform::LookAt_Y(Vector3 Pos)
{
	//타겟의 위치
	Vector3 N = Pos - Position;
	N.Normalize();


	//좌표계에 맞게 변환
	float Angle_Y = -atan2(N.z, N.x) * ConvertPI;
	float Angle_X = atan2(N.y, N.z) * ConvertPI;

	//-(Yangle + 90)
	Rotation = {Rotation.x, -(Angle_Y + 90),Rotation.z };

}

void Transform::SetLocalPosition(float X, float Y, float Z)
{
	IsUpdate = true;

	Position =
	{
		(Local_Right.x * X) + (Local_UP.x * Y) + (Local_Look.x * Z),
		(Local_Right.y * X) + (Local_UP.y * Y) + (Local_Look.y * Z),
		(Local_Right.z * X) + (Local_UP.z * Y) + (Local_Look.z * Z)
	};
}

void Transform::AddLocalPosition(float X, float Y, float Z)
{
	IsUpdate = true;

	Position =
	{
		Position.x + (Local_Right.x * X),
		Position.y + (Local_Right.y * X),
		Position.z + (Local_Right.z * X)
	};

	Position =
	{
		Position.x + (Local_UP.x * Y),
		Position.y + (Local_UP.y * Y),
		Position.z + (Local_UP.z * Y)
	};

	Position =
	{
		Position.x + (Local_Look.x * Z),
		Position.y + (Local_Look.y * Z),
		Position.z + (Local_Look.z * Z)
	};
}

void Transform::SetPosition_X(float pos)
{
	IsUpdate = true;

	Position.x = pos;
}

void Transform::SetPosition_Y(float pos)
{
	IsUpdate = true;

	Position.y = pos;
}

void Transform::SetPosition_Z(float pos)
{
	IsUpdate = true;

	Position.z = pos;
}

void Transform::SetPosition(float X, float Y, float Z)
{
	IsUpdate = true;
	
	Position.x = X;
	Position.y = Y;
	Position.z = Z;
}

void Transform::SetPosition(DirectX::SimpleMath::Vector3& mPos)
{
	IsUpdate = true;
	
	Position.x = mPos.x;
	Position.y = mPos.y;
	Position.z = mPos.z;
}

void Transform::SetPosition(DirectX::SimpleMath::Vector3&& mPos)
{
	IsUpdate = true;

	Position.x = mPos.x;
	Position.y = mPos.y;
	Position.z = mPos.z;
}

void Transform::AddPosition_X(float pos)
{
	IsUpdate = true;

	Position.x += pos;
}

void Transform::AddPosition_Y(float pos)
{
	IsUpdate = true;

	Position.y += pos;
}

void Transform::AddPosition_Z(float pos)
{
	IsUpdate = true;

	Position.z += pos;
}

void Transform::AddPosition(float X, float Y, float Z)
{
	IsUpdate = true;
	
	Position.x += X;
	Position.y += Y;
	Position.z += Z;
}

void Transform::AddPosition(DirectX::SimpleMath::Vector3& mPos)
{
	IsUpdate = true;
	
	Position.x += mPos.x;
	Position.y += mPos.y;
	Position.z += mPos.z;
}

void Transform::AddPosition(DirectX::SimpleMath::Vector3&& mPos)
{
	IsUpdate = true;

	Position.x += mPos.x;
	Position.y += mPos.y;
	Position.z += mPos.z;
}

void Transform::SetRotate_X(float rot)
{
	IsUpdate = true;

	Rotation.x = rot;
}

void Transform::SetRotate_Y(float rot)
{
	IsUpdate = true;

	Rotation.y = rot;
}

void Transform::SetRotate_Z(float rot)
{
	IsUpdate = true;

	Rotation.z = rot;
}

void Transform::SetRotate(float X, float Y, float Z)
{
	IsUpdate = true;
	
	Rotation.x = X;
	Rotation.y = Y;
	Rotation.z = Z;
}

void Transform::SetRotate(DirectX::SimpleMath::Vector3& mRot)
{
	IsUpdate = true;
	
	Rotation.x = mRot.x;
	Rotation.y = mRot.y;
	Rotation.z = mRot.z;
}

void Transform::SetRotate(DirectX::SimpleMath::Vector3&& mRot)
{
	IsUpdate = true;

	Rotation.x = mRot.x;
	Rotation.y = mRot.y;
	Rotation.z = mRot.z;
}

void Transform::SetRotate(DirectX::SimpleMath::Quaternion& mRot)
{
	IsUpdate = true;
	
	Q_Rotation = mRot;
}

void Transform::SetRotate(DirectX::SimpleMath::Quaternion&& mRot)
{
	IsUpdate = true;

	Q_Rotation = mRot;
}

void Transform::AddRotate_X(float rot)
{
	IsUpdate = true;

	Rotation.x += rot;
}

void Transform::AddRotate_Y(float rot)
{
	IsUpdate = true;

	Rotation.y += rot;
}

void Transform::AddRotate_Z(float rot)
{
	IsUpdate = true;

	Rotation.z += rot;
}

void Transform::AddRotate(float X, float Y, float Z)
{
	IsUpdate = true;
	
	Rotation.x += X;
	Rotation.y += Y;
	Rotation.z += Z;
}

void Transform::AddRotate(DirectX::SimpleMath::Vector3& mRot)
{
	IsUpdate = true;
	
	Rotation.x += mRot.x;
	Rotation.y += mRot.y;
	Rotation.z += mRot.z;
}

void Transform::AddRotate(DirectX::SimpleMath::Vector3&& mRot)
{
	IsUpdate = true;

	Rotation.x += mRot.x;
	Rotation.y += mRot.y;
	Rotation.z += mRot.z;
}

void Transform::AddRotate(DirectX::SimpleMath::Quaternion& mRot)
{
	IsUpdate = true;
	
	Q_Rotation += mRot;
}

void Transform::AddRotate(DirectX::SimpleMath::Quaternion&& mRot)
{
	IsUpdate = true;

	Q_Rotation += mRot;
}

void Transform::SetScale_X(float scale)
{
	IsUpdate = true;

	Scale.x = scale;
}

void Transform::SetScale_Y(float scale)
{
	IsUpdate = true;

	Scale.y = scale;
}

void Transform::SetScale_Z(float scale)
{
	IsUpdate = true;

	Scale.z = scale;
}

void Transform::SetScale(float X, float Y, float Z)
{
	IsUpdate = true;

	Scale.x = X;
	Scale.y = Y;
	Scale.z = Z;
}

void Transform::SetScale(DirectX::SimpleMath::Vector3& mScl)
{
	IsUpdate = true;
	
	Scale.x = mScl.x;
	Scale.y = mScl.y;
	Scale.z = mScl.z;
}

void Transform::SetScale(float scale)
{
	IsUpdate = true;

	Scale.x = scale;
	Scale.y = scale;
	Scale.z = scale;
}

void Transform::SetScale(float&& X, float&& Y)
{
	IsUpdate = true;

	Scale.x = X;
	Scale.y = Y;
}

void Transform::AddScale_X(float scale)
{
	IsUpdate = true;

	Scale.x += scale;
}

void Transform::AddScale_Y(float scale)
{
	IsUpdate = true;

	Scale.y += scale;
}

void Transform::AddScale_Z(float scale)
{
	IsUpdate = true;

	Scale.z += scale;
}

void Transform::AddScale(float X, float Y, float Z)
{
	IsUpdate = true;

	Scale.x += X;
	Scale.y += Y;
	Scale.z += Z;
}

void Transform::AddScale(DirectX::SimpleMath::Vector3& mScl)
{
	IsUpdate = true;
	
	Scale.x += mScl.x;
	Scale.y += mScl.y;
	Scale.z += mScl.z;
}

void Transform::AddScale(float scale)
{
	IsUpdate = true;

	Scale.x += scale;
	Scale.y += scale;
	Scale.z += scale;
}

void Transform::SetLoadLocal(DirectX::SimpleMath::Matrix& Local)
{
	IsUpdate = true;

	Load_Local = Local;
}

void Transform::SetLoadLocal(DirectX::SimpleMath::Matrix&& Local)
{
	IsUpdate = true;

	Load_Local = Local;
}

void Transform::SetLoadWorld(DirectX::SimpleMath::Matrix& World)
{
	IsUpdate = true;

	Load_World = World;
}

void Transform::SetLoadWorld(DirectX::SimpleMath::Matrix&& World)
{
	IsUpdate = true;

	Load_World = World;
}

DirectX::SimpleMath::Matrix Transform::GetPositionXM()
{
	return PositionXM;
}

DirectX::SimpleMath::Matrix Transform::GetRotationXM()
{
	return RotationXM;
}

DirectX::SimpleMath::Matrix Transform::GetScaleXM()
{
	return ScaleXM;
}

DirectX::SimpleMath::Matrix* Transform::GetWorld()
{
	return &World_M;
}

DirectX::SimpleMath::Matrix* Transform::GetLocal()
{
	return &Local_M;
}

DirectX::SimpleMath::Vector3 Transform::GetPosition()
{
	return Position;
}

DirectX::SimpleMath::Vector3 Transform::GetRotation()
{
	return Rotation;
}

DirectX::SimpleMath::Quaternion Transform::GetRotation_Q()
{
	return Q_Rotation;
}

DirectX::SimpleMath::Vector3 Transform::GetScale()
{
	return Scale;
}

void Transform::SetChild(Transform* mChild)
{
	IsUpdate = true;

	ChildList.push_back(mChild);
}

void Transform::SetParent(Transform* mParent)
{
	IsUpdate = true;

	Parent = mParent;
}

void Transform::DisconnectParent()
{
	if (Parent == nullptr) return;

	IsUpdate = true;

	Parent->DisconnectChild(this->gameobject);

	Parent = nullptr;
}

void Transform::DisconnectChild(GameObject* obj)
{
	IsUpdate = true;

	for (auto& child : ChildList)
	{
		if (child == obj->transform)
		{
			obj->ChoiceParent(nullptr);
			break;
		}
	}
}

Transform* Transform::GetParent()
{
	return Parent;
}

void Transform::Child_Local_Update()
{
	if (Parent == nullptr)
	{
		Load_Local = Load_World;
	}
	else
	{
		//DirectX::XMMATRIX TM2_1 = DirectX::XMMatrixInverse(nullptr, Parent->Load_World);
		Load_Local = Load_World * Parent->Load_Local;
	}

	for (int i = 0; i < ChildList.size(); i++) 
	{
		ChildList[i]->Child_Local_Update();
	}
}

void Transform::Slow_Y_Rotation(Vector3 Dir, float RotationSpeed, bool Z_Front)
{
	//Y축기준으로 현재 방향에서 보는 방향으로 천천히 회전 시킨다
	//도착 지점의 각도를 구한다 이떄 범위는 -180 ~ 180 이다
	Vector3 N = Dir - Position;
	N.Normalize();
	float EndAngle = 0;

	EndAngle = (-atan2(N.z, N.x) * 180.0f / 3.141592f);

	//도착지점 범위를 변경해준다 우리 엔진과 맞추기 위해서 0~ 360
	if (EndAngle <= 0) { EndAngle += 360; }

	//현재 위치의 각도
	float NowAngle;
	if (Z_Front == false)
	{
		NowAngle = Rotation.y;
	}
	else
	{
		NowAngle = Rotation.y + 180;
	}
	//현재 각도에서 도착지점까지 이동하려고 할때 오른쪽회전,왼쪽회전여부를 판별하기위한 Angle
	float DirAngle = NowAngle - (EndAngle + 90.0f);

	//회전 스피드
	float RotationAngleSpeed =  mTimeManager->DeltaTime() * RotationSpeed;

	//나온각도가 음수냐 양수냐 판별
	float RotationDir = 0;
	if (DirAngle > 0)
	{
		RotationDir = 1;
	}
	else if (DirAngle < 0)
	{
		RotationDir = -1;
	}

	//여기서 한번더 판별해준다 360도를 넘어가면 값이 변경되는것을 해결하기위해
	if (fabs(DirAngle) >= 180) { RotationDir *= -1.0f; }


	/*
	* 여기서 -90을 빼주는 이유는 atan2로 얻은 값은 월드 기준 오른쪽이 0도
	* 우리엔진에서는 월드상에서 z방향이 0도이다 이것을 맞춰주기위해 값을 빼준다
	* 이곳때문에 오차범위가계속 생겼다
	*/

	//오차범위를 설정한다 (float형은 == 비교연산자를 쓰면 안되기때문에 범위를 지정해준다)
	//범위는 현재 위치에서 프레임당 이동하는 각도량 +,- 해준값
	float MaxAngle = (EndAngle - 90.0f) + (RotationAngleSpeed + 1.0f);	//멈춰야하는 각도의 최대값
	float MinAngle = (EndAngle - 90.0f) - (RotationAngleSpeed + 1.0f);	//멈춰야하는 각도의 최소값

	if (MinAngle >= MaxAngle)
	{
		int a = 0;
	}

	float MyAngle =  Rotation.y;				//현재 각도값

	//회전 도착지점 을 설정한다
	if (MyAngle <= MaxAngle && MyAngle >= MinAngle)
	{
		RotationDir = 0;
	}

	//위에도 말했지만 0도 의 위치때문에 한번더 검사를 해준다(0 ~ -90 여기 범위 판별을 위해)
	if (EndAngle - 90 <= 0)
	{
		float Max = 360.0f + MaxAngle;
		float Min = 360.0f + MinAngle;
		if (MyAngle <= Max && MyAngle >= Min)
		{
			RotationDir = 0.0f;
		}
	}

	//이렇게 계산된 값으로 각도를 이동
	if (Dir != Vector3(0, 0, 0))
	{
		AddRotate(0, RotationDir * RotationAngleSpeed, 0);
	}

	//현재 각도는 0 ~ 360 범위 밖으로 나가지못하게 변경
	if (Rotation.y >= 360.0f)
	{
		Rotation.y = 0.0f;
	}
	else if (Rotation.y < 0.0f)
	{
		Rotation.y = 360.0f;
	}

}

float Transform::GetDistance(Vector3 Pos)
{
	return DirectX::SimpleMath::Vector3::Distance(Position, Pos);
}

DirectX::SimpleMath::Matrix Transform::CreateXMPos4x4()
{
	DirectX::SimpleMath::Matrix Position_4x4;
	Position_4x4._11 = 1;				Position_4x4._12 = 0;			Position_4x4._13 = 0;			Position_4x4._14 = 0;
	Position_4x4._21 = 0;				Position_4x4._22 = 1;			Position_4x4._23 = 0;			Position_4x4._24 = 0;
	Position_4x4._31 = 0;				Position_4x4._32 = 0;			Position_4x4._33 = 1;			Position_4x4._34 = 0;
	Position_4x4._41 = Position.x;		Position_4x4._42 = Position.y;	Position_4x4._43 = Position.z;	Position_4x4._44 = 1;

	return Position_4x4;
}

DirectX::SimpleMath::Matrix Transform::CreateXMRot4x4()
{
	float radX = Rotation.x * 3.141592f / 180;
	float radY = Rotation.y * 3.141592f / 180;
	float radZ = Rotation.z * 3.141592f / 180;
	DirectX::XMMATRIX _P = DirectX::XMMatrixRotationX(radX);
	DirectX::XMMATRIX _Y = DirectX::XMMatrixRotationY(radY);
	DirectX::XMMATRIX _R = DirectX::XMMatrixRotationZ(radZ);

	DirectX::XMMatrixRotationRollPitchYaw(radX, radY, radZ);

	return _R * _Y * _P;
}

DirectX::SimpleMath::Matrix Transform::CreateXMRot4x4_Q()
{
	return XMMatrixRotationQuaternion(Q_Rotation);
}

DirectX::SimpleMath::Matrix Transform::CreateXMScl4x4()
{
	DirectX::SimpleMath::Matrix Scale_4x4;
	Scale_4x4._11 = Scale.x;	Scale_4x4._12 = 0;			Scale_4x4._13 = 0;			Scale_4x4._14 = 0;
	Scale_4x4._21 = 0;			Scale_4x4._22 = Scale.y;	Scale_4x4._23 = 0;			Scale_4x4._24 = 0;
	Scale_4x4._31 = 0;			Scale_4x4._32 = 0;			Scale_4x4._33 = Scale.z;	Scale_4x4._34 = 0;
	Scale_4x4._41 = 0;			Scale_4x4._42 = 0;			Scale_4x4._43 = 0;			Scale_4x4._44 = 1;

	return Scale_4x4;
}

void Transform::UpdateTransform()
{
	PositionXM	= CreateXMPos4x4();
	ScaleXM		= CreateXMScl4x4();

	RotationXM = CreateXMRot4x4();

	//if (isRigid == false)
	//{
	//	RotationXM	= CreateXMRot4x4();
	//}
	//else
	//{
	//	RotationXM = CreateXMRot4x4_Q();
	//}

	Local_M = (ScaleXM * RotationXM * PositionXM);
	if (Parent != nullptr)
	{
		World_M = Load_Local * Local_M * Parent->World_M;
	}
	else
	{
		World_M = Load_Local * Local_M;
	}

	for (int i = 0; i < ChildList.size(); i++)
	{
		ChildList[i]->UpdateTransform();
		ChildList[i]->IsUpdate = false;
	}

	// Object Matrix 연동..
	gameobject->OneMeshData->Object_Data->Local = Local_M;
	gameobject->OneMeshData->Object_Data->World = World_M;
	gameobject->OneMeshData->Object_Data->InvWorld = MathHelper::InverseTranspose(World_M);

	/// 로컬 업데이트..
	//월드 행렬 구하기
	DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(World_M);

	//로컬좌표를 구하기위해 월드의 역행렬을 구함
	DirectX::SimpleMath::Matrix A_Master4x4 = DirectX::XMMatrixInverse(&det, World_M);

	//각각의 값을 넣어준다
	Local_Right.x = A_Master4x4._11;
	Local_Right.y = A_Master4x4._12;
	Local_Right.z = A_Master4x4._13;

	Local_UP.x = A_Master4x4._21;
	Local_UP.y = A_Master4x4._22;
	Local_UP.z = A_Master4x4._23;

	Local_Look.x = A_Master4x4._31;
	Local_Look.y = A_Master4x4._32;
	Local_Look.z = A_Master4x4._33;
}





