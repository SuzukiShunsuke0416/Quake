/**
 * @file SceneManager.cpp
 * @brief �e�V�[�����Ǘ�����N���X
 * @author suzuki.shunsuke
 * @date 2022/01/29
 */
#include "pch.h"
#include "SceneManager.h"

#include "TitleScene.h"
#include "PlayScene.h"
#include "DeviceResources.h"
#include "Game/UtilityManager/InputManager.h"
#include "Game/DataBase/ExternalDatabase.h"
#include "Game.h"

const int SceneManager::MAX_FUNCTION_COUNT = 2;

//=====================================================
//		�R���X�g���N�^
//=====================================================
SceneManager::SceneManager()
	:mElapsedTime(0.0f)
	,mpActiveScene(nullptr)
	,mpStandByScene(nullptr)
	,mCurrentUpdates{}
	,mCurrentRenders{}
	,mpCurrentRTV(nullptr)
{
	// ����RTV�����ݒ�
	mpCurrentRTV = DX::DeviceResources::GetInstance()->GetRenderTargetView();

	// �O���t�@�C���ǂݍ���
	ExternalDatabase::GetInstance()->Initialize();

	// �T�E���h�}�l�[�W���̏�����
	SoundManager::GetInstance()->Initialize();
}

//=====================================================
//		�f�X�g���N�^
//=====================================================
SceneManager::~SceneManager()
{
	// �A�N�e�B�u�V�[�����c���Ă����ꍇ�͍폜
	if (mpActiveScene) {
		mpActiveScene->Finalize();
		delete mpActiveScene;
		mpActiveScene = nullptr;
	}
	// �X�^���o�C�V�[�����c���Ă����ꍇ�͍폜
	if (mpStandByScene) {
		mpStandByScene->Finalize();
		delete mpStandByScene;
		mpStandByScene = nullptr;
	}

	// (�S�V�[���Ŏg����)�e�}�l�[�W���B�̉��
	DebugObjManager::GetInstance()->SingletonKill();
	DebugLogManager::GetInstance()->SingletonKill();
	DebugImgManager::GetInstance()->SingletonKill();
	InputManager::GetInstance()->SingletonKill();
	SoundManager::GetInstance()->SingletonKill();
	ExternalDatabase::GetInstance()->SingletonKill();
}

//=====================================================
//		������
//=====================================================
void SceneManager::Initialize()
{
	// �Q�[���N�����ɂ���V�[���𐶐�
	mpActiveScene = new TitleScene();
	
	//�ʏ폈����ǉ�
	this->AddFuncActive();

	//�t�F�[�h�pTexture�Ȃǂ�ݒ�
	this->InitFadeTextures();
}

//=====================================================
//		�X�V
//=====================================================
void SceneManager::Update(DX::StepTimer const& timer)
{
	mElapsedTime = float(timer.GetElapsedSeconds());

	// ���͌n�}�l�[�W���̍X�V
	InputManager::GetInstance()->Update();

	// �T�E���h�}�l�[�W���̍X�V
	SoundManager::GetInstance()->Update();

	// �f�o�b�O���O��fps��o�^
	DebugLogManager::GetInstance()->EntryLog((int)(timer.GetFramesPerSecond()), L"fps");

	// ���ݒǉ�����Ă���X�V�������ꊇ�ŌĂ�
	for (auto& update : mCurrentUpdates) {
		update(this);
	}

	// esc�L�[�������ꂽ��A�Q�[�����I������
	auto& keyboard = InputManager::GetInstance()->GetKeyboardTracker();
	if (keyboard.IsKeyReleased(DirectX::Keyboard::Escape)) {
		PostQuitMessage(0);
	}
}

//=====================================================
//		�`��
//=====================================================
void SceneManager::Render()
{
	// ���ݒǉ�����Ă���`�揈�����ꊇ�ŌĂ�
	for (auto& render : mCurrentRenders) {
		render(this);
	}

	//�f�o�b�O�\���̕`��
	DebugImgManager::GetInstance()->Draw();
	DebugLogManager::GetInstance()->Draw();
}

//=====================================================
//		�V�[���ύX
//=====================================================
void SceneManager::ChangeScene(eSceneType nextScene)
{
	// �����Ɏw�肳�ꂽ�V�[���𐶐�����
	switch (nextScene)
	{
		case eSceneType::Title:

			//���łɃX�^���o�C�V�[������������Ă����疳������
			if (mpStandByScene)return;
			//�^�C�g���V�[���̐���
			mpStandByScene = new TitleScene();
			mpStandByScene->Initialize();

			break;

		case eSceneType::Play:

			//���łɃX�^���o�C�V�[������������Ă����疳������
			if (mpStandByScene)return;
			//�v���C�V�[���̐���
			mpStandByScene = new PlayScene();
			mpStandByScene->Initialize();

			break;

		default:
			assert(!"ChangeScene�@�Ή��O�^�C�v���N��");
			break;
	}

	// �X�^���o�C��Ԃ̃V�[�����������J�n������
	this->AddFuncStandBy();

	// �t�F�[�h���J�n����邱�Ƃ�active,standby���ɓ`����
	mpActiveScene->OnFadeStart();
	mpStandByScene->OnFadeStart();
}

//=====================================================
//		�A�N�e�B�u�V�[���X�V
//=====================================================
void SceneManager::UpdataActiveScene()
{
	mpActiveScene->Update();
}

//=====================================================
//		�A�N�e�B�u�V�[���`��
//=====================================================
void SceneManager::RenderActiveScene()
{
	mpActiveScene->Render();
}

//=====================================================
//		�X�^���o�C�V�[���X�V
//=====================================================
void SceneManager::UpdateStandByScene()
{
	mpStandByScene->Update();
	bool endFlag = mpStandByScene->FadeUpdate();

	// �t�F�[�h�����������ꍇ
	if (endFlag == true) {
		// �X�^���o�C�n�֐���z�񂩂甲��
		this->RemoveFuncStandBy();

		// �t�F�[�h���I��������Ƃ�`����
		mpActiveScene->OnFadeEnd();
		mpStandByScene->OnFadeEnd();

		// �t�F�[�h���I���S�Ă��\�����ꂽ
		// �V�[�����A�N�e�B�u�V�[���Ƃ��Ĉ����B
		std::swap(mpActiveScene, mpStandByScene);

		// �O�A�N�e�B�u�V�[�����폜����
		mpStandByScene->Finalize();
		delete mpStandByScene;
		mpStandByScene = nullptr;
	}
}

//=====================================================
//		�X�^���o�C�V�[���`��
//=====================================================
void SceneManager::RenderStandByScene()
{
	// �f�o�C�X���\�[�X���擾
	auto pDR = DX::DeviceResources::GetInstance();
	auto context = pDR->GetD3DDeviceContext();
	auto renderTarget = pDR->GetRenderTargetView();
	auto depthStencil = pDR->GetDepthStencilView();

	// �p�ӂ����e�N�X�`���Ƀ����_�����O
	pDR->GetD3DDeviceContext()->OMSetRenderTargets(
		1,
		mFadeImageRenderTargetView.GetAddressOf(),
		depthStencil
	);
	mpCurrentRTV = mFadeImageRenderTargetView.Get();

	// �X�^���o�C�V�[���`��i�ޔ�prtv�Ɂj
	mpStandByScene->Render();

	// �����_�[�^�[�Q�b�g�����ɖ߂�
	pDR->GetD3DDeviceContext()->OMSetRenderTargets(
		1,
		&renderTarget,
		depthStencil
	);
	mpCurrentRTV = renderTarget;

	// �t�F�[�h��K�p�����ĕ`��i��ʂɁj
	mpStandByScene->FadeRender(mFadeImageShaderResourceView.Get());

	// ���O�����_�[�^�[�Q�b�g�r���[���N���A
	context->ClearRenderTargetView(
		mFadeImageRenderTargetView.Get(),
		ConstDataBase::BackGroundColor
	);
}


//=====================================================
//		�����֐��ǉ�(active)
//=====================================================
void SceneManager::AddFuncActive()
{
	if (mCurrentUpdates.size() != 0)assert(!"�X�V�֐����]���ɒǉ�����Ă��܂�");
	if (mCurrentRenders.size() != 0)assert(!"�`��֐����]���ɒǉ�����Ă��܂�");

	mCurrentUpdates.push_back(
		std::mem_fn(&SceneManager::UpdataActiveScene));
	mCurrentRenders.push_back(
		std::mem_fn(&SceneManager::RenderActiveScene));
}

//=====================================================
//		�����֐��ǉ�(stand by)
//=====================================================
void SceneManager::AddFuncStandBy()
{
	if (mpStandByScene == nullptr) {
		assert(!"mpStandByScene�ɃV�[�����ݒ肳��Ă��܂���");
	}

	if (mCurrentUpdates.size() >= MAX_FUNCTION_COUNT)assert(!"�X�V�֐����]���ɒǉ�����Ă��܂�");
	if (mCurrentRenders.size() >= MAX_FUNCTION_COUNT)assert(!"�`��֐����]���ɒǉ�����Ă��܂�");

	mCurrentUpdates.push_back(
		std::mem_fn(&SceneManager::UpdateStandByScene));
	mCurrentRenders.push_back(
		std::mem_fn(&SceneManager::RenderStandByScene));
}

//=====================================================
//		�����֐����O(stand by)
//=====================================================
void SceneManager::RemoveFuncStandBy()
{
	// �X�^���o�C�X�V�֐��𔲂�
	if (mCurrentUpdates.size() == MAX_FUNCTION_COUNT) {
		mCurrentUpdates.pop_back();
	}

	// �X�^���o�C�`��֐��𔲂�
	if (mCurrentRenders.size() == MAX_FUNCTION_COUNT) {
		mCurrentRenders.pop_back();
	}
}

//=====================================================
//		�t�F�[�h�pTexture�Ȃǂ�ݒ�
//=====================================================
void SceneManager::InitFadeTextures()
{
	// �f�o�C�X���\�[�X���擾
	auto pDR = DX::DeviceResources::GetInstance();

	// �����_�����O�^�[�Q�b�g�p�̃e�N�X�`�����쐬
	D3D11_TEXTURE2D_DESC texDesc;
	memset(&texDesc, 0, sizeof(texDesc));
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.Width = pDR->GetOutputSize().right;
	texDesc.Height = pDR->GetOutputSize().bottom;
	texDesc.CPUAccessFlags = 0;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	pDR->GetD3DDevice()->CreateTexture2D(&texDesc, NULL, mTexture2D.GetAddressOf());

	// �����_�[�^�[�Q�b�g�r���[�̐���
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	pDR->GetD3DDevice()->CreateRenderTargetView(mTexture2D.Get(), &rtvDesc, mFadeImageRenderTargetView.GetAddressOf());

	// �V�F�[�_�[���\�[�X�r���[�̐���
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	memset(&srvDesc, 0, sizeof(srvDesc));
	srvDesc.Format = rtvDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	pDR->GetD3DDevice()->CreateShaderResourceView(mTexture2D.Get(), &srvDesc, mFadeImageShaderResourceView.GetAddressOf());
}


/* 
	�`�V�[���J�ڂ̗���`

	�@�V�[���ύX���߂����̃N���X�ɓ͂��B
	�A���V�[�����쐬����B
	�B�O�V�[���̕`��̏�ɁA�t�F�[�h�����������������V�[�����`�悳���
	�C�t�F�[�h���I�������O�V�[�����폜����A���V�[�����P�̏��������悤�ɂȂ�B
*/