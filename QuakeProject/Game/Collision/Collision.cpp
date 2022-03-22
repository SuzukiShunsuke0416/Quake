/**
 * @file Collision.cpp
 * @brief コリジョンクラス
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#include "pch.h"
#include "Collision.h"
#include "CollisionManager.h"

 //=====================================================
 //		コンストラクタ
 //=====================================================
Collision::Collision(
	Actor* pOwner, 
	eCollisionType myType, 
	eCollisionType vsType, 
	HitFuncType hitFunctionPtr
)
	: Component(pOwner)
	, mMyType(myType)
	, mVsType(vsType)
	, mHitFunction(hitFunctionPtr)
{
	// 追加
	CollisionManager::GetInstance()->AddMember(this);

	// hit関数ptrのnullチェック
	if (mHitFunction == nullptr) {
		// 関数ptrが設定されていない場合は、適当なラムダ式を挿入しておく。
		mHitFunction = [](Collision* pCol) { UNREFERENCED_PARAMETER(pCol); };
	}
}

//=====================================================
//		デストラクタ
//=====================================================
Collision::~Collision()
{
	// 除外
	CollisionManager::GetInstance()->RemoveMember(this);
}

//=====================================================
//		 Hit関数(ここ経由で各アクターのHit関数へ飛ぶ)
//=====================================================
void Collision::Hit(Collision* pCol)
{
	mHitFunction(pCol);
}


/*============================================
	eCollisionType用の演算子オーバーロード
============================================*/
eCollisionType operator&(const eCollisionType& t1, const eCollisionType& t2)
{
	return static_cast<eCollisionType>
		(static_cast<unsigned>(t1) & static_cast<unsigned>(t2));
}

eCollisionType operator|(const eCollisionType& t1, const eCollisionType& t2)
{
	return static_cast<eCollisionType>
		(static_cast<unsigned>(t1) | static_cast<unsigned>(t2));
}
