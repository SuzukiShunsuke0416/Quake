/**
 * @file AutoDrivePointStinger.cpp
 * @brief 自動走行ポイントを地面に刺す担当
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
//		コンストラクタ
//=====================================================
AutoDrivePointStinger::AutoDrivePointStinger(SatelliteCamera* pSateCam)
	:mpSatelliteCamera(pSateCam)
	,mChoosingEleNum(-1)
	,mpChoosingPoint(nullptr)
	,mSpawnPointType(AutoDrivePointObject::Type::Start)
{
}

//=====================================================
//		初期化
//=====================================================
void AutoDrivePointStinger::Initialize()
{

}

//=====================================================
//		更新
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
//		新しいポイントを生成する
//=====================================================
void AutoDrivePointStinger::CreatePoint()
{
	auto& mouseTracker = InputManager::GetInstance()->GetMouseTracker();

	// 設置 -------------------------------------------
	if (mouseTracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		Vector3 pos = mpSatelliteCamera->GetChoosingPoint();

		// 新しいポイントを生成
		AutoDriveManager::GetInstance()->AddPoint(
			AutoDrivePointObject::CreatePointObj(
				mSpawnPointType,
				pos
			)
		);
	}
}

//=====================================================
//		選択中のポイントを変更する
//=====================================================
void AutoDrivePointStinger::ChangeChoosingPoint()
{
	auto& keyTracker = InputManager::GetInstance()->GetKeyboardTracker();
	const int pointSize = AutoDriveManager::GetInstance()->GetPointSize();

	DebugLogManager::GetInstance()->EntryLog(mChoosingEleNum,L"choosingEle");

	if (pointSize == 0)return;// 何も追加されていなかったら以降の処理をしない

	// 選択要素番号に応じて選択ポイントを変更する
	auto change = [&]() 
	{
		mpChoosingPoint = AutoDriveManager::GetInstance()->GetPoint(mChoosingEleNum);
	};

	// 選択物変更　前 -------------------------------------------
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
	// 選択物変更　後 -------------------------------------------
	else if (keyTracker.IsKeyReleased(Keyboard::Keys::Right)) 
	{
		mChoosingEleNum++;
		mChoosingEleNum = std::min(mChoosingEleNum, pointSize - 1);
		change();
	}
	// 選択物変更　最前 -------------------------------------------
	else if (keyTracker.IsKeyReleased(Keyboard::Keys::Down)) 
	{
		mChoosingEleNum = 0;
		change();
	}
	// 選択物変更　最後 -------------------------------------------
	else if (keyTracker.IsKeyReleased(Keyboard::Keys::Up)) 
	{
		mChoosingEleNum = pointSize - 1;
		change();
	}
}

//=====================================================
//		生成タイプを変更する
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
