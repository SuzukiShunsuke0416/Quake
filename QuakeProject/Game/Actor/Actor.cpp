/**
 * @file Actor.cpp
 * @brief ステージに配置されるオブジェクトの基底クラス
 * @author suzuki.shunsuke
 * @date 2022/02/01
 */
#include "pch.h"
#include "Actor.h"
#include "Game/Component/Component.h"

 //=====================================================
 //		コンストラクタ
 //=====================================================
Actor::Actor()
	: mTransform()
	, mpComponents()
	, mpComponentIDs()
	, mActorID()
{
	mActorID = this->CreateActorID();
}

//=====================================================
//		デストラクタ
//=====================================================
Actor::~Actor()
{
	// 追加されたコンポーネントの削除
	for (auto& comp : mpComponents) {
		delete comp.second;
		comp.second = nullptr;
	}
	mpComponents.clear();
}

//=====================================================
//		コンポーネントの追加
//=====================================================
int Actor::AddComponent(Component* pComponent)
{
	// コンポーネントを追加
	mpComponents[pComponent->GetID()] = pComponent;
	mpComponentIDs.push_back(pComponent->GetID());

	// ID配列を昇順にしておく
	std::sort(mpComponentIDs.begin(), mpComponentIDs.end());

	// コンポーネントIDを返す
	return pComponent->GetID();
}
