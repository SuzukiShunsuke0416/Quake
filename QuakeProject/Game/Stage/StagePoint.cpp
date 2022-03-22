/**
 * @file StagePoint.cpp
 * @brief ステージを構成する１つの点（アクター）
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#include "pch.h"
#include "StagePoint.h"
#include "Game/ElevationColor/ElevationColorManager.h"
#include "StageManager.h"
#include "Game/Camera/CameraManager.h"

//#define FlatMode

using namespace DirectX;
using namespace DirectX::SimpleMath;

//=====================================================
//		コンストラクタ
//=====================================================
StagePoint::StagePoint()
	:Actor()
	,mpNextRight(nullptr)
	,mpNextBottom(nullptr)
	,mpNextRightBottom(nullptr)
	,mpColor(nullptr)
	,mpPrimitiveBatchDatas(nullptr)
{
}

//=====================================================
//		デストラクタ
//=====================================================
StagePoint::~StagePoint()
{
}

//=====================================================
//		初期化
//=====================================================
void StagePoint::Initialize(const StagePointInitDesc& desc, PerlinNoise& noiseMaker)
{
	mTransform.Location = desc.pos;
	mElement = desc.element;
	mpNextRight =		desc.nextRight;
	mpNextBottom =		desc.nextBottom;
	mpNextRightBottom =	desc.nextRightBottom;
	mpPrimitiveBatchDatas = desc.pLineBatchDatas;

	mpColor = new ElevationColor(this);

#ifdef FlatMode
	mTransform.Location.y = desc.element.x + desc.element.y;

#endif // FlatMode
#ifndef FlatMode
	mTransform.Location.y 
		= noiseMaker(thf::GetLocationFromSky(this) * 0.012f) 
		* StageManager::GetInstance()->GetMaxHeight();

#endif // !FlatMode

	mpNextPoints.insert(mpNextRight);
	mpNextPoints.insert(mpNextBottom);
	mpNextPoints.insert(mpNextRightBottom);
}

//=====================================================
//		更新
//=====================================================
void StagePoint::Update()
{
	/*DebugObjManager::GetInstance()->Entry(
		mTransform.Location,
		0.5f,
		mpColor->GetColor().V3()
	);*/
}

//=====================================================
//		面の描画
//=====================================================
void StagePoint::Render()
{
	// 減色率
	Vector4 subtractiveColorRat = Vector4(0.5f, 0.5f, 0.5f, 1.0f);

	// 頂点座標
	VertexPositionColorTexture ver[4] = {};

	ver[0] = VertexPositionColorTexture(// 自点（左上）
		mTransform.Location,
		mpColor->GetColor().V4() * subtractiveColorRat,
		Vector2::Zero
	);
	ver[1] = VertexPositionColorTexture(// 右隣（右上）
		thf::GetLocation(mpNextRight),
		mpNextRight->GetElevationColor()->GetColor().V4() * subtractiveColorRat,
		Vector2::Zero
	);
	ver[2] = VertexPositionColorTexture(// 真下（左下）
		thf::GetLocation(mpNextBottom),
		mpNextBottom->GetElevationColor()->GetColor().V4() * subtractiveColorRat,
		Vector2::Zero
	);
	ver[3] = VertexPositionColorTexture(// 斜め下（右下）
		thf::GetLocation(mpNextRightBottom),
		mpNextRightBottom->GetElevationColor()->GetColor().V4() * subtractiveColorRat,
		Vector2::Zero
	);

	// プリミティブバッチ描画開始
	mpPrimitiveBatchDatas->primitiveBatch->Begin();
	//四角形描画
	mpPrimitiveBatchDatas->primitiveBatch->DrawQuad(ver[0], ver[1], ver[3], ver[2]);
	// プリミティブバッチ描画終了
	mpPrimitiveBatchDatas->primitiveBatch->End();
}

//=====================================================
//		線の描画
//=====================================================
void StagePoint::LineRender()
{
	// カメラからview,proj行列を取得
	auto& view = CameraManager::GetInstance()->GetViewMatrix();
	auto& proj = CameraManager::GetInstance()->GetProjectionMatrix();

	//　ベーシックエフェクト適応
	mpPrimitiveBatchDatas->BasicEffectApply(view, proj);
	// プリミティブバッチ描画開始
	mpPrimitiveBatchDatas->primitiveBatch->Begin();

	for (StagePoint* point : mpNextPoints) {
		// 線を描画
		mpPrimitiveBatchDatas->primitiveBatch->DrawLine(
			VertexPositionColorTexture(mTransform.Location, mpColor->GetColor().V4(), Vector2::Zero),
			VertexPositionColorTexture(thf::GetLocation(point), point->GetElevationColor()->GetColor().V4(), Vector2::Zero)
		);
	}

	// プリミティブバッチ描画終了
	mpPrimitiveBatchDatas->primitiveBatch->End();
}
