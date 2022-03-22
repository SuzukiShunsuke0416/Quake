/**
 * @file Component.h
 * @brief 「アクタに追加することができる機能の構成要素」の基底クラス
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#pragma once

class Actor;
/**
* @brief コンポーネント
*/
class Component {
protected:
	/** このコンポーネントの持ち主 */
	Actor* mpOwnerActor;

	/** コンポーネントID */
	int mComponentID;

protected:
	/**
	* @brief コンストラクタ
	*/
	Component(Actor* pOwner);

public:

	/**
	* @brief デストラクタ
	*/
	virtual ~Component() = default;


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
	* @brief コンポーネントＩＤ取得
	*/
	int GetID() { return mComponentID; }
	
	/**
	* @brief オーナーの取得
	*/
	Actor* const GetOwner() { return mpOwnerActor; }

private:
	/**
	* @brief コンポーネントID生成
	*/
	inline void CreateID() {
		static int id = 0;
		mComponentID = id;
		id++;
	}
};
