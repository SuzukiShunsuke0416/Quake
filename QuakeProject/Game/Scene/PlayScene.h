/**
 * @file PlayScene.h
 * @brief プレイシーン
 * @author suzuki.shunsuke
 * @date 2022/01/29
 */
#pragma once
#include "IScene.h"
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include "SceneChangeFader.h"
#include "Game/Actor/DebugActor.h"

class StageManager;
class CameraManager;
class ElevationColorManager;
class CharacterManager;
class CollisionManager;
class AutoDriveManager;

/**
* @brief プレイシーン
*/
class PlayScene :public IScene {
private:
	/** シーン切り替え担当 */
	SceneChangeFader mSceneChangeFader;

	/** ステージ */
	StageManager* mpStageManager;

	/** カメラマネージャ */
	CameraManager* mpCameraManager;

	/** 色マネージャ */
	ElevationColorManager* mpColorManager;

	/** キャラクターマネージャ */
	CharacterManager* mpCharacterManager;

	/** 当たり判定マネージャ */
	CollisionManager* mpCollisionManager;

	// デバッグ用
	DebugActor mDebugActor;

	/** 自動走行マネージャ */
	AutoDriveManager* mpAutoDriveManager;
public:
	/**
	 * コンストラクタ
	 */
	PlayScene();

	/**
	 * デストラクタ
	 */
	~PlayScene();

	/**
	 * @brief 初期化関数
	 */
	void Initialize() override;

	/**
	 * @brief 更新関数
	 */
	void Update() override;

	/**
	 * @brief 描画関数
	 */
	void Render() override;

	/**
	 * @brief 終了関数
	 */
	void Finalize() override;

	/**
	 * @brief フェード開始通知
	 */
	void OnFadeStart() override;

	/**
	 * @brief フェード終了通知
	 */
	void OnFadeEnd() override;

	/**
	 * @brief フェード更新関数
	 * @return	フェードが終わったらtrue
	 */
	bool FadeUpdate() override;

	/**
	 * @fn
	 * @brief なし
	 * @param[in] srv マスク用シェーダリソースビュー
	 */
	void FadeRender(ID3D11ShaderResourceView* srv) override;
};