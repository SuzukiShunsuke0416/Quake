/**
 * @file ComponentManager.h
 * @brief 特定コンポーネントを管理するマネージャの基底クラス
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */

#pragma once
#include <vector>
#include <unordered_map>
#include "Game/Actor/Actor.h"
#include <iterator>
#include "Game/Character/CharacterActor.h"
#include "Game/UtilityManager/MemberManager.h"

 /**
 * @brief コンポーネントを管理するマネージャの基底クラス
 */
template <typename MemberType>
class ComponentManager :
	public MemberManager<MemberType>
{
protected:

public:
	/**
	* @brief コンストラクタ
	*/
	ComponentManager() 
		:MemberManager()
	{
	}

	/**
	* @brief デストラクタ
	*/
	virtual ~ComponentManager() 
	{
	}

	/**
	* @brief 初期化
	*/
	virtual void Initialize() {}
	
	/**
	* @brief 更新
	*/
	virtual void Update() {}
	
	/**
	* @brief 描画
	*/
	virtual void Render() {}

public:
	/**
	 * @fn
	 * @brief メンバーの取得(actor内Component配列から一致するものを検索)
	 * @param[in] アクターptr
	 * @return メンバーの型ptr
	 */
	MemberType* GetComponentFromActor(Actor* pActor);
};

/**
 * @fn
 * @brief メンバーの取得(actor内Component配列から一致するものを検索)
 * @param アクターptr
 * @return メンバーの型ptr
 */
template<typename MemberType>
inline MemberType* ComponentManager<MemberType>::GetComponentFromActor(Actor* pActor)
{
	// actorが持っているコンポーネントのID配列を取得
	const auto& ActorCompIDs = pActor->GetComponentIDs();
	// 一致したIDを入れる配列。（１要素しか入らない。１以外なら不具合）
	std::vector<int> matchIDs;
	matchIDs.reserve(1);

	// ID配列同士で積集合を求める。
	std::set_intersection(
		mpMembersKeys.begin(), mpMembersKeys.end(),
		ActorCompIDs.begin(), ActorCompIDs.end(),
		std::back_inserter(matchIDs));

	// 被っていたIDを元にコンポーネントを検索する。
	return GetMemberFromID(matchIDs.front());
}
