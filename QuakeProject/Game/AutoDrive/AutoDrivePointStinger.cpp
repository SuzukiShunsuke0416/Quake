/**
 * @file AutoDrivePointStinger.cpp
 * @brief �������s�|�C���g��n�ʂɎh���S��
 * @author suzuki.shunsuke
 * @date 2022/03/25
 */
#include "pch.h"
#include "AutoDrivePointStinger.h"
#include "Game/Camera/SatelliteCamera.h"
#include "Game/UtilityManager/InputManager.h"
#include "AutoDriveManager.h"
#include "AutoDrivePointObject.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//=====================================================
//		�R���X�g���N�^
//=====================================================
AutoDrivePointStinger::AutoDrivePointStinger(SatelliteCamera* pSateCam)
	:mpSatelliteCamera(pSateCam)
	,mChoosingEleNum(-1)
	,mpChoosingPoint(nullptr)
	,mSpawnPointType(AutoDrivePointObject::Type::Start)
{
}

//=====================================================
//		������
//=====================================================
void AutoDrivePointStinger::Initialize()
{

}

//=====================================================
//		�X�V
//=====================================================
void AutoDrivePointStinger::Update()
{
	this->CreatePoint();

	this->ChangeChoosingPoint();

	if (mpChoosingPoint) {
		mpSatelliteCamera->SetTargetPosRubber(mpChoosingPoint->GetPos());
	}
	else {
		mpSatelliteCamera->SetTargetPosDefaultRubber();
	}

	this->ChangeSpawnType();
}

//=====================================================
//		�V�����|�C���g�𐶐�����
//=====================================================
void AutoDrivePointStinger::CreatePoint()
{
	auto& mouseTracker = InputManager::GetInstance()->GetMouseTracker();

	// �ݒu -------------------------------------------
	if (mouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		Vector3 pos = mpSatelliteCamera->GetChoosingPoint();

		// �V�����|�C���g�𐶐�
		AutoDriveManager::GetInstance()->AddPoint(
			AutoDrivePointObject::CreatePointObj(
				mSpawnPointType,
				pos
			)
		);
	}
}

//=====================================================
//		�I�𒆂̃|�C���g��ύX����
//=====================================================
void AutoDrivePointStinger::ChangeChoosingPoint()
{
	auto& keyTracker = InputManager::GetInstance()->GetKeyboardTracker();
	const int pointSize = AutoDriveManager::GetInstance()->GetPointSize();

	DebugLogManager::GetInstance()->EntryLog(mChoosingEleNum,L"choosingEle");

	if (pointSize == 0)return;// �����ǉ�����Ă��Ȃ�������ȍ~�̏��������Ȃ�

	// �I��v�f�ԍ��ɉ����đI���|�C���g��ύX����
	auto change = [&]() 
	{
		mpChoosingPoint = AutoDriveManager::GetInstance()->GetPoint(mChoosingEleNum);
	};

	// �I�𕨕ύX�@�O -------------------------------------------
	if (keyTracker.IsKeyReleased(Keyboard::Keys::Left)) 
	{
		mChoosingEleNum--;

		if (mChoosingEleNum < 0) { 
			mChoosingEleNum = -1;
			mpChoosingPoint = nullptr; 
		}
		else {
			change();
		}
	}
	// �I�𕨕ύX�@�� -------------------------------------------
	else if (keyTracker.IsKeyReleased(Keyboard::Keys::Right)) 
	{
		mChoosingEleNum++;
		mChoosingEleNum = std::min(mChoosingEleNum, pointSize - 1);
		change();
	}
	// �I�𕨕ύX�@�őO -------------------------------------------
	else if (keyTracker.IsKeyReleased(Keyboard::Keys::Down)) 
	{
		mChoosingEleNum = 0;
		change();
	}
	// �I�𕨕ύX�@�Ō� -------------------------------------------
	else if (keyTracker.IsKeyReleased(Keyboard::Keys::Up)) 
	{
		mChoosingEleNum = pointSize - 1;
		change();
	}
}

//=====================================================
//		�����^�C�v��ύX����
//=====================================================
void AutoDrivePointStinger::ChangeSpawnType()
{
	auto& keyTracker = InputManager::GetInstance()->GetKeyboardTracker();

	if (keyTracker.IsKeyReleased(Keyboard::Keys::D1)) {
		mSpawnPointType = AutoDrivePointObject::Type::Start;
	}
	else if (keyTracker.IsKeyReleased(Keyboard::Keys::D2)) {
		mSpawnPointType = AutoDrivePointObject::Type::Run;
	}
	else if (keyTracker.IsKeyReleased(Keyboard::Keys::D3)) {
		mSpawnPointType = AutoDrivePointObject::Type::Goal;
	}

	DebugLogManager::GetInstance()->EntryLog(unsigned(mSpawnPointType), L"SpawnDobjType");
}
