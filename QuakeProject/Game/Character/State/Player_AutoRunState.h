/**
 * @file Player_AutoRunState.h
 * @brief プレイヤー自動ランステート
 * @author suzuki.shunsuke
 * @date 2022/02/12
 */
#pragma once
#include "CharacterStateBase.h"

class CharacterActor;
class MyCamera;
class Player;

/**
* @brief プレイヤー自動ランステート
*/
class Player_AutoRunState :public CharacterStateBase {
private:
	

public:
	/**
	* @brief コンストラクタ
	*/
	Player_AutoRunState(Player* pOwner);

	/**
	* @brief デストラクタ
	*/
	~Player_AutoRunState() = default;

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