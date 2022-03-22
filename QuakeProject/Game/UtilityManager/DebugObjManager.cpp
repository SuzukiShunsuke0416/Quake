/**
 * @file DebugObjManager.cpp
 * @brief 3D�I�u�W�F�N�g��`�悷���p�̃}�l�[�W���[
 * @author suzuki.shunsuke
 * @date 2022/02/27
 */
#include "pch.h"
#include "DebugObjManager.h"
#include "DeviceResources.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//=====================================================
//		�R���X�g���N�^
//=====================================================
DebugObjManager::DebugObjManager()
	:mSphereDataVec()
	,mCubeDataVec()
	,mGeoPri()
	,mGeoPriCube()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	auto context = pDR->GetD3DDeviceContext();
	mGeoPri = GeometricPrimitive::CreateGeoSphere(context);
	mGeoPriCube = GeometricPrimitive::CreateCube(context);
	mGeoPriTeaPot = GeometricPrimitive::CreateTeapot(context);
}

//=====================================================
//		�`��
//=====================================================
void DebugObjManager::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj)
{
	// �X�t�B�A�̕`��
	for (const DebugObjData_Sphere& data : mSphereDataVec) {
		mGeoPri->Draw(
			Matrix::CreateScale(data.scale) *
			Matrix::CreateTranslation(data.pos),
			view,
			proj,
			Vector4(data.color.x, data.color.y, data.color.z, 1.0f)
		);
	}

	// �L���[�u�̕`��
	for (const DebugObjData_Cube& data : mCubeDataVec) {
		mGeoPriCube->Draw(
			data.world,
			view,
			proj,
			Vector4(data.color.x, data.color.y, data.color.z, 1.0f)
		);
	}

	// �e�B�[�J�b�v�̕`��
	for (const DebugObjData_TeaPot& data : mTeaPotDataVec) {
		mGeoPriTeaPot->Draw(
			data.world,
			view,
			proj,
			Vector4(data.color.x, data.color.y, data.color.z, 1.0f)
		);
	}

	// �z������
	this->Clear();
}