/**
 * @file Actor.cpp
 * @brief �X�e�[�W�ɔz�u�����I�u�W�F�N�g�̊��N���X
 * @author suzuki.shunsuke
 * @date 2022/02/01
 */
#include "pch.h"
#include "Actor.h"
#include "Game/Component/Component.h"

 //=====================================================
 //		�R���X�g���N�^
 //=====================================================
Actor::Actor()
	: mTransform()
	, mpComponents()
	, mpComponentIDs()
	, mActorID()
{
	mActorID = this->CreateActorID();
}

//=====================================================
//		�f�X�g���N�^
//=====================================================
Actor::~Actor()
{
	// �ǉ����ꂽ�R���|�[�l���g�̍폜
	for (auto& comp : mpComponents) {
		delete comp.second;
		comp.second = nullptr;
	}
	mpComponents.clear();
}

//=====================================================
//		�R���|�[�l���g�̒ǉ�
//=====================================================
int Actor::AddComponent(Component* pComponent)
{
	// �R���|�[�l���g��ǉ�
	mpComponents[pComponent->GetID()] = pComponent;
	mpComponentIDs.push_back(pComponent->GetID());

	// ID�z��������ɂ��Ă���
	std::sort(mpComponentIDs.begin(), mpComponentIDs.end());

	// �R���|�[�l���gID��Ԃ�
	return pComponent->GetID();
}
