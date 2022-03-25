/**
 * @file Player_AutoRunState.h
 * @brief �v���C���[���������X�e�[�g
 * @author suzuki.shunsuke
 * @date 2022/02/12
 */
#pragma once
#include "CharacterStateBase.h"

class CharacterActor;
class MyCamera;
class Player;

/**
* @brief �v���C���[���������X�e�[�g
*/
class Player_AutoRunState :public CharacterStateBase {
private:
	

public:
	/**
	* @brief �R���X�g���N�^
	*/
	Player_AutoRunState(Player* pOwner);

	/**
	* @brief �f�X�g���N�^
	*/
	~Player_AutoRunState() = default;

	/**
	* @brief ���Z�b�g
	*/
	void Reset() override;

	/**
	* @brief �X�V
	*/
	void Update() override;

	/**
	* @brief �I������
	*/
	void Finalize() override;
};