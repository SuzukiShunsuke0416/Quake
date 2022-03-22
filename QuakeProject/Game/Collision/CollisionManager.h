/**
 * @file CollisionManager.h
 * @brief 当たり判定を一括で管理する。
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#pragma once
#include "Game/UtilityManager/SingletonFounder.h"
#include "Game/Component/ComponentManager.h"
#include "Game/Collision/Collision.h"
#include <functional>
#include <unordered_map>

 /**
 * @brief 当たり判定を一括で管理するマネージャ
 */
class CollisionManager
	:public SingletonFounder<CollisionManager>
	, public ComponentManager<Collision> 
{
	friend SingletonFounder;

private:
	/** 当たり判定を担当する関数たちをまとめた配列 */
	std::unordered_map<eCollisionType,
		std::function<bool(CollisionManager*, Collision*, Collision*)>>
		mCheckMap;

	/** 単体系当たり判定を担当する関数たち */
	std::unordered_map<eCollisionType,
		std::function<void(CollisionManager*, Collision*)>>
		mSingleFunctionMap;

private:
	/**
	* @brief コンストラクタ
	*/
	CollisionManager();

	/**
	* @brief デストラクタ
	*/
	~CollisionManager() = default;

public:

	/**
	* @brief 更新
	*/
	void Update() override;

private:
	/**
	 * @brief	相手actor無し系判定をまとめる
	 * @param[in] pCol		コリジョンポインタ
	 * @param[in] myType	自分のタイプ
	 * @param[in] vsType	相手のタイプ
	 */
	void CheckSingles(
		Collision* const pCol,
		const eCollisionType myType,
		const eCollisionType vsType);

	/**
	* @brief キャラクターと地面
	*/
	void Character_vs_Floor(Collision* pCol);
};