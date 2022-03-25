/**
 * @file AutoDriveManager.cpp
 * @brief �f�o�b�O�p�B��Ԃ̎������s�x���}�l�[�W���N���X
 * @author suzuki.shunsuke
 * @date 2022/03/23
 */
#include "pch.h"
#include "AutoDriveManager.h"
#include "Game/UtilityManager/InputManager.h"
#include "Game/Character/CharacterManager.h"
#include "Game/Character/CharacterActor.h"
#include "Game/Character/Player/Player.h"

using namespace DirectX;

//=====================================================
//	�I�[�g�h���C�u���[�h�̐ݒ�
//=====================================================

// �ǂ��炩�ɐ؂�ւ���or�Q�Ƃ�����
#define AUTODRIVEMODE_CREATE
//#define AUTODRIVEMODE_LOAD




#ifdef AUTODRIVEMODE_CREATE

#define IS_AUTODRIVEMODE

#endif // AUTODRIVEMODE_CREATE
#ifdef AUTODRIVEMODE_LOAD

#define IS_AUTODRIVEMODE

#endif // AUTODRIVEMODE_LOAD


 //=====================================================
 //		�R���X�g���N�^
 //=====================================================
AutoDriveManager::AutoDriveManager() 
	:mpCameraUser()
	,mpPointSinger()
	,mAllPoints()
	,mCurrentActivePointObjIte()
	,mPointUpdateFlag(false)
	,mpDriveCharacter(nullptr)
{
// �Q�Ƃ���`���Ă��Ȃ����`�F�b�N
#ifdef AUTODRIVEMODE_CREATE
#ifdef AUTODRIVEMODE_LOAD
	static_assert(0, "Double definition!");
#endif // AUTODRIVEMODE_LOAD
#endif // AUTODRIVEMODE_CREATE


#ifdef IS_AUTODRIVEMODE

	// �J�������[�U�̐���
	mpCameraUser = std::make_unique<SatelliteCameraUser>();

#endif // IS_AUTODRIVEMODE

#ifdef AUTODRIVEMODE_CREATE

	// �|�C���g�ݒu�S���̐���
	mpPointSinger = std::make_unique<AutoDrivePointStinger>
		(mpCameraUser->GetCamera());

#endif // AUTODRIVEMODE_CREATE
}

//=====================================================
//		�f�X�g���N�^
//=====================================================
AutoDriveManager::~AutoDriveManager()
{
	for (auto ite = mAllPoints.begin(); ite != mAllPoints.end();) {
		delete (*ite);
		ite = mAllPoints.erase(ite);
	}
}

//=====================================================
//		������
//=====================================================
void AutoDriveManager::Initialize()
{
#ifdef IS_AUTODRIVEMODE
	mpCameraUser->Initialize();

	// �����^�]�Ώۂ�ݒ肷��
	Player* player = CharacterManager::GetInstance()->GetAllPlayers()[0];
	player->ChangeState_AutoRun();
	mpDriveCharacter = player;

#endif // IS_AUTODRIVEMODE

#ifdef AUTODRIVEMODE_CREATE
	mpPointSinger->Initialize();
#endif // AUTODRIVEMODE_CREATE

}

//=====================================================
//		�X�V
//=====================================================
void AutoDriveManager::Update()
{
#ifdef AUTODRIVEMODE_CREATE

	mpPointSinger->Update();

	this->UpdateObjPoint();

#endif // AUTODRIVEMODE_CREATE
}

//=====================================================
//		�`��
//=====================================================
void AutoDriveManager::Render()
{
#ifdef IS_AUTODRIVEMODE

	for (const auto& point : mAllPoints) {
		point->Render();
	}

#endif // IS_AUTODRIVEMODE
}

//=====================================================
//		�|�C���g���P�����ԂɍX�V����
//=====================================================
void AutoDriveManager::UpdateObjPoint()
{
	const auto& keyboard = InputManager::GetInstance()->GetKeyboardTracker();
	if (keyboard.IsKeyReleased(Keyboard::LeftShift))
	{
		mPointUpdateFlag = !mPointUpdateFlag;
	}


	if (mAllPoints.size() == 0 || !mPointUpdateFlag || !mpDriveCharacter)return;

	// �X�V
	bool endFlag = (*mCurrentActivePointObjIte)->Update();

	// �X�V���I������玟�̃|�C���g�ɕύX����
	if (endFlag) {
		++mCurrentActivePointObjIte;
		if (mCurrentActivePointObjIte == mAllPoints.end()) {
			mCurrentActivePointObjIte = mAllPoints.begin();
		}
	}
}

//=====================================================
//		�|�C���g��ǉ�����
//=====================================================
void AutoDriveManager::AddPoint(AutoDrivePointObject* pPoint)
{
	mAllPoints.push_back(pPoint);
	mCurrentActivePointObjIte = mAllPoints.begin();
}

//=====================================================
//		�|�C���g���擾����
//=====================================================
AutoDrivePointObject* AutoDriveManager::GetPoint(int ele)
{
	auto ite = mAllPoints.begin();
	for (int i = 0; i < ele; i++) {
		++ite;
	}
	return *ite;
}
