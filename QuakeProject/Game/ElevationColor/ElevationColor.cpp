/**
 * @file ElevationColor.cpp
 * @brief	オーナーの高さ座標に応じて値が変化する
 * @brief	色情報を扱うコンポーネント
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#include "pch.h"
#include "ElevationColor.h"
#include "ElevationColorManager.h"

 //=====================================================
 //		コンストラクタ
 //=====================================================
ElevationColor::ElevationColor(Actor* pOwner)
	:Component(pOwner)
	, mColor()
{
	// 追加
	ElevationColorManager::GetInstance()->AddMember(this);
}

//=====================================================
//		デストラクタ
//=====================================================
ElevationColor::~ElevationColor()
{
	// 除外
	ElevationColorManager::GetInstance()->RemoveMember(this);
}
