/**
 * @file Actor.h
 * @brief ステージに配置されるオブジェクトの基底クラス
 * @author suzuki.shunsuke
 * @date 2022/02/01
 */
#pragma once
#include "Libraries/MyLibraries/Transform.h"
#include <unordered_map>
#include <vector>

class Component;

/**
* @brief ステージに配置されるオブジェクトの基底クラス
*/
class Actor {
protected://全アクターが必ず持つ要素

	/** 座標・回転・拡縮パラメータ */
	Transform mTransform;

	/** アクターID */
	int mActorID;

private://アクターによって内容が変わる要素

	/** コンポーネント配列 */
	std::unordered_map<int,Component*> mpComponents;
	/** コンポーネントID配列 */
	std::vector<int> mpComponentIDs;
public:
	/**
	* @brief コンストラクタ
	*/
	Actor();

	/**
	* @brief デストラクタ
	*/
	virtual ~Actor();


	/**
	* @brief 更新
	*/
	virtual void Update() = 0;

	/**
	* @brief 描画
	*/
	virtual void Render() = 0;

public:
	/**
	 * @brief コンポーネントの追加
	 * @param[in] Component*（newされた物のみ）
	 * @return コンポーネントID
	 * @note 使用方法
	 * @note ①ComponentManager::CreateComponent経由で呼び出す。
	 * @note ②引数に new Component する。(コンポーネントによってはできない物もあり)
	 */
	int AddComponent(Component* pComponent);

private:
	/**
	* @brief アクターidを生成する
	*/
	inline int CreateActorID() {
		static int id = -1;
		id++;
		return id;
	}

public:
	/**
	 * @brief Transform値の取得
	 * @return transform値(const)
	 */
	inline const Transform& GetTransform() {
		return mTransform;
	}

	/**
	 * @brief Transform値の取得
	 * @return transform値(編集可能状態)
	 */
	inline Transform& GetTransformRef() {
		return mTransform;
	}

	/**
	 * @brief コンポーネントの取得
	 * @param[in] コンポーネントID
	 * @return Component*
	 */
	inline Component* const GetComponent(int id) {
		return mpComponents.at(id);
	}

	/**
	* @brief アクターIDの取得
	*/
	inline int GetActorID() {
		return mActorID;
	}
public:

	/**
	* @brief ComponentID配列の取得
	*/
	inline const std::vector<int>& GetComponentIDs() {
		return mpComponentIDs;
	}

};