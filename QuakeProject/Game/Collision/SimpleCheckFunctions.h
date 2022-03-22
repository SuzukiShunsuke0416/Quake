/**
 * @file SimpleCheckFunctions.h
 * @brief シンプルな判定を行う関数をまとめたファイル
 * @author suzuki.shunsuke
 * @date 2022/02/06
 */
#pragma once
#include "pch.h"

 /**
 * @brief 四角型の情報をまとめておくデータ群
 */
template <class T>
struct BoxParam {
	T left;
	T right;
	T top;
	T bottom;

	/**
	* @brief コンストラクタ
	*/
	BoxParam(T lleft, T lright, T ltop, T lbottom) 
		:left(lleft)
		,right(lright)
		,top(ltop)
		,bottom(lbottom)
	{
	}

	/**
	* @brief 箱を縮ませる。
	* @note leftには足し、rightには引く。top,bottomも同じ。
	*/
	void Shrink(T x, T y) {
		left += x; right -= x;
		top += y; bottom -= y;
	}

	/**
	* @brief 箱を引き伸ばす。
	* @note leftには引き、rightには足す。top,bottomも同じ。
	*/
	void Grow(T x, T y) {
		left -= x; right += x;
		top -= y; bottom += y;
	}
};

/**
* @brief シンプルな判定を行う関数を名前空間
*/
namespace SimpleCheckFunctions {

	/**
	 * @brief	上下限値(四角)の中に値(点)が入っているかどうか
	 * @tparam[in] left		四角の左
	 * @tparam[in] invalX	点の横座標
	 * @tparam[in] right	四角の右
	 * @tparam[in] top		四角の上
	 * @tparam[in] inValY	点の縦座標
	 * @tparam[in] bottom	四角の下
	 * @return bool	
	 */
	template <class T>
	inline static bool IsBoxInPoint(T left, T invalX, T right, T top, T inValY, T bottom) {
		return
			(left <= invalX && invalX <= right) &&
			(top <= inValY && inValY <= bottom);
	}

	/**
	 * @brief	上下限値(四角)の中に値(点)が入っているかどうか
	 * @param[in] outBox	四角のデータ群
	 * @tparam[in] invalX	点の横座標
	 * @tparam[in] inValY	点の縦座標
	 * @return bool
	 */
	template <class T>
	inline static bool IsBoxInPoint(const BoxParam<T>& outBox, const T& inValX, const T& inValY) {
		return IsBoxInPoint(outBox.left, inValX, outBox.right, outBox.top, inValY, outBox.bottom);
	}

	/**
	 * @brief	上下限値(四角)の中に値(点)が入っているかどうか
	 * @param[in] outBox	四角のデータ群
	 * @tparam[out] invalX	点の横座標
	 * @tparam[out] inValY	点の縦座標
	 * @note もしも四角から点が出ていた場合、受け取った点の座標を変更する。
	 */
	template <class T>
	inline static void IsBoxInPointRef(const BoxParam<T>& outBox, T& inValX, T& inValY) {
		myf::ClampRef(inValX, outBox.right, outBox.left);
		myf::ClampRef(inValY, outBox.bottom, outBox.top);
	}

	/**
	 * @brief	上下限値(四角)の中に値(点)が入っているかどうか(vec2バージョン)
	 * @param[in] topleft		左上
	 * @param[in] bottomright	右下
	 * @param[in] inVal			点
	 * @return bool
	 */
	inline static bool IsBoxInPoint(
		const DirectX::SimpleMath::Vector2& topleft,
		const DirectX::SimpleMath::Vector2& bottomright,
		const DirectX::SimpleMath::Vector2& inVal) 
	{
		return 
			(topleft.x <= inVal.x && inVal.x <= bottomright.x) &&
			(topleft.y <= inVal.y && inVal.y <= bottomright.y);
	}

	/**
	 * @brief	上下限値の中に値(箱)が入っているかどうか
	 * @param[in] outBox	外側箱
	 * @param[in] inBox		内側箱
	 * @return bool
	 */
	template <class T>
	inline static bool IsBoxInBox(const BoxParam<T>& outBox, const BoxParam<T>& inBox) {
		return
			(outBox.left <= inBox.left && inBox.right <= outBox.right) &&
			(outBox.top <= inBox.top && inBox.bottom <= outBox.bottom);
	}
}