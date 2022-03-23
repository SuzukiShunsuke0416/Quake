/**
 * @file PlayScene.cpp
 * @brief �v���C�V�[��
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
//		�R���X�g���N�^
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
	// �X�e�[�W�̐���
	mpStageManager = StageManager::GetInstance();
	// �J�����}�l�[�W���[�̐���
	mpCameraManager = CameraManager::GetInstance();
	// �F�}�l�̐���
	mpColorManager = ElevationColorManager::GetInstance();

	// �L�����}�l�̐���
	mpCharacterManager = CharacterManager::GetInstance();

	// �R���}�l�̐���
	mpCollisionManager = CollisionManager::GetInstance();

	// �������s�}�l�̐���
	mpAutoDriveManager = AutoDriveManager::GetInstance();

	// �V�[����BGM�J�n
	SoundManager::GetInstance()->PlaySoundFadeIn(eSoundType::PlaySceneBGM, true);
}

//=====================================================
//		�f�X�g���N�^
//=====================================================
PlayScene::~PlayScene()
{
	mpColorManager->SingletonKill();
	mpCharacterManager->SingletonKill();
	mpAutoDriveManager->SingletonKill();
	mpCameraManager->SingletonKill();
	mpStageManager->SingletonKill();
	mpCollisionManager->SingletonKill();
	// �V�[����BGM�I��
	SoundManager::GetInstance()->StopSoundFadeOut(eSoundType::PlaySceneBGM);
}

//=====================================================
//		������
//=====================================================
void PlayScene::Initialize()
{
	// �V�[���`�F���W���[�����������Ă���
	mSceneChangeFader.Initialize(eSceneChangeFadeType::Normal);

	// �L�����}�l�̐���
	mpCharacterManager->Initialize();

	// �X�e�[�W�f�[�^�̏�����
	mpStageManager->Initialize();

	// �F�}�l�[�W���̏�����
	mpColorManager->Initialize();

	// �������s�}�l�̏�����
	mpAutoDriveManager->Initialize();
}

//=====================================================
//		�X�V
//=====================================================
void PlayScene::Update()
{
	//DebugImgManager::GetInstance()->Entry(eDebugImgType::Full2);
	DebugLogManager::GetInstance()->EntryLog(L"PlayScene");

	// �J�����}�l�[�W���̍X�V
	mpCameraManager->Update();

	// �F���}�l�[�W���̍X�V
	mpColorManager->Update();

	// �X�e�[�W�̍X�V
	mpStageManager->Update();

	//�L�����N�^�[�}�l�[�W���̍X�V
	mpCharacterManager->Update();

	// �R���}�l�̍X�V
	mpCollisionManager->Update();

	// �������s�}�l�̍X�V
	mpAutoDriveManager->Update();

	const auto& keytrack = InputManager::GetInstance()->GetKeyboardTracker();
	if (keytrack.IsKeyPressed(DirectX::Keyboard::Keys::Q)) {
		SceneManager::GetInstance()->ChangeScene(eSceneType::Title);
	}

	// �f�o�b�O�I�u�W�F�̍X�V
	mDebugActor.Update();
}

//=====================================================
//		�`��
//=====================================================
void PlayScene::Render()
{
	//DebugImgManager::GetInstance()->QuickDraw(eDebugImgType::Full2);


	// �f�o�b�O�I�u�W�F�\��
	auto& view = mpCameraManager->GetViewMatrix();
	auto& proj = mpCameraManager->GetProjectionMatrix();
	DebugObjManager::GetInstance()->Render(view, proj);


	// �X�e�[�W�̕`��
	mpStageManager->Render();

	//�L�����N�^�[�}�l�[�W���̕`��
	mpCharacterManager->Render();

	// �������s�}�l�̕`��
	mpAutoDriveManager->Render();

	// �f�o�b�O�I�u�W�F�̕`��
	mDebugActor.Render(); // �X�e�[�W�̌�ɌĂԂƃX�e�[�W�����B���
}

//=====================================================
//		�I������
//=====================================================
void PlayScene::Finalize()
{
	// �}�l�[�W���̏I������
	mpStageManager->Finalize();
}

//=====================================================
//		�t�F�[�h�J�n
//=====================================================
void PlayScene::OnFadeStart()
{
	//! �t�F�[�h���n�܂�u�ԂɌ��ʉ���炷
	SoundManager::GetInstance()->GetSound(eSoundType::TitleToPlay)->Play();
}

//=====================================================
//		�t�F�[�h�I��
//=====================================================
void PlayScene::OnFadeEnd()
{
}

//=====================================================
//		�t�F�[�h�X�V�֐�
//=====================================================
bool PlayScene::FadeUpdate()
{
	return mSceneChangeFader.Update();
}

//=====================================================
//		�t�F�[�h�`��֐�
//=====================================================
void PlayScene::FadeRender(ID3D11ShaderResourceView* srv)
{
	mSceneChangeFader.Draw(srv);
}