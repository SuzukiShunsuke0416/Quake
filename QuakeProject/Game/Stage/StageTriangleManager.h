/**
 * @file StageTriangleManager.h
 * @brief ステージトライアングルコンポーネントを管理する。
 * @author suzuki.shunsuke
 * @date 2022/02/011
 */
#pragma once
#include "Game/Component/ComponentManager.h"
#include "StageTriangle.h"

 /**
 * @brief StageTriangleを管理する。
 */
class StageTriangleManager :public ComponentManager<StageTriangle> {

public:
	/**
	* @brief コンストラクタ
	*/
	StageTriangleManager();
	/**
	* @brief デストラクタ
	*/
	~StageTriangleManager();


public:
	/**
	* @brief メンバー配列の取得
	* @note 親クラスのComponentManagerに、
			配列自体を取得する関数は存在しないため実装。
	*/
	const std::vector<StageTriangle*>& GetStageTriangles() {
		return mpMembers;
	}
};