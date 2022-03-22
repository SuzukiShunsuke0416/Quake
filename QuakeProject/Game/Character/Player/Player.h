/**
 * @file Player.h
 * @brief �v���C���[�N���X
 * @author suzuki.shunsuke
 * @date 2022/02/06
 */
#pragma once

#include "../CharacterActor.h"
#include "../State/CharacterStateBase.h"
#include "../State/Player_MoveState.h"
#include "PlayerVisual.h"

 /**
 * @brief �v���C���[�֘A�̊O���f�[�^�Q
 * @note �p�X�f�[�^��new�Ń������m�ۂ��ĕۑ�����Ă��邽�߁A
 * @note ���N���X�f�X�g���N�^���ɉ������B
 */
struct ExternalPlayerData {
	/** �x�[�X�̃��f���̃p�X */
	wchar_t* baseModelPath;
	/** ��镔���̃��f���̃p�X */
	wchar_t* rideModelPath;
	/** �C���̃��f���̃p�X */
	wchar_t* cannonModelPath;
	/** �S�Ẵ��f���p�X���Ǘ�����z�� */
	wchar_t** allModelPath;

	/**
	* @brief �R���X�g���N�^
	*/
	ExternalPlayerData() 
		:baseModelPath(nullptr)
		,rideModelPath(nullptr)
		,cannonModelPath(nullptr)
		,allModelPath(nullptr)
	{
	}

	~ExternalPlayerData()
	{
		delete[] baseModelPath;
		baseModelPath = nullptr;
		delete[] rideModelPath;
		rideModelPath = nullptr;
		delete[] cannonModelPath;
		cannonModelPath = nullptr;
		delete[] allModelPath;
		allModelPath = nullptr;
	}



};

class StageTriangle;
class Collision;
class ControlCamera;
/**
* @brief �v���C���[
*/
class Player :public CharacterActor {
private:
	/** �����X�e�[�W�|�C���g */
	StageTriangle* mpStageTriangle;

	/** �J���� */
	ControlCamera* mpCamera;

	/** �v���C���[�̌����ڒS�� */
	std::unique_ptr<PlayerVisual> mpVisual;

	/** �v���C���[�֘A�̊O���f�[�^�Q */
	ExternalPlayerData mExternalData;

	/** ���݂̃X�e�[�g */
	CharacterStateBase* mpCurrentState;

	/** �ړ��X�e�[�g */
	std::unique_ptr<Player_MoveState> mpMoveState;
public:
	/**
	* @brief �R���X�g���N�^
	*/
	Player();

	/**
	* @brief �f�X�g���N�^
	*/
	~Player();

	/**
	* @brief �X�V
	*/
	void Update() override;

	/**
	* @brief �`��
	*/
	void Render() override;

public:
	/**
	 * @brief	�����蔻���̏���������֐�
	 * @param[in] pCol	����̃R���W����
	 */
	void Hit(Collision* pCol);

	/**
	* @brief ���n�����u�ԌĂ΂��C�x���g
	*/
	void OnTouchGroundEvent()override;

private:
	/**
	* @brief �O���f�[�^�̓ǂݍ���
	*/
	void LoadExternalData();

private:
	/**
	* @brief ���X�e�[�g���u�ړ��v�ɕύX
	*/
	inline void ChangeState_Move() {
		mpCurrentState->Finalize();
		mpCurrentState = mpMoveState.get();
		mpCurrentState->Reset();
	}

public:
	/**
	* @brief �O���f�[�^�Q�̎擾
	* !!!�K���󂯎�鎞�́uconst�Q�Ɓv�̕ϐ��Ɋi�[���邱��!!!
	*/
	const ExternalPlayerData& GetExternalData() {
		return mExternalData;
	}
};