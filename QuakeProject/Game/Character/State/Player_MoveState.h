/**
 * @file Player_MoveState.h
 * @brief �v���C���[�ړ��X�e�[�g
 * @author suzuki.shunsuke
 * @date 2022/02/12
 */
#pragma once
#include "CharacterStateBase.h"

class CharacterActor;
class MyCamera;
class Player;

/**
* @brief �v���C���[�ړ��X�e�[�g
*/
class Player_MoveState :public CharacterStateBase {
private:
	/** �I�[�i�[�������Ă���J���� */
	MyCamera* mpCamera;

	/** �ړ������i�p�x�j */
	float mMoveWayAngle;

	DirectX::SimpleMath::Vector3 mFrontWay;

public:
	/**
	* @brief �R���X�g���N�^
	*/
	Player_MoveState(Player* pOwner);

	/**
	* @brief �f�X�g���N�^
	*/
	~Player_MoveState() = default;

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