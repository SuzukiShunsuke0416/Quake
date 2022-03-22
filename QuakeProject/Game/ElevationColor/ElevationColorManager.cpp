/**
 * @file ElevationColorManager.cpp
 * @brief ElevationColorコンポーネントを管理するマネージャ
 * @author suzuki.shunsuke
 * @date 2022/02/05
 */
#include "pch.h"
#include "ElevationColorManager.h"
#include "Game/Stage/StageManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//=====================================================
//		コンストラクタ
//=====================================================
ElevationColorManager::ElevationColorManager()
	:ComponentManager()
	,mMaxHeight()
{
}

//=====================================================
//		初期化
//=====================================================
void ElevationColorManager::Initialize()
{
	//ステージマネージャから、最大標高と山頂・地面色を取得する。
	auto stageManager = StageManager::GetInstance();
	ColorData topColorData = stageManager->GetTopColor();
	ColorData groundColorData = stageManager->GetGroundColor();

	//最大標高、色を設定する。
	mMaxHeight = stageManager->GetMaxHeight();
	mTopColor = Vector3(topColorData.r, topColorData.g, topColorData.b);
	mGroundColor = Vector3(groundColorData.r, groundColorData.g, groundColorData.b);
}

//=====================================================
//		更新
//=====================================================
void ElevationColorManager::Update()
{
	for (const auto color : mpMembers) {
		float heightRat = color->GetOwner()->GetTransform().Location.y / mMaxHeight;

		color->SetColorV3(myf::Lerp(mGroundColor, mTopColor, heightRat));
	}
}
