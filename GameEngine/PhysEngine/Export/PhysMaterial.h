#pragma once
#include "PhysEngineDLL.h"


///�ݶ��̴��� ����
/*
 ������ (Restitution)
	�籸���� ���� ���ڸ� �����̴� ���� �������ִ� ���� �浹������ �����̴� ���� ���߰�
	�����ִ� ���� �����̰� �ȴ� �̶� �ۿ�Ǵ°��� ������ 1�ϼ��� �浹�Ͽ����� �ӵ��� �����ȴ�
	0�̶�� �浹 ���ڸ��� ����� �� 1 = ���� ź��  0 = ��ź�� �浹

 ���� ������ (StaticFriction),� ������ (DynamicFriction)
	��ü�� �����̱� ���� �������� �ǹ� ���� ���ڸ� ������ �ִ� �ڵ����� ó�������̱�� ��������
	�����̱� �����ϸ� ū���� �����ʴ´� ��ü�� �����̱� �����ϸ� � �������� ����ȴ� ���� ��쿡
	� �������� ���� �����º��� �۱⶧����

	F(��) = u(�������) * m(����) * g(�߷�)
*/
class PHYS_ENGINEDLL PhysMaterial
{
public:
	PhysMaterial();
	~PhysMaterial();
public:
	float MT_StaticFriction		= 0.5f;
	float MT_DynamicFriction	= 0.5f;
	float MT_Restitution		= 0.6f;
};

