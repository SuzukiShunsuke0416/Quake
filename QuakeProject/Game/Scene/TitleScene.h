/**
 * @file TitleScene.h
 * @brief タイトルシーン
 * @author suzuki.shunsuke
 * @date 2022/01/29
 */
#pragma once
#include "IScene.h"
#include "SceneChangeFader.h"

 /**
 * @brief タイトルシーン
 */
class TitleScene :public IScene {
private:
	/** シーン切り替え担当 */
	SceneChangeFader mSceneChangeFader;
public:
	/**
	* @brief コンストラクタ
	*/
	TitleScene();

	/**
	* @brief デストラクタ
	*/
	~TitleScene();

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