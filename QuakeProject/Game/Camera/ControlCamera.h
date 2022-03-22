/**
 * @file ControlCamera.h
 * @brief OwnerActor�𒆐S�ɁA�}�E�X����Ȃǂœ����J�����N���X
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#pragma once
#include "MyCamera.h"
#include "CameraManager.h"
#include "Libraries/MyLibraries/Utility.h"

struct ControlCameraMaker;

/**
* @brief OwnerActor�𒆐S�ɁA�}�E�X����Ȃǂœ����J����
*/
class ControlCamera final :public MyCamera {
	friend ControlCameraMaker;
public:
	/** �����_�̈ʒu�i�I�[�i�[�{�����ォ�j */
	static const float ADD_VIEW_HEIGHT;
	/** �I�[�i�[�Ƃ̋��� */
	static const DirectX::SimpleMath::Vector3 DISTANCE;

private:
	/** �}�E�X�̃J�[�\�����W���O */
	Int2 mMouseCursorPosLog;

	/** �J�����̈ړ��� */
	DirectX::SimpleMath::Vector2 mMoveVol;

	/** �J�����̋������� */
	float mDistanceRat;
private:
	/**
	* @brief �R���X�g���N�^
	* @note (ControlCameraMaker�̂ݐ����\)
	*/
	ControlCamera(Actor* pOwner);

public:

	/**
	* @brief �f�X�g���N�^
	*/
	~ControlCamera();

public:
	/**
	* @brief �X�V
	*/
	void Update() override;

private:

	/**
	* @brief �}�E�X����ɍ��킹�ċ�����ς���
	*/
	void CalculateDistance(DirectX::SimpleMath::Vector3& distance);
};

/**
* @brief ControlCamera�𐶐�����
* @note CameraManager�̂ݎg�p�\
*/
struct ControlCameraMaker {
	friend CameraManager;
	// (CameraManager�̂݃A�N�Z�X�\)
private:
	/**
	 * @brief	�R���g���[���J�����𐶐�
	 * @param[in] pOwner	�I�[�i�[�|�C���^
	 * @return ControlCamera* ���۰ٶ�ׂ̃|�C���^
	 * @note �idelete�͎��l���S���j
	 */
	static ControlCamera* SpawnNewControlCamera(Actor* pOwner) {
		return new ControlCamera(pOwner);
	}

};