#include "pch.h"
#include "CameraManager.h"
#include "ControlCamera.h"
#include "Camera.h"

//#define DEBUG_MODE

//=====================================================
//		コンストラクタ
//=====================================================
CameraManager::CameraManager()
	:ComponentManager()
	,mpActiveCamera(nullptr)
	,mpDebugCamera()
	,mpControlCamera(nullptr)
{
	// デバッグカメラの生成
	mpDebugCamera = std::make_unique<DebugCamera>();

	// activeカメラを変更(特殊カメラが生成されるまで用)
	mpActiveCamera = mpDebugCamera.get();
}

//=====================================================
//		更新
//=====================================================
void CameraManager::Update()
{
	mpDebugCamera->Update();

	for (auto& cam : mpMembers) {
		cam->Update();
	}
}

//=====================================================
//		描画
//=====================================================
void CameraManager::Render()
{
}

//=====================================================
//		ControlCameraを生成、取得
//=====================================================
ControlCamera* CameraManager::SpawnControlCamera(Actor* pOwner)
{
	assert(mpControlCamera == nullptr && "コントロールカメラはすでに生成されています。");

	// コントロールカメラ生成
	mpControlCamera = ControlCameraMaker::SpawnNewControlCamera(pOwner);

	// 活動カメラを変更
	mpActiveCamera = mpControlCamera;

	return mpControlCamera;
}

//=====================================================
//		ビュー行列取得
//=====================================================
const DirectX::SimpleMath::Matrix& CameraManager::GetViewMatrix()
{
#ifdef DEBUG_MODE
	return mpDebugCamera->GetViewMatrix();
#endif // DEBUG_MODE

#ifndef DEBUG_MODE
	return mpActiveCamera->GetViewMatrix();
#endif // !DEBUG_MODE
}

//=====================================================
//		プロジェクション行列取得
//=====================================================
const DirectX::SimpleMath::Matrix& CameraManager::GetProjectionMatrix()
{
#ifdef DEBUG_MODE
	return mpDebugCamera->GetProjectionMatrix();
#endif // DEBUG_MODE

#ifndef DEBUG_MODE
	return mpActiveCamera->GetProjectionMatrix();
#endif // !DEBUG_MODE
}
