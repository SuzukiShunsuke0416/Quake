/**
 * @file PlayScene.cpp
 * @brief プレイシーン
 * @author suzuki.shunsuke
 * @date 2022/01/29
 */
#include "pch.h"
#include "PlayScene.h"
#include "DeviceResources.h"
#include "Game/Shader/BinaryFile.h"
#include "Libraries/MyLibraries/Utility.h"
#include "Game/Scene/SceneManager.h"
#include "Game/UtilityManager/InputManager.h"
#include "Game/Camera/CameraManager.h"
#include "Game/ElevationColor/ElevationColorManager.h"
#include "Game/Stage/StageManager.h"
#include "Game/Character/CharacterManager.h"
#include "Game/Collision/CollisionManager.h"
#include "Game/UtilityManager/AutoDriveManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//=====================================================
//		コンストラクタ
//=====================================================
PlayScene::PlayScene()
	:IScene()
	, mSceneChangeFader()
	, mpStageManager()
	, mpCameraManager()
	, mpColorManager()
	, mpCharacterManager()
	, mpCollisionManager()
{
	// ステージの生成
	mpStageManager = StageManager::GetInstance();
	// カメラマネージャーの生成
	mpCameraManager = CameraManager::GetInstance();
	// 色マネの生成
	mpColorManager = ElevationColorManager::GetInstance();

	// キャラマネの生成
	mpCharacterManager = CharacterManager::GetInstance();

	// コリマネの生成
	mpCollisionManager = CollisionManager::GetInstance();

	// 自動走行マネの生成
	mpAutoDriveManager = AutoDriveManager::GetInstance();

	// シーン内BGM開始
	SoundManager::GetInstance()->PlaySoundFadeIn(eSoundType::PlaySceneBGM, true);
}

//=====================================================
//		デストラクタ
//=====================================================
PlayScene::~PlayScene()
{
	mpColorManager->SingletonKill();
	mpCharacterManager->SingletonKill();
	mpAutoDriveManager->SingletonKill();
	mpCameraManager->SingletonKill();
	mpStageManager->SingletonKill();
	mpCollisionManager->SingletonKill();
	// シーン内BGM終了
	SoundManager::GetInstance()->StopSoundFadeOut(eSoundType::PlaySceneBGM);
}

//=====================================================
//		初期化
//=====================================================
void PlayScene::Initialize()
{
	// シーンチェンジャーを初期化しておく
	mSceneChangeFader.Initialize(eSceneChangeFadeType::Normal);

	// キャラマネの生成
	mpCharacterManager->Initialize();

	// ステージデータの初期化
	mpStageManager->Initialize();

	// 色マネージャの初期化
	mpColorManager->Initialize();

	// 自動走行マネの初期化
	mpAutoDriveManager->Initialize();
}

//=====================================================
//		更新
//=====================================================
void PlayScene::Update()
{
	//DebugImgManager::GetInstance()->Entry(eDebugImgType::Full2);
	DebugLogManager::GetInstance()->EntryLog(L"PlayScene");

	// カメラマネージャの更新
	mpCameraManager->Update();

	// 色情報マネージャの更新
	mpColorManager->Update();

	// ステージの更新
	mpStageManager->Update();

	//キャラクターマネージャの更新
	mpCharacterManager->Update();

	// コリマネの更新
	mpCollisionManager->Update();

	// 自動走行マネの更新
	mpAutoDriveManager->Update();

	const auto& keytrack = InputManager::GetInstance()->GetKeyboardTracker();
	if (keytrack.IsKeyPressed(DirectX::Keyboard::Keys::Q)) {
		SceneManager::GetInstance()->ChangeScene(eSceneType::Title);
	}

	// デバッグオブジェの更新
	mDebugActor.Update();
}

//=====================================================
//		描画
//=====================================================
void PlayScene::Render()
{
	//DebugImgManager::GetInstance()->QuickDraw(eDebugImgType::Full2);


	// デバッグオブジェ表示
	auto& view = mpCameraManager->GetViewMatrix();
	auto& proj = mpCameraManager->GetProjectionMatrix();
	DebugObjManager::GetInstance()->Render(view, proj);


	// ステージの描画
	mpStageManager->Render();

	//キャラクターマネージャの描画
	mpCharacterManager->Render();

	// 自動走行マネの描画
	mpAutoDriveManager->Render();

	// デバッグオブジェの描画
	mDebugActor.Render(); // ステージの後に呼ぶとステージ線が隠れる
}

//=====================================================
//		終了処理
//=====================================================
void PlayScene::Finalize()
{
	// マネージャの終了処理
	mpStageManager->Finalize();
}

//=====================================================
//		フェード開始
//=====================================================
void PlayScene::OnFadeStart()
{
	//! フェードが始まる瞬間に効果音を鳴らす
	SoundManager::GetInstance()->GetSound(eSoundType::TitleToPlay)->Play();
}

//=====================================================
//		フェード終了
//=====================================================
void PlayScene::OnFadeEnd()
{
}

//=====================================================
//		フェード更新関数
//=====================================================
bool PlayScene::FadeUpdate()
{
	return mSceneChangeFader.Update();
}

//=====================================================
//		フェード描画関数
//=====================================================
void PlayScene::FadeRender(ID3D11ShaderResourceView* srv)
{
	mSceneChangeFader.Draw(srv);
}