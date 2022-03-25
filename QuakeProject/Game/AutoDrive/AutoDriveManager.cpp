/**
 * @file AutoDriveManager.cpp
 * @brief デバッグ用。戦車の自動走行支援マネージャクラス
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
//	オートドライブモードの設定
//=====================================================

// どちらかに切り替えるor２つとも消す
#define AUTODRIVEMODE_CREATE
//#define AUTODRIVEMODE_LOAD




#ifdef AUTODRIVEMODE_CREATE

#define IS_AUTODRIVEMODE

#endif // AUTODRIVEMODE_CREATE
#ifdef AUTODRIVEMODE_LOAD

#define IS_AUTODRIVEMODE

#endif // AUTODRIVEMODE_LOAD


 //=====================================================
 //		コンストラクタ
 //=====================================================
AutoDriveManager::AutoDriveManager() 
	:mpCameraUser()
	,mpPointSinger()
	,mAllPoints()
	,mCurrentActivePointObjIte()
	,mPointUpdateFlag(false)
	,mpDriveCharacter(nullptr)
{
// ２つとも定義していないかチェック
#ifdef AUTODRIVEMODE_CREATE
#ifdef AUTODRIVEMODE_LOAD
	static_assert(0, "Double definition!");
#endif // AUTODRIVEMODE_LOAD
#endif // AUTODRIVEMODE_CREATE


#ifdef IS_AUTODRIVEMODE

	// カメラユーザの生成
	mpCameraUser = std::make_unique<SatelliteCameraUser>();

#endif // IS_AUTODRIVEMODE

#ifdef AUTODRIVEMODE_CREATE

	// ポイント設置担当の生成
	mpPointSinger = std::make_unique<AutoDrivePointStinger>
		(mpCameraUser->GetCamera());

#endif // AUTODRIVEMODE_CREATE
}

//=====================================================
//		デストラクタ
//=====================================================
AutoDriveManager::~AutoDriveManager()
{
	for (auto ite = mAllPoints.begin(); ite != mAllPoints.end();) {
		delete (*ite);
		ite = mAllPoints.erase(ite);
	}
}

//=====================================================
//		初期化
//=====================================================
void AutoDriveManager::Initialize()
{
#ifdef IS_AUTODRIVEMODE
	mpCameraUser->Initialize();

	// 自動運転対象を設定する
	Player* player = CharacterManager::GetInstance()->GetAllPlayers()[0];
	player->ChangeState_AutoRun();
	mpDriveCharacter = player;

#endif // IS_AUTODRIVEMODE

#ifdef AUTODRIVEMODE_CREATE
	mpPointSinger->Initialize();
#endif // AUTODRIVEMODE_CREATE

}

//=====================================================
//		更新
//=====================================================
void AutoDriveManager::Update()
{
#ifdef AUTODRIVEMODE_CREATE

	mpPointSinger->Update();

	this->UpdateObjPoint();

#endif // AUTODRIVEMODE_CREATE
}

//=====================================================
//		描画
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
//		ポイントを１つずつ順番に更新する
//=====================================================
void AutoDriveManager::UpdateObjPoint()
{
	const auto& keyboard = InputManager::GetInstance()->GetKeyboardTracker();
	if (keyboard.IsKeyReleased(Keyboard::LeftShift))
	{
		mPointUpdateFlag = !mPointUpdateFlag;
	}


	if (mAllPoints.size() == 0 || !mPointUpdateFlag || !mpDriveCharacter)return;

	// 更新
	bool endFlag = (*mCurrentActivePointObjIte)->Update();

	// 更新が終わったら次のポイントに変更する
	if (endFlag) {
		++mCurrentActivePointObjIte;
		if (mCurrentActivePointObjIte == mAllPoints.end()) {
			mCurrentActivePointObjIte = mAllPoints.begin();
		}
	}
}

//=====================================================
//		ポイントを追加する
//=====================================================
void AutoDriveManager::AddPoint(AutoDrivePointObject* pPoint)
{
	mAllPoints.push_back(pPoint);
	mCurrentActivePointObjIte = mAllPoints.begin();
}

//=====================================================
//		ポイントを取得する
//=====================================================
AutoDrivePointObject* AutoDriveManager::GetPoint(int ele)
{
	auto ite = mAllPoints.begin();
	for (int i = 0; i < ele; i++) {
		++ite;
	}
	return *ite;
}
