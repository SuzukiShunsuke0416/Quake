/**
 * @file CharacterActor.cpp
 * @brief キャラクター系アクター
 * @author suzuki.shunsuke
 * @date 2022/02/04
 */
#include "pch.h"
#include "CharacterActor.h"
#include "Game/Stage/StageManager.h"

using namespace DirectX::SimpleMath;

 //=====================================================
 //		更新
 //=====================================================
void CharacterActor::Update()
{
	// 1f前からの移動差を計算
	mDistanceFrommBefore1f = mTransform.Location - mLocationLog;
	mLocationLog = mTransform.Location;

	// 重力適用
	mVelocity.y -= mGravity;

	// 座標変更
	mTransform.Location += mVelocity;

	// 減速
	mVelocity *= mSpeedDownRat;


	// ステージから出ないようにする
	float size = thf::GetScale(this).x;
	Vector2 pos = thf::GetLocationFromSky(this);
	BoxParam<float> limitBox = StageManager::GetInstance()->GetSidePosBox();
	limitBox.Shrink(size, size);
	SimpleCheckFunctions::IsBoxInPointRef(limitBox, pos.x, pos.y);
	thf::SetLocationFromSky(this, pos);
}
