/**
 * @file BrightChanger.h
 * @brief 他のクラスの描画処理を光らせる用クラス
 * @author suzuki.shunsuke
 * @date 2022/03/13
 */
#pragma once
#include "ScreenEffectBase.h"
#include <vector>

 /**
 * @brief 他のクラスの描画処理を光らせる用クラス
 */
class BrightChanger:public ScreenEffectBase{
private:

public:

	/**
	* @brief コンストラクタ
	*/
	BrightChanger(bool isCross = false);

	/**
	* @brief デストラクタ
	*/
	virtual ~BrightChanger() = default;

	/**
	* @brief 定数値変更
	* @param[in] cBufParam.x 元の描画をハッキリ残すか全発光か
	*						（残す=0.0f　全発光=1.0f）
	* @param[in] cBufParam.y 加算の強さ (基本値→0.15f)
	* @param[in] cBufParam.z 光の大きさ (基本値→0.0013f)
	* @param[in] cBufParam.w なし(画面比で使用する。設定済み。)
	*/
	void ChangeConstBuffer(const DirectX::SimpleMath::Vector4& cBufParam)override {
		ScreenEffectBase::SetConstBuf(cBufParam);
	}
};


class CrossBright :public ScreenEffectBase {
private:

	/** 光らせクラス */
	std::vector<std::unique_ptr<BrightChanger>> mpBrights;
public:

	/**
	* @brief コンストラクタ
	*/
	CrossBright();

	/**
	* @brief デストラクタ
	*/
	~CrossBright() = default;

	/**
	* @brief 定数値変更
	* @param[in] cBufParam.x 元の描画をハッキリ残すか全発光か
	*						（残す=0.0f　全発光=1.0f）
	* @param[in] cBufParam.y 加算の強さ (基本値→0.15f)
	* @param[in] cBufParam.z 光の大きさ (基本値→0.0013f)
	* @param[in] cBufParam.w なし(画面比で使用する。設定済み。)
	*/
	void ChangeConstBuffer(const DirectX::SimpleMath::Vector4 & cBufParam)override {
		ScreenEffectBase::SetConstBuf(cBufParam);
	}

	/**
	* @brief 描画効果開始
	*/
	//void Begin() override;

	/**
	* @brief 描画効果終了
	*/
	void End(bool isBlendAdd = false) override;
};