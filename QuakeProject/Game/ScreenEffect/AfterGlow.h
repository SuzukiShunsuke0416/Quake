/**
 * @file AfterGlow.h
 * @brief 他のクラスの描画処理に残光を付与するクラス
 * @author suzuki.shunsuke
 * @date 2022/03/13
 */
#pragma once
#include <functional>
#include "BrightChanger.h"
#include <Model.h>
#include <deque>

 /**
 * @brief 残光を付与するクラス
 */
class AfterGlow{
private:

	/** 発光用クラス */
	std::unique_ptr<BrightChanger> mpBright;

	/** 残光オブジェの縮小割合 */
	float mReductionRat;

	/** 残光オブジェの描画数 */
	int mGlowSize;

	/** モデル行列のログ */
	std::deque<DirectX::SimpleMath::Matrix> mModelMatrixLogs;

	/** モデル行列ログ変更関数のptr */
	std::function<void(AfterGlow*, const DirectX::SimpleMath::Matrix&)>
		mUpdateLogFunction;
public:

	/**
	* @brief コンストラクタ
	* @param[in] reductionRat 残光オブジェたちの縮小割合
	* @param[in] glowSize 残光オブジェの数
	*/
	AfterGlow(float reductionRat = 0.9f, int glowSize = 32);

	/**
	* @brief デストラクタ
	*/
	~AfterGlow() = default;

	/**
	* @brief 残光を適応させる描画処理
	* @param[in] pGeoPri ｼﾞｵﾒﾄﾘｯｸﾌﾟﾘﾐﾃｨﾌﾞのポインタ
	* @param[in] ModelMat モデル行列
	* @param[in] color ｼﾞｵﾒﾄﾘｯｸﾌﾟﾘﾐﾃｨﾌﾞの色
	*/
	void Adaptation(
		DirectX::GeometricPrimitive* pGeoPri,
		DirectX::SimpleMath::Matrix ModelMat,
		DirectX::XMVECTORF32 color
	);

	/**
	* @brief 残光を適応させる描画処理
	* @param[in] pModel モデルのポインタ
	* @param[in] pCommonStates コモンステートのポインタ
	* @param[in] ModelMat モデル行列
	*/
	void Adaptation(
		DirectX::Model* pModel,
		DirectX::CommonStates* pCommonStates,
		DirectX::SimpleMath::Matrix ModelMat
	);

private:
	/**
	* @brief モデル行列のログを追加する
	* @param[in] ModelMat モデル行列
	*/
	void PushModelMat(const DirectX::SimpleMath::Matrix& modelMat);

	/**
	* @brief モデル行列のログを追加し、いらない分は削除する
	* @param[in] ModelMat モデル行列
	*/
	void PushPopModelMat(const DirectX::SimpleMath::Matrix& modelMat);
};