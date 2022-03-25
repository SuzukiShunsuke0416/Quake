/**
 * @file AutoDriveManager.h
 * @brief �f�o�b�O�p�B��Ԃ̎������s�x���}�l�[�W���N���X
 * @author suzuki.shunsuke
 * @date 2022/03/23
 */
#pragma once
#include "Game/UtilityManager/SingletonFounder.h"
#include "Game/Camera/SatelliteCamera.h"
#include "AutoDrivePointStinger.h"
#include "AutoDrivePointObject.h"
#include <list>

class CharacterActor;

 /**
 * @brief ��Ԃ̎������s�x���}�l�[�W���N���X
 */
class AutoDriveManager :public SingletonFounder<AutoDriveManager> {
	friend SingletonFounder;

private:
	/** �T�e���C�g�J���������A�N�^�[ */
	std::unique_ptr<SatelliteCameraUser> mpCameraUser;

	/** �|�C���g��u���S�� */
	std::unique_ptr<AutoDrivePointStinger> mpPointSinger;

	/** �X�e�[�W�|�C���g���� */
	std::list<AutoDrivePointObject*> mAllPoints;

	/** ���ݍX�V���̃|�C���g�̃C�e���[�^�\ */
	std::list<AutoDrivePointObject*>::iterator mCurrentActivePointObjIte;

	/** �|�C���g�̍X�V���s���� */
	bool mPointUpdateFlag;

	/** ���ݎ����^�]���̃L�����N�^�[ */
	CharacterActor* mpDriveCharacter;
private:
	/**
	* @brief �R���X�g���N�^
	*/
	AutoDriveManager();

public:

	/**
	* @brief �f�X�g���N�^
	*/
	~AutoDriveManager();

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

public:
	/**
	* @brief �|�C���g���P�����ԂɍX�V����
	*/
	void UpdateObjPoint();


	/**
	* @brief �|�C���g��ǉ�����
	* @note new�������̂������ɓ����B
	*/
	void AddPoint(AutoDrivePointObject* pPoint);

public:
	/**
	 * @brief ���ݎ����^�]���̃L�����N�^�[���擾
	 */
	inline CharacterActor* GetDriveCharacter() {
		return mpDriveCharacter;
	}

	/**
	* @brief �|�C���g���擾����
	* @param[in] ele	�v�f�ԍ�
	*/
	AutoDrivePointObject* GetPoint(int ele);

	/**
	 * @brief ���ݓo�^����Ă���|�C���g�̐���Ԃ�
	 */
	inline int GetPointSize() { return mAllPoints.size(); }
};