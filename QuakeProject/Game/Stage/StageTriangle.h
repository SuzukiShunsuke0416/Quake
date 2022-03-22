/**
 * @file	StageTriangle.h
 * @brief	オーナーの足元にある地面情報（三角）
			をまとめたコンポーネント
 * @author	suzuki.shunsuke
 * @date	2022/02/02
 */
#pragma once

#include "Game/Component/Component.h"
#include "Libraries/MyLibraries/Utility.h"

class StagePoint;

/**
* @brief オーナーの足元にある地面情報（三角）
*/
class StageTriangle :public Component {
public:
	/** 管理するポイントの数 */
	static const int POINT_SIZE = 3;
private:
	/** オーナーの足元にあるステージポイント */
	StagePoint* mpStagePoint[POINT_SIZE];

	/** 面の法線ベクトル */
	DirectX::SimpleMath::Vector3 mNormal;

	/** 法線方向への回転行列 */
	DirectX::SimpleMath::Matrix mNormalRotationMatrix;

	/** 方位角 */
	float mAzimuth;

	/** 仰角 */
	float mElevationAngle;
public:
	/**
	 * @brief	初期化
	 * @param[in] pOwner 持ち主
	 */
	StageTriangle(Actor* pOwner);

	/**
	* @brief デストラクタ
	*/
	~StageTriangle();

	/**
	* @brief 更新
	* @note (StageManagerから呼ばれる)
	*/
	void Update();
public:

	/**
	* @brief 足元ステージポイント配列の先頭アドレスを取得
	*/
	inline StagePoint** GetStagePoints() {
		return mpStagePoint;
	}

	/**
	* @brief 足元ステージポイントを取得
	*/
	inline StagePoint* GetStagePoints(int element) {
		return mpStagePoint[element];
	}

	/**
	* @brief 法線ベクトル取得
	*/
	inline const DirectX::SimpleMath::Vector3& GetNormal() {
		return mNormal;
	}

	/**
	* @brief 法線方向の回転行列を取得
	*/
	inline const DirectX::SimpleMath::Matrix& GetNormalRotMat() {
		return mNormalRotationMatrix;
	}

	/**
	* @brief 方位角を取得
	*/
	inline float GetAzimuth() {
		return mAzimuth;
	}

	/**
	* @brief 仰角を取得
	*/
	inline float GetElevationAngle() {
		return mElevationAngle;
	}

};