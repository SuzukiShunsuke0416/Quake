/**
 * @file AutoDriveManager.h
 * @brief �f�o�b�O�p�B��Ԃ̎������s�x���}�l�[�W���N���X
 * @author suzuki.shunsuke
 * @date 2022/03/23
 */
#pragma once
#include "SingletonFounder.h"
#include "Game/Camera/SatelliteCamera.h"

 /**
 * @brief ��Ԃ̎������s�x���}�l�[�W���N���X
 */
class AutoDriveManager :public SingletonFounder<AutoDriveManager> {
	friend SingletonFounder;

private:
	/** �T�e���C�g�J���������A�N�^�[ */
	std::unique_ptr<SatelliteCameraUser> mpCameraUser;

private:
	/**
	* @brief �R���X�g���N�^
	*/
	AutoDriveManager();

public:

	/**
	* @brief �f�X�g���N�^
	*/
	~AutoDriveManager() = default;

	/**
	 * @brief �������֐�
	 */
	void Initialize();

	/**
	 * @brief �X�V�֐�
	 */
	void Update();

	/**
	 * @brief �`��֐�
	 */
	void Render();

};