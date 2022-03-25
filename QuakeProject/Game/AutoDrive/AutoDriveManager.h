/**
 * @file AutoDriveManager.h
 * @brief デバッグ用。戦車の自動走行支援マネージャクラス
 * @author suzuki.shunsuke
 * @date 2022/03/23
 */
#pragma once
#include "Game/UtilityManager/SingletonFounder.h"
#include "Game/Camera/SatelliteCamera.h"
#include "AutoDrivePointStinger.h"
#include "AutoDrivePointObject.h"
#include <list>

class CharacterActor;

 /**
 * @brief 戦車の自動走行支援マネージャクラス
 */
class AutoDriveManager :public SingletonFounder<AutoDriveManager> {
	friend SingletonFounder;

private:
	/** サテライトカメラを持つアクター */
	std::unique_ptr<SatelliteCameraUser> mpCameraUser;

	/** ポイントを置く担当 */
	std::unique_ptr<AutoDrivePointStinger> mpPointSinger;

	/** ステージポイントたち */
	std::list<AutoDrivePointObject*> mAllPoints;

	/** 現在更新中のポイントのイテレータ― */
	std::list<AutoDrivePointObject*>::iterator mCurrentActivePointObjIte;

	/** ポイントの更新を行うか */
	bool mPointUpdateFlag;

	/** 現在自動運転中のキャラクター */
	CharacterActor* mpDriveCharacter;
private:
	/**
	* @brief コンストラクタ
	*/
	AutoDriveManager();

public:

	/**
	* @brief デストラクタ
	*/
	~AutoDriveManager();

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

public:
	/**
	* @brief ポイントを１つずつ順番に更新する
	*/
	void UpdateObjPoint();


	/**
	* @brief ポイントを追加する
	* @note newしたものを引数に入れる。
	*/
	void AddPoint(AutoDrivePointObject* pPoint);

public:
	/**
	 * @brief 現在自動運転中のキャラクターを取得
	 */
	inline CharacterActor* GetDriveCharacter() {
		return mpDriveCharacter;
	}

	/**
	* @brief ポイントを取得する
	* @param[in] ele	要素番号
	*/
	AutoDrivePointObject* GetPoint(int ele);

	/**
	 * @brief 現在登録されているポイントの数を返す
	 */
	inline int GetPointSize() { return mAllPoints.size(); }
};