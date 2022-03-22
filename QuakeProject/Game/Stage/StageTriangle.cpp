/**
 * @file	StageTriangle.cpp
 * @brief	オーナーの足元にある地面情報（三角）
			をまとめたコンポーネント
 * @author	suzuki.shunsuke
 * @date	2022/02/02
 */
#include "pch.h"
#include "StageTriangle.h"
#include "StagePoint.h"
#include "StageManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//=====================================================
//		コンストラクタ
//=====================================================
StageTriangle::StageTriangle(Actor* pOwner)
	:Component(pOwner)
	,mpStagePoint()
	,mNormal()
	,mNormalRotationMatrix()
	,mAzimuth()
	,mElevationAngle()
{
	StageManager::GetInstance()->AddTriangleMember(this);
}

//=====================================================
//		デストラクタ
//=====================================================
StageTriangle::~StageTriangle()
{
	StageManager::GetInstance()->RemoveTriangleMember(this);
}

//=====================================================
//		更新
//=====================================================
void StageTriangle::Update()
{
	const Vector3 triPos1 = thf::GetLocation(mpStagePoint[0]);
	const Vector3 triPos2 = thf::GetLocation(mpStagePoint[1]);
	const Vector3 triPos3 = thf::GetLocation(mpStagePoint[2]);

	// 法線ベクトル設定
	mNormal = (triPos3 - triPos1).Cross(triPos2 - triPos1);

	// 方位角
	mAzimuth = atan2f(mNormal.z, -mNormal.x);

	// 仰角
	mElevationAngle = atan2f(Vector2(mNormal.x, mNormal.z).Length(), mNormal.y);

	// 回転行列設定
	mNormalRotationMatrix = Matrix::Identity;
	mNormalRotationMatrix *= Matrix::CreateRotationZ(mElevationAngle);
	mNormalRotationMatrix *= Matrix::CreateRotationY(mAzimuth);
}
