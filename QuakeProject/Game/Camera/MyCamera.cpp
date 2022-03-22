/**
 * @file MyCamera.cpp
 * @brief 基本的なカメラクラス。用途に合わせて継承。
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#include "pch.h"
#include "MyCamera.h"
#include "CameraManager.h"

 //=====================================================
 //		コンストラクタ
 //=====================================================
MyCamera::MyCamera(Actor* pOwner)
	: Camera()
	, Component(pOwner)
{
	//マネージャに追加
	CameraManager::GetInstance()->AddMember(this);
}

//=====================================================
//		デストラクタ
//=====================================================
MyCamera::~MyCamera()
{
	//マネージャから除外
	CameraManager::GetInstance()->RemoveMember(this);
}
