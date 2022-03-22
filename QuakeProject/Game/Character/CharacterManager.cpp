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
	:mpCharacters()
{
}

//=====================================================
//		�f�X�g���N�^
//=====================================================
CharacterManager::~CharacterManager()
{
	mpCharacters.clear();
}

//=====================================================
//		������
//=====================================================
void CharacterManager::Initialize()
{
	// �v���C���[�̐���
	this->AddCharacter(std::make_unique<Player>());
}

//=====================================================
//		�X�V
//=====================================================
void CharacterManager::Update()
{
	// �L�����N�^�[�����̍X�V
	for (const auto& chara : mpCharacters) {
		chara.second->Update();
	}
}

//=====================================================
//		�`��
//=====================================================
void CharacterManager::Render()
{
	// �L�����N�^�[�����̕`��
	for (const auto& chara : mpCharacters) {
		chara.second->Render();
	}
}

//=====================================================
//		�L�����̒ǉ�
//=====================================================
void CharacterManager::AddCharacter(std::unique_ptr<CharacterActor> character)
{
	int id = character->GetActorID();
	mpCharacters[id] = std::move(character);
}
