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
#include "Game/UtilityManager/MemberManager.h"

class Player;
 /**
 * @brief �L�����N�^�[�n���Ǘ�����}�l�[�W��
 */
class CharacterManager 
	:public SingletonFounder<CharacterManager> 
	,public MemberManager<CharacterActor>
{
	friend SingletonFounder;

private:

	/** �v���C���[�����̔z�� */
	std::vector<Player*> mpPlayers;
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
	* @brief �S�v���C���[�̎擾
	*/
	inline const std::vector<Player*>& GetAllPlayers() {
		return mpPlayers;
	}

};