#include "pch.h"
#include "CameraManager.h"
#include "ControlCamera.h"
#include "Camera.h"

//#define DEBUG_MODE

//=====================================================
//		�R���X�g���N�^
//=====================================================
CameraManager::CameraManager()
	:ComponentManager()
	,mpActiveCamera(nullptr)
	,mpDebugCamera()
	,mpControlCamera(nullptr)
{
	// �f�o�b�O�J�����̐���
	mpDebugCamera = std::make_unique<DebugCamera>();

	// active�J������ύX(����J���������������܂ŗp)
	mpActiveCamera = mpDebugCamera.get();
}

//=====================================================
//		�X�V
//=====================================================
void CameraManager::Update()
{
	mpDebugCamera->Update();

	for (auto& cam : mpMembers) {
		cam->Update();
	}
}

//=====================================================
//		�`��
//=====================================================
void CameraManager::Render()
{
}

//=====================================================
//		ControlCamera�𐶐��A�擾
//=====================================================
ControlCamera* CameraManager::SpawnControlCamera(Actor* pOwner)
{
	assert(mpControlCamera == nullptr && "�R���g���[���J�����͂��łɐ�������Ă��܂��B");

	// �R���g���[���J��������
	mpControlCamera = ControlCameraMaker::SpawnNewControlCamera(pOwner);

	// �����J������ύX
	mpActiveCamera = mpControlCamera;

	return mpControlCamera;
}

//=====================================================
//		�r���[�s��擾
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
//		�v���W�F�N�V�����s��擾
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
