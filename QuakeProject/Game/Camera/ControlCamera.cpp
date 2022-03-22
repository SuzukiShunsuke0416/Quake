#include "pch.h"
#include "ControlCamera.h"
#include "Game/UtilityManager/InputManager.h"
#include "Libraries/MyLibraries/Transform.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// 注視点の位置（オーナー＋何ｍ上か）
const float ControlCamera::ADD_VIEW_HEIGHT = 10.0f;

// オーナーとの距離
const Vector3 ControlCamera::DISTANCE = Vector3(0.0f, 24.0f, 10.0f);

//=====================================================
//		コンストラクタ
//=====================================================
ControlCamera::ControlCamera(Actor* pOwner)
	:MyCamera(pOwner)
	,mMouseCursorPosLog()
	,mDistanceRat(1.0f)
{
	const auto& mouseTrack = InputManager::GetInstance()->GetMouseTracker();
	mMouseCursorPosLog.x = mouseTrack.GetLastState().x;
	mMouseCursorPosLog.y = mouseTrack.GetLastState().y;
}

//=====================================================
//		デストラクタ
//=====================================================
ControlCamera::~ControlCamera()
{
}

//=====================================================
//		更新
//=====================================================
void ControlCamera::Update()
{
	// オーナーとカメラの距離差を計算
	Vector3 distance;
	this->CalculateDistance(distance);

	// オーナーの座標
	Vector3 ownerPos = thf::GetLocation(mpOwnerActor);

	// カメラ座標変更
	this->SetEyePosition(ownerPos + distance);

	// 注視点座標変更
	this->SetTargetPosition(ownerPos + Vector3(0.0f,ADD_VIEW_HEIGHT * mDistanceRat,0.0f));

	// ビュー・プロジェクション行列を設定
	this->CalculateViewMatrix();

	//TODO オーナーからの速度を元に
	const auto& keyboard = InputManager::GetInstance()->GetKeyboardTracker().GetLastState();
	if (keyboard.W || keyboard.S || keyboard.A || keyboard.D) {
		myf::RubberComplementSlow(mDistanceRat, 1.0f);
	}
	else {
		myf::RubberComplementSlow(mDistanceRat,0.5f);
	}
}

//=====================================================
//		マウス操作に合わせて距離を変える
//=====================================================
void ControlCamera::CalculateDistance(Vector3& distance)
{
	// トラッカーを取得
	const auto& mouseTrack = InputManager::GetInstance()->GetMouseTracker();

	// 現在のマウスカーソルの位置を取得
	Int2 cursorPos = Int2(mouseTrack.GetLastState().x, mouseTrack.GetLastState().y);

	// 左シフトキーが押されていたら、動かす
	if (mouseTrack.GetLastState().rightButton == true) {
		// カーソル移動量を設定
		mMoveVol.x -= (float)(cursorPos.x - mMouseCursorPosLog.x);
		mMoveVol.y -= (float)(cursorPos.y - mMouseCursorPosLog.y);

		// 上下操作の限界値を設定
		myf::ClampRef(mMoveVol.y, 1200.0f, 0.0f);
	}

	// カーソル移動量を元に角度を作る
	Matrix eyeMat = Matrix::CreateTranslation(DISTANCE * mDistanceRat);
	eyeMat *= Matrix::CreateRotationX(mMoveVol.y * 0.001f);
	eyeMat *= Matrix::CreateRotationY(mMoveVol.x * 0.002f);

	DebugLogManager::GetInstance()->EntryLog(mMoveVol.y, L"MouseY");

	// オーナーとカメラの位置差を設定
	distance = eyeMat.Translation();

	// カーソルログ値を更新
	mMouseCursorPosLog = cursorPos;
}
