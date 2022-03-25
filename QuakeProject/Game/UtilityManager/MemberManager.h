/**
 * @file MemberManager.h
 * @brief あるクラスを一括管理するマネージャクラスの基底
 * @author suzuki.shunsuke
 * @date 2022/03/26
 */
#pragma once

 /**
 * @brief あるクラスを一括管理するマネージャクラスの基底
 * @note メンバとして扱える型は、一意に割り振られたID(int型)を持つもののみ。
 *		 そしてIDを「GetID」という関数で取得できるようにしておく。
 *		 なお、メンバ達のポインタは管理するがメモリ管理は担当しない。
 */
template <typename MemberType>
class MemberManager {
protected:
	/**  管理するメンバーたち（通常使い用） */
	std::vector<MemberType*> mpMembers;

	/** メンバたち（ID検索用） */
	std::unordered_map<int, MemberType*> mpMembersForSearch;

	/** メンバたちのIDのみ管理する */
	std::vector<int> mpMembersKeys;

public:
	/**
	* @brief コンストラクタ
	*/
	MemberManager()
		:mpMembers()
		, mpMembersForSearch()
		, mpMembersKeys()
	{
	}

	/**
	* @brief デストラクタ
	*/
	virtual ~MemberManager() {
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
	 * @return 追加したメンバのID
	 */
	int AddMember(MemberType* pAddMem);

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
};


//=====================================================
//		追加
//=====================================================
template<typename MemberType>
inline int MemberManager<MemberType>::AddMember(MemberType* pAddMem)
{
	int id = pAddMem->GetID();

	// 配列へ追加する
	mpMembers.push_back(pAddMem);
	// 配列（ID検索用）へ追加する
	mpMembersForSearch[id] = pAddMem;
	// 配列（ID）へ追加する
	mpMembersKeys.push_back(id);

	// 配列（ID）を昇順に並び替える
	std::sort(mpMembersKeys.begin(), mpMembersKeys.end());

	// IDを返却する
	return id;
}

//=====================================================
//		除外
//=====================================================
template<typename MemberType>
inline void MemberManager<MemberType>::RemoveMember(MemberType* pRemMem)
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
inline MemberType* MemberManager<MemberType>::GetMemberFromID(int id)
{
	return mpMembersForSearch.at(id);
}