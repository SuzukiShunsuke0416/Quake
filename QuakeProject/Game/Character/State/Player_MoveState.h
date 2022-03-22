/**
 * @file Player_MoveState.h
 * @brief プレイヤー移動ステート
 * @author suzuki.shunsuke
 * @date 2022/02/12
 */
#pragma once
#include "CharacterStateBase.h"

class CharacterActor;
class MyCamera;
class Player;

/**
* @brief プレイヤー移動ステート
*/
class Player_MoveState :public CharacterStateBase {
private:
	/** オーナーが持っているカメラ */
	MyCamera* mpCamera;

	/** 移動方向（角度） */
	float mMoveWayAngle;

	DirectX::SimpleMath::Vector3 mFrontWay;

public:
	/**
	* @brief コンストラクタ
	*/
	Player_MoveState(Player* pOwner);

	/**
	* @brief デストラクタ
	*/
	~Player_MoveState() = default;

	/**
	* @brief リセット
	*/
	void Reset() override;

	/**
	* @brief 更新
	*/
	void Update() override;

	/**
	* @brief 終了処理
	*/
	void Finalize() override;
};