/**
 * @file Collision.h
 * @brief コリジョンクラス
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#pragma once
#include "Game/Component/Component.h"
#include <functional>


/**
* @brief 当たり判定の種類特定に使う型
*/
enum class eCollisionType : unsigned{
	NONE = 0,

	//===各アクターに応じてタイプを作る===//
	Player	= 1 << 0,
	Enemy	= 1 << 1,
	Item	= 1 << 2,

	//===各アクターが単身で判定を取りに行くタイプ===//
	Floor	= 1 << 3,
	Wall	= 1 << 4,

	SINGLE_START = Floor,
	SINGLE_END = Wall,

	//===相手actorがいなくても処理をさせる型群===//
	SINGLES = (Floor | Wall),

};

/**
* @brief eCollisionTypeを扱いやすくする関数をまとめた名前空間
*/
namespace ctf {
	/**
	 * @brief	受け取った次のコリジョンタイプを返す
	 * @param[in] type	現在のコリジョンタイプ
	 * @return eCollisionType 次のコリジョンタイプ
	 */
	static eCollisionType next(const eCollisionType type) 
	{
		return static_cast<eCollisionType>(static_cast<unsigned>(type) * 2);
	}
}

/*============================================
	eCollisionType用の演算子オーバーロード
============================================*/

/** eCollisionType演算子　＆ */
eCollisionType operator&(const eCollisionType& t1, const eCollisionType& t2);

/** eCollisionType演算子　| */
eCollisionType operator|(const eCollisionType& t1, const eCollisionType& t2);


/**
* @brief コリジョンクラス
*/
class Collision :public Component {
public:
	/** 当たり判定を取る関数の型を別名定義 */
	using HitFuncType = std::function<void(Collision*)>;

private:
	/** 自分のタイプ */
	eCollisionType mMyType;

	/** 相手のタイプ */
	eCollisionType mVsType;

	// オーナーptr
	//(componentクラスに有)

	/** オーナーのHit関数のポインタ */
	HitFuncType mHitFunction;

public:

	/**
	 * @fn
	 * @brief	Collisionクラスのコンストラクタ
	 * @param[in] pOwner	オーナーポインタ
	 * @param[in] myType	自分のタイプ
	 * @param[in] vsType	相手のタイプ
	 * @param[in] hitFunctionPtr	ヒット関数呼び出し用関数ptr
	 */
	Collision(
		Actor* pOwner, 
		eCollisionType myType, 
		eCollisionType vsType, 
		HitFuncType hitFunctionPtr
	);

	/**
	* @brief デストラクタ
	*/
	~Collision();

public:

	/**
	 * @brief	Hit関数
	 * @param[in] pCol	コリジョンポインタ
	 * @note	(ここ経由で各アクターのHit関数へ飛ぶ)
	 */
	void Hit(Collision* pCol);

public:
	/**
	* @brief 自分のタイプを取得
	*/
	eCollisionType GetMyType() { return mMyType; }

	/**
	* @brief 相手のタイプを取得
	*/
	eCollisionType GetVsType() { return mVsType; }
};