/**
 * @file ScreenToWorld.h
 * @brief �X�N���[�����W�����[���h���W�֕ϊ����邽�߂̊֐�����
 * @date 2022/03/23
 */
#pragma once
#include "pch.h"
#include "DeviceResources.h"
#include "Game/Camera/CameraManager.h"
#include "Game/UtilityManager/InputManager.h"

namespace MyFunctions {

	/**
	 * @brief	�X�N���[�����W�����[���h���W�֕ϊ�����ۂɎg�p����s����쐬
	 */
	static DirectX::SimpleMath::Matrix CreateMatrix_Screen2World()
	{
		using namespace DirectX::SimpleMath;

		// �r���[�|�[�g�����ʃT�C�Y���擾����
		D3D11_VIEWPORT viewport = DX::DeviceResources::GetInstance()->GetScreenViewport();
		float screenW = viewport.Width;
		float screenH = viewport.Height;

		// �r���[�|�[�g�X�P�[�����O�s����쐬
		Matrix viewportScalingMat;
		viewportScalingMat._11 = screenW / 2.0f;
		viewportScalingMat._22 = -screenH / 2.0f;
		viewportScalingMat._41 = screenW / 2.0f;
		viewportScalingMat._42 = screenH / 2.0f;

		// �r���[�s��Ǝˉe�s����擾
		CameraManager* cam = CameraManager::GetInstance();
		Matrix view = cam->GetViewMatrix();
		Matrix proj = cam->GetProjectionMatrix();

		// �t�s����쐬
		Matrix invS = viewportScalingMat.Invert();
		Matrix invP = proj.Invert();
		Matrix invV = view.Invert();

		// �w�r���[�|�[�g�X�P�[�����O�s��̋t�s��x �~ �w�ˉe�s��̋t�s��x �~ �w�r���[�s��̋t�s��x
		return invS * invP * invV;
	}

	/**
	 * @brief	�J�����̍��W���� (���[���h���W�֕ϊ�����)�}�E�X�̍��W���w�������x�N�g��(�n�_�ƏI�_)���v�Z����
	 * @param[out] start	�n�_
	 * @param[out] end		�I�_
	 */
	static void CalculateStartandEndPointsFromMousePos(DirectX::SimpleMath::Vector3& start, DirectX::SimpleMath::Vector3& end) {
		using namespace DirectX;
		using namespace DirectX::SimpleMath;

		// �ϊ��s��𐶐�
		Matrix screen2worldMat = CreateMatrix_Screen2World();

		// �}�E�X���W���擾���O���N���b�v�ʂƌ���N���b�v�ʗp�̒l��ݒ肷��
		Mouse::State mouse = InputManager::GetInstance()->GetMouseTracker().GetLastState();
		// (��u���ꌳ�Ƃ��Ďg��)
		start = Vector3(float(mouse.x), float(mouse.y), 0.0f);
		end = Vector3(float(mouse.x), float(mouse.y), 1.0f);

		// �n�_�ƏI�_���ꂼ������[���h���W�֕ϊ�
		start = Vector3::Transform(start, screen2worldMat);
		end = Vector3::Transform(end, screen2worldMat);
	}
}

namespace myf = MyFunctions;