/**
 * @file Component.h
 * @brief �u�A�N�^�ɒǉ����邱�Ƃ��ł���@�\�̍\���v�f�v�̊��N���X
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#include "pch.h"
#include "Component.h"
#include "Game/Actor/Actor.h"

 //=====================================================
 //		�R���X�g���N�^
 //=====================================================
Component::Component(Actor* pOwner)
	:mpOwnerActor(pOwner)
	, mComponentID()
{
	// ID�𐶐�
	CreateID();
	// �A�N�^�[�Ɏ�����ǉ�
	pOwner->AddComponent(this);
}
