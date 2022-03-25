/**
 * @file AutoDrivePointObject.cpp
 * @brief 自動走行時に使うポイント
 * @author suzuki.shunsuke
 * @date 2022/03/25
 */
#include "pch.h"
#include "AutoDrivePointObject.h"
#include "Game/Camera/CameraManager.h"
#include "Game/Character/CharacterManager.h"
#include "Libraries/MyLibraries/Utility.h"
#include "AutoDriveManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//=====================================================
//		コンストラクタ
//=====================================================
AutoDrivePointObject::AutoDrivePointObject(Vector3 pos)
	:mPos(pos)
	,mpGeometricPrimitive()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	auto context = pDR->GetD3DDeviceContext();
	mpGeometricPrimitive = GeometricPrimitive::CreateCone(context);
}

//=====================================================
//		描画
//=====================================================
void AutoDrivePointObject::Render()
{
	Matrix view = CameraManager::GetInstance()->GetViewMatrix();
	Matrix proj = CameraManager::GetInstance()->GetProjectionMatrix();

	mpGeometricPrimitive->Draw(
		Matrix::CreateScale(Vector3(20,30,20)) *
		Matrix::CreateTranslation(mPos),
		view,
		proj,
		Colors::Green,
		nullptr,
		true
	);
}

//=====================================================
//		対象物が自オブジェクトに触れたか
//=====================================================
bool AutoDrivePointObject::IsHitObject()
{
	CharacterActor* driver = AutoDriveManager::GetInstance()->GetDriveCharacter();

	return Vector3::Distance(thf::GetLocation(driver), mPos) < 10.0f;
}

//=====================================================
//		オブジェクトポイントを生成する
//=====================================================
AutoDrivePointObject* AutoDrivePointObject::CreatePointObj(
	Type type,
	DirectX::SimpleMath::Vector3 pos
)
{
	AutoDrivePointObject* pObj = nullptr;

	switch (type)
	{
		case AutoDrivePointObject::Type::Start:
			pObj = new AutoDrivePoint_Start(pos);
			break;
		case AutoDrivePointObject::Type::Run:
			pObj = new AutoDrivePoint_Run(pos);
			break;
		case AutoDrivePointObject::Type::Goal:
			pObj = new AutoDrivePoint_Goal(pos);
			break;
		default:
			assert(!"NoneType!");
			break;
	}

	return pObj;
}

//----------------------------　スタート　----------------------------

//=====================================================
//		更新	指定位置に瞬間移動させてすぐ終わる
//=====================================================
bool AutoDrivePoint_Start::Update()
{
	CharacterActor* driver = AutoDriveManager::GetInstance()->GetDriveCharacter();
	thf::SetLocation(driver, mPos + Vector3(0, 5, 0));
	return true;
}

//----------------------------　ラン　----------------------------

//=====================================================
//		更新	指定位置までドライバーを進行させる
//=====================================================
bool AutoDrivePoint_Run::Update()
{
	CharacterActor* driver = AutoDriveManager::GetInstance()->GetDriveCharacter();
	Vector3 way = myf::GetWay(mPos, thf::GetLocation(driver));
	driver->AddForce(way * 1.0f);

	return IsHitObject();
}

//----------------------------　ゴール　----------------------------

//=====================================================
//		更新
//=====================================================
bool AutoDrivePoint_Goal::Update()
{
	return false;
}
