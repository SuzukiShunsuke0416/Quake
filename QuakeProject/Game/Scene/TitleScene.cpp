/**
 * @file TitleScene.cpp
 * @brief タイトルシーン
 * @author suzuki.shunsuke
 * @date 2022/01/29
 */
#include "pch.h"
#include "TitleScene.h"
#include "Game/Scene/SceneManager.h"
#include "Game/UtilityManager/InputManager.h"

 //=====================================================
 //		コンストラクタ
 //=====================================================
TitleScene::TitleScene()
	:IScene()
{
	SoundManager::GetInstance()->PlaySoundFadeIn(eSoundType::TitleSceneBGM, true);
}

//=====================================================
//		デストラクタ
//=====================================================
TitleScene::~TitleScene()
{
	SoundManager::GetInstance()->StopSoundFadeOut(eSoundType::TitleSceneBGM);
}

//=====================================================
//		初期化
//=====================================================
void TitleScene::Initialize()
{
	mSceneChangeFader.Initialize(eSceneChangeFadeType::Normal);
}

//=====================================================
//		更新
//=====================================================
void TitleScene::Update()
{
	//DebugImgManager::GetInstance()->Entry(eDebugImgType::Full1);
	DebugLogManager::GetInstance()->EntryLog(L"TitleScene");

	auto& keytrack = InputManager::GetInstance()->GetKeyboardTracker();

	
	if (keytrack.IsKeyPressed(DirectX::Keyboard::Keys::Space)) {
		SceneManager::GetInstance()->ChangeScene(eSceneType::Play);
	}

}

//=====================================================
//		描画
//=====================================================
void TitleScene::Render()
{
	DebugImgManager::GetInstance()->QuickDraw(eDebugImgType::Full1);
}

//=====================================================
//		終了処理
//=====================================================
void TitleScene::Finalize()
{
}

//=====================================================
//		フェード開始
//=====================================================
void TitleScene::OnFadeStart()
{
}

//=====================================================
//		フェード終了
//=====================================================
void TitleScene::OnFadeEnd()
{
}

//=====================================================
//		フェード更新関数
//=====================================================
bool TitleScene::FadeUpdate()
{
	return mSceneChangeFader.Update();
}

//=====================================================
//		フェード描画関数
//=====================================================
void TitleScene::FadeRender(ID3D11ShaderResourceView* srv)
{
	mSceneChangeFader.Draw(srv);
}

