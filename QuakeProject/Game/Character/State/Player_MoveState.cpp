/**
 * @file Player_MoveState.cpp
 * @brief プレイヤー移動ステート
 * @author suzuki.shunsuke
 * @date 2022/02/12
 */
#include "pch.h"
#include "Player_MoveState.h"
#include "Game/Camera/CameraManager.h"
#include "Game/Camera/MyCamera.h"
#include "Game/UtilityManager/InputManager.h"
#include "../Player/Player.h"
#include "Libraries/MyLibraries/Utility.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//=====================================================
//		コンストラクタ
//=====================================================
Player_MoveState::Player_MoveState(Player* pOwner)
	:CharacterStateBase(pOwner)
	,mpCamera()
	,mMoveWayAngle(0.0f)
	, mFrontWay()
{
	// オーナーが持っているカメラをマネージャ経由で受け取る
	mpCamera = CameraManager::GetInstance()->GetComponentFromActor(mpOwner);
}

//=====================================================
//		リセット
//=====================================================
void Player_MoveState::Reset()
{
}

//=====================================================
//		更新
//=====================================================
void Player_MoveState::Update()
{
	// keyステートを取得
	const auto& keytrack = InputManager::GetInstance()->GetKeyboardTracker();
	auto state = keytrack.GetLastState();

	// ジャンプ
	if (keytrack.IsKeyPressed(Keyboard::Space) &&
		mpOwner->NowIsOnTheGround())
	{
		mpOwner->Jump();
	}

	// WASDキーの入力状態を設定
	const bool wasd[] = { state.W,	state.A, state.S, state.D };
	// キーに対応した角度
	const int angles[] = { 0, 90, 180, 270 };
	// いくつのキーを同時押ししているか
	int multiPushCount = std::count(wasd, wasd + sizeof(wasd), true);

	// 押していないor３つ以上押している場合は無視。
	if (multiPushCount == 0 || multiPushCount >= 3) {
		mpOwner->SetSlipFlag(false);// 滑らないように
		return;
	}
	else {
		mpOwner->SetSlipFlag(true);// 滑るように
	}

	// 押しているキーを元に角度を決めていく
	int inputAngle = 0;
	for (int i = 0; i < 4; i++) {
		inputAngle += angles[i] * (int)(wasd[i]);
	}

	// WDの場合（右上入力）の場合は360度足して計算の辻褄を合わせる
	inputAngle += ((int)(wasd[0]) * (int)(wasd[3]) * 360);
	// WSの場合（上下同時）の場合は値を０にするため辻褄を合わせる
	inputAngle -= ((int)(wasd[0]) * (int)(wasd[2]) * 180);
	// 単発入力の場合はそのままの値、同時入力の場合は間を取るため割る。
	inputAngle /= multiPushCount;

	// 角度を滑らかに変更する
	myf::RubberComplementAngleAdjustmentCustom(mMoveWayAngle, XMConvertToRadians((float)(inputAngle)));

	// カメラの向いている方向を取得する
	Vector3 frontWay = thf::GetLocation(mpOwner) - mpCamera->GetEyePosition();
	// 上下の値は無視する。
	frontWay.y = 0.0f;
	frontWay.Normalize();

	// キー入力に応じた角度値に応じて正面ベクトルから回転させる
	Matrix moveMat = Matrix::CreateTranslation(frontWay);
	moveMat *= Matrix::CreateRotationY(mMoveWayAngle);
	// キー入力を適応させた値に設定
	frontWay = moveMat.Translation();
	// オーナーの速度変数を変更
	mpOwner->AddForce(frontWay * 0.5f);
}

//=====================================================
//		終了処理
//=====================================================
void Player_MoveState::Finalize()
{
}

