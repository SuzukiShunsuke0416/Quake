/**
 * @file AfterGlow.h
 * @brief 描画効果を扱うクラスの基底クラス
 * @author suzuki.shunsuke
 * @date 2022/03/14
 */
#pragma once
#include "ScreenEffectDatas.h"
#include "Game/Shader/ShaderDatas.h"

 /**
  * @brief 描画効果を扱うクラスの基底クラス
  */
class ScreenEffectBase {
protected:
	/** 画面効果用データ群 */
	std::unique_ptr<ScreenEffectDatas> mpScreenEffectDatas;

	/** レンダーターゲットビュー(デフォルト値一時保存用) */
	ID3D11RenderTargetView* mDefaultRTV;

	/** シェーダ用データ群 */
	ShaderDataPack mShaderDataPack;
public:
	
	/**
	* @brief コンストラクタ
	*/
	ScreenEffectBase();

	/**
	* @brief デストラクタ
	*/
	virtual ~ScreenEffectBase() = default;

	/**
	* @brief 定数バッファ値変更
	*/
	virtual void ChangeConstBuffer(const DirectX::SimpleMath::Vector4& cBufParam) = 0;

	/**
	* @brief 描画効果開始
	*/
	virtual void Begin();

	/**
	* @brief 描画効果終了
	*/
	virtual void End(bool isBlendAdd = false);
	/**
	* @brief 効果本描画
	*/
	void Render();

protected:

	/**
	* @brief 定数バッファ値設定
	*/
	void SetConstBuf(const DirectX::SimpleMath::Vector4& cBufParam) {
		mShaderDataPack.mTimeParam = cBufParam;
	}
};