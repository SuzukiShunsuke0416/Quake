/**
 * @file AutoDriveManager.cpp
 * @brief �f�o�b�O�p�B��Ԃ̎������s�x���}�l�[�W���N���X
 * @author suzuki.shunsuke
 * @date 2022/03/23
 */
#include "pch.h"
#include "AutoDriveManager.h"

// �ǂ��炩�P�܂�
#define AUTODRIVEMODE_CREATE
//#define AUTODRIVEMODE_LOAD

 //=====================================================
 //		�R���X�g���N�^
 //=====================================================
AutoDriveManager::AutoDriveManager() 
	:mpCameraUser()
{
// �Q�Ƃ���`���Ă��Ȃ����`�F�b�N
#ifdef AUTODRIVEMODE_CREATE
#ifdef AUTODRIVEMODE_LOAD
	static_assert(0, "Double definition!");
#endif // AUTODRIVEMODE_LOAD
#endif // AUTODRIVEMODE_CREATE


#ifdef AUTODRIVEMODE_CREATE
	/*------- �������[�h -------*/

	// �J�������[�U�̐���
	mpCameraUser = std::make_unique<SatelliteCameraUser>();
#endif // AUTODRIVEMODE_CREATE
#ifdef AUTODRIVEMODE_LOAD
	/*------- �ǂݍ��݃��[�h -------*/

	// �J�������[�U�̐���
	mpCameraUser = std::make_unique<SatelliteCameraUser>();

#endif // AUTODRIVEMODE_LOAD

}

//=====================================================
//		������
//=====================================================
void AutoDriveManager::Initialize()
{
#ifdef AUTODRIVEMODE_CREATE
	mpCameraUser->Initialize();
#endif // AUTODRIVEMODE_CREATE

#ifdef AUTODRIVEMODE_LOAD
	mpCameraUser->Initialize();
#endif // AUTODRIVEMODE_LOAD
}

//=====================================================
//		�X�V
//=====================================================
void AutoDriveManager::Update()
{

}

//=====================================================
//		�`��
//=====================================================
void AutoDriveManager::Render()
{

}
