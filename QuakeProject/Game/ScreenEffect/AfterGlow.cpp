#include "pch.h"
#include "AfterGlow.h"
#include <Model.h>
#include "Game/Camera/CameraManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//=====================================================
//		コンストラクタ
//=====================================================
AfterGlow::AfterGlow(float reductionRat, int glowSize)
{
	mReductionRat = reductionRat;
	mGlowSize = glowSize;

	mpBright = std::make_unique<BrightChanger>();
	mpBright->ChangeConstBuffer({ 1.0f,0.2f,0.0013f,0.0f });

	mUpdateLogFunction = std::mem_fn(&AfterGlow::PushModelMat);
}


//=====================================================
//		残光適応開始（ｼﾞｵﾒﾄﾘｯｸﾌﾟﾘﾐﾃｨﾌﾞ）
//=====================================================
void AfterGlow::Adaptation(
	GeometricPrimitive* pGeoPri,
	Matrix ModelMat,
	XMVECTORF32 color
)
{
	// モデル行列ログ追加
	mUpdateLogFunction(this, ModelMat);

	// カメラ関連の行列を取得
	const Matrix view = CameraManager::GetInstance()->GetViewMatrix();
	const Matrix proj = CameraManager::GetInstance()->GetProjectionMatrix();

	// 発光描画準備開始
	mpBright->Begin();

	float size = 1.0f;
	for (Matrix world : mModelMatrixLogs) {
		Vector3 pos = world.Translation();
		world *= Matrix::CreateTranslation(-pos);
		world *= Matrix::CreateScale(size);
		world *= Matrix::CreateTranslation(pos);

		pGeoPri->Draw(world, view, proj, color);

		size *= mReductionRat;
	}

	// 発光描画終了
	mpBright->End(true);

	DebugLogManager::GetInstance()->EntryLog(mModelMatrixLogs.size(), L"AfGlowSize");
}

//=====================================================
//		残光適応開始（モデル）
//=====================================================
void AfterGlow::Adaptation(
	Model* pModel, 
	CommonStates* pCommonStates, 
	Matrix ModelMat)
{
	// モデル行列ログ追加
	mUpdateLogFunction(this, ModelMat);

	// 発光描画準備開始
	mpBright->Begin();



	// 発光描画終了
	mpBright->End();
}

//=====================================================
//		モデル行列のログを追加する
//=====================================================
void AfterGlow::PushModelMat(const Matrix& modelMat)
{
	// ログ追加
	mModelMatrixLogs.push_front(modelMat);

	// もしサイズオーバーだったら、次回以降は削除も行う
	if (mModelMatrixLogs.size() >= mGlowSize) {
		mUpdateLogFunction = std::mem_fn(&AfterGlow::PushPopModelMat);
	}
}

//=====================================================
//		モデル行列のログを追加・削除する
//=====================================================
void AfterGlow::PushPopModelMat(const DirectX::SimpleMath::Matrix& modelMat)
{
	// ログ削除
	mModelMatrixLogs.pop_back();

	// ログ追加
	mModelMatrixLogs.push_front(modelMat);
}
