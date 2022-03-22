/**
 * @file IScene.h
 * @brief 各シーンの元となる抽象クラス
 * @author suzuki.shunsuke
 * @date 2022/01/29
 */
#pragma once

 /**
 * @brief 各シーンの元となる抽象クラス
 */
class IScene {
public:

	/**
	* @brief コンストラクタ
	*/
	IScene() = default;

	/**
	* @brief デストラクタ
	*/
	virtual ~IScene() = default;

	/**
	* @brief 初期化
	*/
	virtual void Initialize() = 0;

	/**
	* @brief 更新
	*/
	virtual void Update() = 0;

	/**
	* @brief 描画
	*/
	virtual void Render() = 0;

	/**
	* @brief 終了処理
	*/
	virtual void Finalize() = 0;

	/**
	 * @brief フェード開始通知
	 */
	virtual void OnFadeStart() = 0;

	/**
	 * @brief フェード終了通知
	 */
	virtual void OnFadeEnd() = 0;

	/**
	 * @brief フェード更新関数
	 * @return フェードが終わったらtrue
	 */
	virtual bool FadeUpdate() = 0;

	/**
	 * @fn
	 * @brief なし
	 * @param[in] srv マスク用シェーダリソースビュー
	 */
	virtual void FadeRender(ID3D11ShaderResourceView* srv) = 0;
};