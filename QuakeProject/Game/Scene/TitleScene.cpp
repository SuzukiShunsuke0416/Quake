/**
 * @file TitleScene.cpp
 * @brief �^�C�g���V�[��
 * @author suzuki.shunsuke
 * @date 2022/01/29
 */
#include "pch.h"
#include "TitleScene.h"
#include "Game/Scene/SceneManager.h"
#include "Game/UtilityManager/InputManager.h"

 //=====================================================
 //		�R���X�g���N�^
 //=====================================================
TitleScene::TitleScene()
	:IScene()
{
	SoundManager::GetInstance()->PlaySoundFadeIn(eSoundType::TitleSceneBGM, true);
}

//=====================================================
//		�f�X�g���N�^
//=====================================================
TitleScene::~TitleScene()
{
	SoundManager::GetInstance()->StopSoundFadeOut(eSoundType::TitleSceneBGM);
}

//=====================================================
//		������
//=====================================================
void TitleScene::Initialize()
{
	mSceneChangeFader.Initialize(eSceneChangeFadeType::Normal);
}

//=====================================================
//		�X�V
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
//		�`��
//=====================================================
void TitleScene::Render()
{
	DebugImgManager::GetInstance()->QuickDraw(eDebugImgType::Full1);
}

//=====================================================
//		�I������
//=====================================================
void TitleScene::Finalize()
{
}

//=====================================================
//		�t�F�[�h�J�n
//=====================================================
void TitleScene::OnFadeStart()
{
}

//=====================================================
//		�t�F�[�h�I��
//=====================================================
void TitleScene::OnFadeEnd()
{
}

//=====================================================
//		�t�F�[�h�X�V�֐�
//=====================================================
bool TitleScene::FadeUpdate()
{
	return mSceneChangeFader.Update();
}

//=====================================================
//		�t�F�[�h�`��֐�
//=====================================================
void TitleScene::FadeRender(ID3D11ShaderResourceView* srv)
{
	mSceneChangeFader.Draw(srv);
}

