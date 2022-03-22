/**
 * @file ElevationColor.cpp
 * @brief	�I�[�i�[�̍������W�ɉ����Ēl���ω�����
 * @brief	�F���������R���|�[�l���g
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#include "pch.h"
#include "ElevationColor.h"
#include "ElevationColorManager.h"

 //=====================================================
 //		�R���X�g���N�^
 //=====================================================
ElevationColor::ElevationColor(Actor* pOwner)
	:Component(pOwner)
	, mColor()
{
	// �ǉ�
	ElevationColorManager::GetInstance()->AddMember(this);
}

//=====================================================
//		�f�X�g���N�^
//=====================================================
ElevationColor::~ElevationColor()
{
	// ���O
	ElevationColorManager::GetInstance()->RemoveMember(this);
}
