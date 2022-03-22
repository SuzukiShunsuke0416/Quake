/**
 * @file CharacterStateBase.h
 * @brief �L�����N�^�[�X�e�[�g
 * @author suzuki.shunsuke
 * @date 2022/02/04
 */
#pragma once

class CharacterActor;

/**
* @brief �L�����N�^�[�X�e�[�g�̊��N���X
*/
class CharacterStateBase {
protected:
	/** �X�e�[�g�̎����� */
	CharacterActor* mpOwner;

public:
	/**
	* @brief �R���X�g���N�^
	*/
	CharacterStateBase(CharacterActor* pOwner)
		:mpOwner(pOwner)
	{
	}

	/**
	* @brief �f�X�g���N�^
	*/
	virtual ~CharacterStateBase() = default;

	/**
	* @brief ���Z�b�g
	*/
	virtual void Reset() = 0;

	/**
	* @brief �X�V
	*/
	virtual void Update() = 0;

	/**
	* @brief �I������
	*/
	virtual void Finalize() = 0;
};