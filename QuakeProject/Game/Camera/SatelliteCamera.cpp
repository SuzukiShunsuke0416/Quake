/**
 * @file SatelliteCamera.cpp
 * @brief 上から見た視点のカメラ。衛星カメラ。
 * @author suzuki.shunsuke
 * @date 2022/03/23
 */
#include "pch.h"
#include "SatelliteCamera.h"
#include "Game/Stage/StageManager.h"
#include "Game/Camera/CameraManager.h"
#include "Game/UtilityManager/InputManager.h"
#include "Libraries/MyLibraries/Utility.h"
#include "Libraries/MyLibraries/ScreenToWorld.h"
#include "Game/Collision/CollisionManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//=====================================================
//		コンストラクタ
//=====================================================
SatelliteCamera::SatelliteCamera(Actor* pOwner)
	:MyCamera(pOwner)
	,mDistanceRat(0.0f)
	,mTargetPos()
	,mEyeOriginPos()
	,mWheelValue(0)
{
}

//=====================================================
//		初期化
//=====================================================
void SatelliteCamera::Initialize()
{
	this->SetUpVector(Vector3(0, 0, -1));
	CameraManager::GetInstance()->ChangeActiveCamera(this->GetID());
	
	Vector3 centerPos = StageManager::GetInstance()->GetCenterPos();
	mTargetPos = centerPos * Vector3(1, 0, 1);
	mEyeOriginPos = mTargetPos + Vector3(0, 250, 0);

	this->CallCalculateViewMatrixFunction();
}

//=====================================================
//		更新
//=====================================================
void SatelliteCamera::Update()
{
	this->CalculateDistance();

	this->CallCalculateViewMatrixFunction();

	this->CalculateTargetPos();
}

//=====================================================
//		ビュー行列計算関数を呼ぶ
//=====================================================
void SatelliteCamera::CallCalculateViewMatrixFunction()
{
	this->SetTargetPosition(mTargetPos);
	this->SetEyePosition(myf::Lerp(mEyeOriginPos, mTargetPos, mDistanceRat));

	this->CalculateViewMatrix();
}

//=====================================================
//		ターゲット座標を計算する
//=====================================================
void SatelliteCamera::CalculateTargetPos()
{
	Vector3 startPos;
	Vector3 endPos;
	Vector3 hitPos;

	myf::CalculateStartandEndPointsFromMousePos(startPos, endPos);

	DebugLogManager::GetInstance()->EntryLog(startPos, L"start");
	DebugLogManager::GetInstance()->EntryLog(endPos, L"end");

	CollisionManager::Ground_vs_Ray(startPos, endPos, hitPos);

	DebugObjManager::GetInstance()->Entry(hitPos, 5.0f);
	DebugObjManager::GetInstance()->EntryLine(startPos,endPos);
}

//=====================================================
//		地面との距離差を計算する
//=====================================================
void SatelliteCamera::CalculateDistance()
{
	int wheelVal = InputManager::GetInstance()->
		GetMouseTracker().GetLastState().scrollWheelValue;
	mWheelValue.Act += (wheelVal - mWheelValue.Hoz);
	mWheelValue.Hoz = wheelVal;
	myf::ClampRef(mWheelValue.Act, 1700, 0);

	mDistanceRat = myf::Clamp(float(mWheelValue.Act) * 0.0005f, 0.99f, 0.0f);
	DebugLogManager::GetInstance()->EntryLog(mWheelValue.Act, L"whel");
}
