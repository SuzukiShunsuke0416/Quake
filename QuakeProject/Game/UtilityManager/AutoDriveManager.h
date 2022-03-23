/**
 * @file AutoDriveManager.h
 * @brief デバッグ用。戦車の自動走行支援マネージャクラス
 * @author suzuki.shunsuke
 * @date 2022/03/23
 */
#pragma once
#include "SingletonFounder.h"
#include "Game/Camera/SatelliteCamera.h"

 /**
 * @brief 戦車の自動走行支援マネージャクラス
 */
class AutoDriveManager :public SingletonFounder<AutoDriveManager> {
	friend SingletonFounder;

private:
	/** サテライトカメラを持つアクター */
	std::unique_ptr<SatelliteCameraUser> mpCameraUser;

private:
	/**
	* @brief コンストラクタ
	*/
	AutoDriveManager();

public:

	/**
	* @brief デストラクタ
	*/
	~AutoDriveManager() = default;

	/**
	 * @brief 初期化関数
	 */
	void Initialize();

	/**
	 * @brief 更新関数
	 */
	void Update();

	/**
	 * @brief 描画関数
	 */
	void Render();

};