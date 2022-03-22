/**
 * @file Player.h
 * @brief プレイヤークラス
 * @author suzuki.shunsuke
 * @date 2022/02/06
 */
#pragma once

#include "../CharacterActor.h"
#include "../State/CharacterStateBase.h"
#include "../State/Player_MoveState.h"
#include "PlayerVisual.h"

 /**
 * @brief プレイヤー関連の外部データ群
 * @note パスデータはnewでメモリ確保して保存されているため、
 * @note 自クラスデストラクタ時に解放する。
 */
struct ExternalPlayerData {
	/** ベースのモデルのパス */
	wchar_t* baseModelPath;
	/** 乗る部分のモデルのパス */
	wchar_t* rideModelPath;
	/** 砲塔のモデルのパス */
	wchar_t* cannonModelPath;
	/** 全てのモデルパスを管理する配列 */
	wchar_t** allModelPath;

	/**
	* @brief コンストラクタ
	*/
	ExternalPlayerData() 
		:baseModelPath(nullptr)
		,rideModelPath(nullptr)
		,cannonModelPath(nullptr)
		,allModelPath(nullptr)
	{
	}

	~ExternalPlayerData()
	{
		delete[] baseModelPath;
		baseModelPath = nullptr;
		delete[] rideModelPath;
		rideModelPath = nullptr;
		delete[] cannonModelPath;
		cannonModelPath = nullptr;
		delete[] allModelPath;
		allModelPath = nullptr;
	}



};

class StageTriangle;
class Collision;
class ControlCamera;
/**
* @brief プレイヤー
*/
class Player :public CharacterActor {
private:
	/** 足元ステージポイント */
	StageTriangle* mpStageTriangle;

	/** カメラ */
	ControlCamera* mpCamera;

	/** プレイヤーの見た目担当 */
	std::unique_ptr<PlayerVisual> mpVisual;

	/** プレイヤー関連の外部データ群 */
	ExternalPlayerData mExternalData;

	/** 現在のステート */
	CharacterStateBase* mpCurrentState;

	/** 移動ステート */
	std::unique_ptr<Player_MoveState> mpMoveState;
public:
	/**
	* @brief コンストラクタ
	*/
	Player();

	/**
	* @brief デストラクタ
	*/
	~Player();

	/**
	* @brief 更新
	*/
	void Update() override;

	/**
	* @brief 描画
	*/
	void Render() override;

public:
	/**
	 * @brief	当たり判定後の処理をする関数
	 * @param[in] pCol	相手のコリジョン
	 */
	void Hit(Collision* pCol);

	/**
	* @brief 着地した瞬間呼ばれるイベント
	*/
	void OnTouchGroundEvent()override;

private:
	/**
	* @brief 外部データの読み込み
	*/
	void LoadExternalData();

private:
	/**
	* @brief 現ステートを「移動」に変更
	*/
	inline void ChangeState_Move() {
		mpCurrentState->Finalize();
		mpCurrentState = mpMoveState.get();
		mpCurrentState->Reset();
	}

public:
	/**
	* @brief 外部データ群の取得
	* !!!必ず受け取る時は「const参照」の変数に格納すること!!!
	*/
	const ExternalPlayerData& GetExternalData() {
		return mExternalData;
	}
};