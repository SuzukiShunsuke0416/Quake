/**
 * @file AutoDriveManager.cpp
 * @brief デバッグ用。戦車の自動走行支援マネージャクラス
 * @author suzuki.shunsuke
 * @date 2022/03/23
 */
#include "pch.h"
#include "AutoDriveManager.h"

// どちらか１つまで
#define AUTODRIVEMODE_CREATE
//#define AUTODRIVEMODE_LOAD

 //=====================================================
 //		コンストラクタ
 //=====================================================
AutoDriveManager::AutoDriveManager() 
	:mpCameraUser()
{
// ２つとも定義していないかチェック
#ifdef AUTODRIVEMODE_CREATE
#ifdef AUTODRIVEMODE_LOAD
	static_assert(0, "Double definition!");
#endif // AUTODRIVEMODE_LOAD
#endif // AUTODRIVEMODE_CREATE


#ifdef AUTODRIVEMODE_CREATE
	/*------- 生成モード -------*/

	// カメラユーザの生成
	mpCameraUser = std::make_unique<SatelliteCameraUser>();
#endif // AUTODRIVEMODE_CREATE
#ifdef AUTODRIVEMODE_LOAD
	/*------- 読み込みモード -------*/

	// カメラユーザの生成
	mpCameraUser = std::make_unique<SatelliteCameraUser>();

#endif // AUTODRIVEMODE_LOAD

}

//=====================================================
//		初期化
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
//		更新
//=====================================================
void AutoDriveManager::Update()
{

}

//=====================================================
//		描画
//=====================================================
void AutoDriveManager::Render()
{

}
