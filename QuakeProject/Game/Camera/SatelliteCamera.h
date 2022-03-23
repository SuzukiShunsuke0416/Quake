/**
 * @file SatelliteCamera.h
 * @brief 上から見た視点のカメラ。衛星カメラ。
 * @author suzuki.shunsuke
 * @date 2022/03/23
 */
#pragma once
#include "MyCamera.h"
#include "Game/Actor/Actor.h"
#include "Libraries/MyLibraries/Utility.h"

 /**
 * @brief 上から見た視点のカメラ
 */
class SatelliteCamera final:public MyCamera{
private:
	/** 地面との距離 */
	float mDistanceRat;

	/** 注視点 */
	DirectX::SimpleMath::Vector3 mTargetPos;

	/** カメラ座標のスタート地点 */
	DirectX::SimpleMath::Vector3 mEyeOriginPos;
	
	/** マウスホイールの値 */
	Save<int> mWheelValue;

public:
	/**
	* @brief コンストラクタ
	*/
	SatelliteCamera(Actor* pOwner);

	/**
	* @brief デストラクタ
	*/
	~SatelliteCamera() = default;

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
	* @brief ビュー行列計算関数を呼ぶ
	*/
	void CallCalculateViewMatrixFunction();

	/**
	* @brief ターゲット座標を計算する
	*/
	void CalculateTargetPos();

	/**
	* @brief 地面との距離差を計算する
	*/
	void CalculateDistance();
};


/**
* @brief サテライトカメラを持つだけのアクター
*/
class SatelliteCameraUser :public Actor {
private:
	SatelliteCamera* mpCamera;
public:
	/**
	* @brief コンストラクタ
	*/
	SatelliteCameraUser() 
		:Actor()
		,mpCamera()
	{
		mpCamera = new SatelliteCamera(this);
		// 削除はカメラマネージャが担当
	}

	/**
	* @brief デストラクタ
	*/
	~SatelliteCameraUser() = default;

	/**
	* @brief 初期化
	*/
	inline void Initialize() {
		mpCamera->Initialize();
	}

	/**
	* @brief 更新
	*/
	inline void Update() override {
		mpCamera->Update();
	}

	/**
	* @brief 描画
	*/
	void Render() override {}
};