/**
 * @file ElevationColor.h
 * @brief	オーナーの高さ座標に応じて値が変化する
 * @brief	色情報を扱うコンポーネント
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#pragma once
#include "Game/Component/Component.h"
#include "Libraries/MyLibraries/Utility.h"

 /**
 * @brief オーナーの高さ座標に応じて値が変化する
 * @brief 色情報を扱うコンポーネント
 * @note ElevationColorManagerが一括で変更する
 */
class ElevationColor :public Component {
private:
	/** 色情報 */
	ColorData mColor;
public:
	/**
	* @brief コンストラクタ
	* @param[in] pOwner	オーナーポインタ
	*/
	ElevationColor(Actor* pOwner);

	/**
	* @brief デストラクタ
	*/
	~ElevationColor();

public:
	/**
	* @brief 色情報を取得
	*/
	inline ColorData& GetColor() {
		return mColor;
	}

	/**
	 * @brief	色情報を設定
	 * @param[in] newColor 新しい色
	 */
	inline void SetColorV3(
		const DirectX::SimpleMath::Vector3& newColor)
	{
		mColor.r = newColor.x;
		mColor.g = newColor.y;
		mColor.b = newColor.z;
	}
};