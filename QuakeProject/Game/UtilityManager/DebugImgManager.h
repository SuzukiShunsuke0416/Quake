/**
 * @file DebugImgManager.h
 * @brief 画像を表示する専用のマネージャー
 * @author suzuki.shunsuke
 * @date 2022/03/03
 */

#pragma once
#include "SingletonFounder.h"
#include <SpriteBatch.h>
#include <unordered_map>
#include <vector>
#include <CommonStates.h>

/**
 * @brief 音のデバッグイメージの描画タイプ
 */
enum class eDebugImgType {
	Full1,
	Full2,
	TridentLogo,

	TYPE_NUM,
};

/**
 * @brief 画像を表示する専用のマネージャー
 * @note 外部から値(種類・座標)を渡すとDraw関数が呼ばれた際
 * @note 一括で画像描画する。
 */
class DebugImgManager :public SingletonFounder<DebugImgManager> {
	friend SingletonFounder;

private:
	/**
	* @brief コンストラクタ
	*/
	DebugImgManager();

	/** コモンステート */
	std::unique_ptr<DirectX::CommonStates> mCommonState;

	/** スプライトバッチ */
	std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;

	/** テクスチャ */
	std::unordered_map<eDebugImgType,
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>mTextures;

	/** 表示位置配列 */
	std::unordered_map <eDebugImgType, std::vector<std::pair<int, int>>>
		mPositions;
public:
	/**
	* @brief デストラクタ
	*/
	~DebugImgManager() = default;

	/**
	* @brief 描画
	*/
	void Draw();

	/**
	* @brief 追加
	* @param[in] type	描画タイプ
	* @param[in] x	描画座標（スクリーンx）
	* @param[in] y	描画座標（スクリーンy）
	*/
	void Entry(eDebugImgType type, int x = 0, int y = 0);

	/**
	* @brief 呼ばれた瞬間描画
	* @param[in] type	描画タイプ
	* @param[in] x	描画座標（スクリーンx）
	* @param[in] y	描画座標（スクリーンy）
	*/
	void QuickDraw(eDebugImgType type, int x = 0, int y = 0);
private:

	/**
	* @brief 配列データのリセット
	*/
	void Reset();
};