/**
 * @file DebugObjManager.cpp
 * @brief 3Dオブジェクトを描画する専用のマネージャー
 * @author suzuki.shunsuke
 * @date 2022/02/27
 */
#include "pch.h"
#include "DebugObjManager.h"
#include "DeviceResources.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//=====================================================
//		コンストラクタ
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
//		描画
//=====================================================
void DebugObjManager::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj)
{
	// スフィアの描画
	for (const DebugObjData_Sphere& data : mSphereDataVec) {
		mGeoPri->Draw(
			Matrix::CreateScale(data.scale) *
			Matrix::CreateTranslation(data.pos),
			view,
			proj,
			Vector4(data.color.x, data.color.y, data.color.z, 1.0f)
		);
	}

	// キューブの描画
	for (const DebugObjData_Cube& data : mCubeDataVec) {
		mGeoPriCube->Draw(
			data.world,
			view,
			proj,
			Vector4(data.color.x, data.color.y, data.color.z, 1.0f)
		);
	}

	// ティーカップの描画
	for (const DebugObjData_TeaPot& data : mTeaPotDataVec) {
		mGeoPriTeaPot->Draw(
			data.world,
			view,
			proj,
			Vector4(data.color.x, data.color.y, data.color.z, 1.0f)
		);
	}

	// 配列を空に
	this->Clear();
}