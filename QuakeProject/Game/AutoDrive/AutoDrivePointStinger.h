/**
 * @file AutoDrivePointStinger.h
 * @brief �������s�|�C���g��n�ʂɎh���S��
 * @author suzuki.shunsuke
 * @date 2022/03/25
 */
#pragma once
#include "AutoDrivePointObject.h"

class SatelliteCamera;
class AutoDrivePointObject;

 /**
  * @brief �������s�|�C���g��n�ʂɎh���S��
  */
class AutoDrivePointStinger {
private:

	/** �T�e���C�g�J���� */
	SatelliteCamera* mpSatelliteCamera;

	/** ���ݑI�𒆂̃|�C���g�ԍ� */
	int mChoosingEleNum;

	/** ���ݑI�𒆂̃|�C���g */
	AutoDrivePointObject* mpChoosingPoint;

	/** ��������|�C���g�̃^�C�v */
	AutoDrivePointObject::Type mSpawnPointType;
public:

	/**
	* @brief �R���X�g���N�^
	*/
	AutoDrivePointStinger(SatelliteCamera* pSateCam);

	/**
	* @brief �f�X�g���N�^
	*/
	~AutoDrivePointStinger() = default;

	/**
	* @brief ������
	*/
	void Initialize();

	/**
	* @brief �X�V
	*/
	void Update();

private:

	/**
	* @brief �V�����|�C���g�𐶐�����
	*/
	void CreatePoint();

	/**
	* @brief �I�𒆂̃|�C���g��ύX����
	*/
	void ChangeChoosingPoint();

	/**
	* @brief �����^�C�v��ύX����
	*/
	void ChangeSpawnType();
};