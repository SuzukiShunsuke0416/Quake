/**
 * @file CharacterManager.h
 * @brief �L�����N�^�[�n���Ǘ�����}�l�[�W���N���X
 * @author suzuki.shunsuke
 * @date 2022/02/04
 */
#pragma once
#include "Game/UtilityManager/SingletonFounder.h"
#include <unordered_map>
#include "CharacterActor.h"

class Player;
 /**
 * @brief �L�����N�^�[�n���Ǘ�����}�l�[�W��
 */
class CharacterManager :public SingletonFounder<CharacterManager> {
	friend SingletonFounder;

private:
	/** �L�����N�^�[�����i�匳�f�[�^�j */
	std::unordered_map<int,std::unique_ptr<CharacterActor>> 
		mpCharacters;

private:
	/**
	* @brief �R���X�g���N�^
	*/
	CharacterManager();

	/**
	* @brief �f�X�g���N�^
	*/
	~CharacterManager();
public:
	/**
	* @brief ������
	*/
	void Initialize();

	/**
	* @brief �X�V
	*/
	void Update();

	/**
	* @brief �`��
	*/
	void Render();

public:
	/**
	 * @brief	�L�����N�^�[�̒ǉ�
	 * @param[in] character	CharacterActor�̃��j�[�N�|�C���^
	 */
	void AddCharacter(std::unique_ptr<CharacterActor> character);

	/**
	* @brief �L�����N�^�[�̎擾
	*/
	inline CharacterActor* GetCharacter(int id) {
		return mpCharacters.at(id).get();
	}
};