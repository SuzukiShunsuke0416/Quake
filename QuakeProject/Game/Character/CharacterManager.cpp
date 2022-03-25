/**
 * @file CharacterManager.cpp
 * @brief �L�����N�^�[�n���Ǘ�����}�l�[�W���N���X
 * @author suzuki.shunsuke
 * @date 2022/02/04
 */
#include "pch.h"
#include "CharacterManager.h"
#include "Player/Player.h"

 //=====================================================
 //		�R���X�g���N�^
 //=====================================================
CharacterManager::CharacterManager()
	:mpPlayers()
{
}

//=====================================================
//		�f�X�g���N�^
//=====================================================
CharacterManager::~CharacterManager()
{
	for (auto& chara : mpMembers) {
		delete chara;
		chara = nullptr;
	}
}

//=====================================================
//		������
//=====================================================
void CharacterManager::Initialize()
{
	// �v���C���[�̐���
	mpPlayers.push_back(new Player());

	// �v���C���[�̓o�^
	for (const auto& pla : mpPlayers) {
		this->AddMember(pla);
	}
	
}

//=====================================================
//		�X�V
//=====================================================
void CharacterManager::Update()
{
	// �L�����N�^�[�����̍X�V
	for (const auto& chara : mpMembers) {
		chara->Update();
	}
}

//=====================================================
//		�`��
//=====================================================
void CharacterManager::Render()
{
	// �L�����N�^�[�����̕`��
	for (const auto& chara : mpMembers) {
		chara->Render();
	}
}
