/**
 * @file PerlinNoise.h
 * @brief パーリンノイズを生成するクラス
 * @author suzuki.shunsuke
 * @date 2022/02/11
 */
#pragma once

 /**
 * @brief パーリンノイズを生成するクラス
 */
class PerlinNoise {
private:
	/** 乱数調整値 */
	float mAdjustedValue;

public:
	/**
	* @brief コンストラクタ
	*/
	PerlinNoise();

	/**
	* @brief デストラクタ
	*/
	~PerlinNoise() = default;

	/**
	 * @brief	パーリンノイズの結果を取得
	 */
	float operator()(const DirectX::SimpleMath::Vector2& uv);

private:
	/**
	 * @brief	疑似乱数生成
	 */
	float Random(const DirectX::SimpleMath::Vector2& uv);
};