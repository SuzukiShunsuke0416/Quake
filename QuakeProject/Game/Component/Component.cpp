/**
 * @file Component.h
 * @brief 「アクタに追加することができる機能の構成要素」の基底クラス
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#include "pch.h"
#include "Component.h"
#include "Game/Actor/Actor.h"

 //=====================================================
 //		コンストラクタ
 //=====================================================
Component::Component(Actor* pOwner)
	:mpOwnerActor(pOwner)
	, mComponentID()
{
	// IDを生成
	CreateID();
	// アクターに自分を追加
	pOwner->AddComponent(this);
}
