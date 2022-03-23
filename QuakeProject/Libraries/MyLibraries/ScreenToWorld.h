/**
 * @file ScreenToWorld.h
 * @brief スクリーン座標をワールド座標へ変換するための関数たち
 * @date 2022/03/23
 */
#pragma once
#include "pch.h"
#include "DeviceResources.h"
#include "Game/Camera/CameraManager.h"
#include "Game/UtilityManager/InputManager.h"

namespace MyFunctions {

	/**
	 * @brief	スクリーン座標をワールド座標へ変換する際に使用する行列を作成
	 */
	static DirectX::SimpleMath::Matrix CreateMatrix_Screen2World()
	{
		using namespace DirectX::SimpleMath;

		// ビューポートから画面サイズを取得する
		D3D11_VIEWPORT viewport = DX::DeviceResources::GetInstance()->GetScreenViewport();
		float screenW = viewport.Width;
		float screenH = viewport.Height;

		// ビューポートスケーリング行列を作成
		Matrix viewportScalingMat;
		viewportScalingMat._11 = screenW / 2.0f;
		viewportScalingMat._22 = -screenH / 2.0f;
		viewportScalingMat._41 = screenW / 2.0f;
		viewportScalingMat._42 = screenH / 2.0f;

		// ビュー行列と射影行列を取得
		CameraManager* cam = CameraManager::GetInstance();
		Matrix view = cam->GetViewMatrix();
		Matrix proj = cam->GetProjectionMatrix();

		// 逆行列を作成
		Matrix invS = viewportScalingMat.Invert();
		Matrix invP = proj.Invert();
		Matrix invV = view.Invert();

		// 『ビューポートスケーリング行列の逆行列』 × 『射影行列の逆行列』 × 『ビュー行列の逆行列』
		return invS * invP * invV;
	}

	/**
	 * @brief	カメラの座標から (ワールド座標へ変換した)マウスの座標を指し示すベクトル(始点と終点)を計算する
	 * @param[out] start	始点
	 * @param[out] end		終点
	 */
	static void CalculateStartandEndPointsFromMousePos(DirectX::SimpleMath::Vector3& start, DirectX::SimpleMath::Vector3& end) {
		using namespace DirectX;
		using namespace DirectX::SimpleMath;

		// 変換行列を生成
		Matrix screen2worldMat = CreateMatrix_Screen2World();

		// マウス座標を取得し前方クリップ面と後方クリップ面用の値を設定する
		Mouse::State mouse = InputManager::GetInstance()->GetMouseTracker().GetLastState();
		// (一瞬入れ元として使う)
		start = Vector3(float(mouse.x), float(mouse.y), 0.0f);
		end = Vector3(float(mouse.x), float(mouse.y), 1.0f);

		// 始点と終点それぞれをワールド座標へ変換
		start = Vector3::Transform(start, screen2worldMat);
		end = Vector3::Transform(end, screen2worldMat);
	}
}

namespace myf = MyFunctions;