/**
 * @file ScreenEffectDatas.h
 * @brief 画面効果処理を行うときに必要なデータ群
 * @author suzuki.shunsuke
 * @date 2022/03/13
 */
#pragma once

/**
 * @brief 画面効果処理を行うときに必要なデータ群
 * @note 生成と同時に変数たちに値が自動セットされる。
 */
struct ScreenEffectDatas {
	/** 2Dテクスチャ */
	Microsoft::WRL::ComPtr<ID3D11Texture2D>
		mTexture2D;
	/** レンダーターゲットビュー */
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>
		mRenderTargetView;
	/** シェーダーリソースビュー */
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
		mShaderResourceView;

	/**
	* @brief コンストラクタ
	*/
	ScreenEffectDatas();


	/**
	* @brief デストラクタ
	*/
	~ScreenEffectDatas() = default;
};