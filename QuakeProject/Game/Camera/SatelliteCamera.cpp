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
	,mChoosingPoint()
{
}

//=====================================================
//		初期化
//=====================================================
void SatelliteCamera::Initialize()
{
	CameraManager::GetInstance()->ChangeActiveCamera(this->GetID());
	
	this->SetTargetPosDefault();
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

	this->CalculateChoosingPoint();
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
//		カーソル指定点を計算する
//=====================================================
void SatelliteCamera::CalculateChoosingPoint()
{
	Vector3 startPos;
	Vector3 endPos;

	myf::CalculateStartandEndPointsFromMousePos(startPos, endPos);

	DebugLogManager::GetInstance()->EntryLog(startPos, L"start");
	DebugLogManager::GetInstance()->EntryLog(endPos, L"end");

	CollisionManager::Ground_vs_Ray(startPos, endPos, mChoosingPoint);

	DebugObjManager::GetInstance()->Entry(mChoosingPoint, 5.0f);
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

//=====================================================
//		ターゲット座標を滑らかに変更する
//=====================================================
void SatelliteCamera::SetTargetPosRubber(const Vector3& goal)
{
	myf::RubberComplementSlow(mTargetPos, goal);
	this->SetUpVector((mTargetPos - mEyeOriginPos) * Vector3(1, 0, 1));
}

//=====================================================
//		ターゲット座標を滑らかにデフォルト値にする
//=====================================================
void SatelliteCamera::SetTargetPosDefaultRubber()
{
	Vector3 centerPos = StageManager::GetInstance()->GetCenterPos();
	this->SetTargetPosRubber(centerPos * Vector3(1, 0, 1));

	Vector3 up = this->GetUpVector();
	myf::RubberComplementSlow(up, Vector3(0, 0, -1));
	this->SetUpVector(up);
}

//=====================================================
//		ターゲット座標をデフォルト値にする
//=====================================================
void SatelliteCamera::SetTargetPosDefault()
{
	Vector3 centerPos = StageManager::GetInstance()->GetCenterPos();
	mTargetPos = centerPos * Vector3(1, 0, 1);
	this->SetUpVector(Vector3(0, 0, -1));
}
