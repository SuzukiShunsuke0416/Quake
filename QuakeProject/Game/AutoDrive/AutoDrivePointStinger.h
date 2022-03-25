/**
 * @file AutoDrivePointStinger.h
 * @brief 自動走行ポイントを地面に刺す担当
 * @author suzuki.shunsuke
 * @date 2022/03/25
 */
#pragma once
#include "AutoDrivePointObject.h"

class SatelliteCamera;
class AutoDrivePointObject;

 /**
  * @brief 自動走行ポイントを地面に刺す担当
  */
class AutoDrivePointStinger {
private:

	/** サテライトカメラ */
	SatelliteCamera* mpSatelliteCamera;

	/** 現在選択中のポイント番号 */
	int mChoosingEleNum;

	/** 現在選択中のポイント */
	AutoDrivePointObject* mpChoosingPoint;

	/** 生成するポイントのタイプ */
	AutoDrivePointObject::Type mSpawnPointType;
public:

	/**
	* @brief コンストラクタ
	*/
	AutoDrivePointStinger(SatelliteCamera* pSateCam);

	/**
	* @brief デストラクタ
	*/
	~AutoDrivePointStinger() = default;

	/**
	* @brief 初期化
	*/
	void Initialize();

	/**
	* @brief 更新
	*/
	void Update();

private:

	/**
	* @brief 新しいポイントを生成する
	*/
	void CreatePoint();

	/**
	* @brief 選択中のポイントを変更する
	*/
	void ChangeChoosingPoint();

	/**
	* @brief 生成タイプを変更する
	*/
	void ChangeSpawnType();
};