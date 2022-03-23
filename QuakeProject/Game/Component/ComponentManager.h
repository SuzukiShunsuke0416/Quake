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

 /**
 * @brief コンポーネントを管理するマネージャの基底クラス
 */
template <typename MemberType>
class ComponentManager {
protected:
	/**  管理するメンバーたち（通常使い用） */
	std::vector<MemberType*> mpMembers;

	/** メンバたち（ID検索用） */
	std::unordered_map<int,MemberType*> mpMembersForSearch;
	
	/** メンバたちのIDのみ管理する */
	std::vector<int> mpMembersKeys;

public:
	/**
	* @brief コンストラクタ
	*/
	ComponentManager() 
		:mpMembers()
		,mpMembersForSearch()
		,mpMembersKeys()
	{
	}

	/**
	* @brief デストラクタ
	*/
	virtual ~ComponentManager() {
		mpMembers.clear();
		mpMembersForSearch.clear();
		mpMembersKeys.clear();
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
	 * @brief メンバーの追加
	 * @param[in] 追加するコンポーネントのポインタ
	 */
	void AddMember(MemberType* pAddMem);

	/**
	 * @brief メンバーの除外
	 * @param[in] 除外するコンポーネントのポインタ
	 */
	void RemoveMember(MemberType* pRemMem);

	/**
	 * @brief メンバーの取得(ID指定)
	 * @param[in] ComponentID
	 * @return メンバーの型ptr
	 */
	MemberType* GetMemberFromID(int id);

	/**
	 * @fn
	 * @brief メンバーの取得(actor内Component配列から一致するものを検索)
	 * @param[in] アクターptr
	 * @return メンバーの型ptr
	 */
	MemberType* GetComponentFromActor(Actor* pActor);
};


//=====================================================
//		追加
//=====================================================
template<typename MemberType>
inline void ComponentManager<MemberType>::AddMember(MemberType* pAddMem)
{
	// 配列へ追加する
	mpMembers.push_back(pAddMem);
	// 配列（ID検索用）へ追加する
	mpMembersForSearch[pAddMem->GetID()] = pAddMem;
	// 配列（ID）へ追加する
	mpMembersKeys.push_back(pAddMem->GetID());

	// 配列（ID）を昇順に並び替える
	std::sort(mpMembersKeys.begin(), mpMembersKeys.end());
}

//=====================================================
//		除外
//=====================================================
template<typename MemberType>
inline void ComponentManager<MemberType>::RemoveMember(MemberType* pRemMem)
{
	// 配列から除外する
	 auto result = std::remove_if(mpMembers.begin(), mpMembers.end(), [&](MemberType* mem) { return pRemMem == mem; });
	 mpMembers.erase(result, mpMembers.end());

	// 配列（ID検索用）から除外する
	mpMembersForSearch.erase(pRemMem->GetID());

	// 配列（ID）から除外する
	auto result2 = std::remove_if(mpMembersKeys.begin(), mpMembersKeys.end(), [&](int key) { return pRemMem->GetID() == key; });
	mpMembersKeys.erase(result2, mpMembersKeys.end());
}

/**
 * @fn
 * @brief メンバーの取得(ID指定)
 * @param ComponentID
 * @return メンバーの型ptr
 */
template<typename MemberType>
inline MemberType* ComponentManager<MemberType>::GetMemberFromID(int id)
{
	return mpMembersForSearch.at(id);
}

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
