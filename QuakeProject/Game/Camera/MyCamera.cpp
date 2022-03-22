/**
 * @file MyCamera.cpp
 * @brief ��{�I�ȃJ�����N���X�B�p�r�ɍ��킹�Čp���B
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#include "pch.h"
#include "MyCamera.h"
#include "CameraManager.h"

 //=====================================================
 //		�R���X�g���N�^
 //=====================================================
MyCamera::MyCamera(Actor* pOwner)
	: Camera()
	, Component(pOwner)
{
	//�}�l�[�W���ɒǉ�
	CameraManager::GetInstance()->AddMember(this);
}

//=====================================================
//		�f�X�g���N�^
//=====================================================
MyCamera::~MyCamera()
{
	//�}�l�[�W�����珜�O
	CameraManager::GetInstance()->RemoveMember(this);
}
